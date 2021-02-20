//=============================================================================
//
// player�w�b�_ [player.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "rank_ui.h"
#include "locus.h"
#include "course.h"
#include "item_ui.h"
#include "trap.h"
#include "attack.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH     "./data/Models/player.x" // ���f���̃p�X
#define SHADER_PATH    "./data/HLSL/Shader.fx"  // HLSL�t�@�C���̃p�X
#define LOCUS_DISTANCE 80.0f                    // �O�Ղ��o������
#define LOCUS_ADJUST 10.0f                      // �O�Ղ��o�������̒���
#define TRAP_SET_DISTACE 150.0f                 // �g���b�v��u������
#define ATTACK_SET_DISTACE 300.0f                 // �g���b�v��u������

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CPlayer::m_model = {};  //���b�V�����ւ̃|�C���^
CScene::Shader CPlayer::m_shader = {};  // �V�F�[�_�[�\����

//******************************
// �R���X�g���N�^
//******************************
CPlayer::CPlayer() :CCharacter(OBJTYPE_PLAYER)
{
	// �ϐ��̃N���A
	m_nPlayerNum = -1;  // �v���C���[ID
}

//******************************
// �f�X�g���N�^
//******************************
CPlayer::~CPlayer()
{
}

//******************************
// �N���G�C�g
//******************************
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum)
{
	// �������̊m��
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	// �v���C���[�ԍ��̐ݒ�
	pPlayer->m_nPlayerNum = nPlayerNum; 

	// ������
	pPlayer->Init();

	// �e�l�̑���E�Z�b�g
	pPlayer->SetPos(pos); // ���W�̃Z�b�g
	pPlayer->SetRot(rot); // ��]�̃Z�b�g
	
	return pPlayer;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// �e�N�X�`���ǂݍ���
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	// HLSL�t�@�C���̓ǂݍ���
	D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CPlayer::Unload(void)
{
	//���b�V���̔j��
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//******************************
// ����������
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// �V�F�[�_�[�̃Z�b�g
	SetShader(m_shader);

	// ����UI�̐���
	CRankUi::Create(m_nPlayerNum);
	// �A�C�e��UI�̐���
	CItemUi::Create(m_nPlayerNum);
	return S_OK;
}

//******************************
// �I������
//******************************
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();
}

//******************************
// �X�V����
//******************************
void CPlayer::Update(void)
{
	// �O�՗p�f�[�^�̕ێ�
	D3DXVECTOR3 posOld = GetPos();
	float fAngeleOld = -GetRot().y;

	if (!GetGoalFlag())
	{
		// �ړ��ʂ̐ݒ�
		Move();
	}
	else
	{
		// �ړ��ʂ̃Z�b�g
		SetMoveDist(VEC3_ZERO);
	}

	// �L�����N�^�[�N���X�̍X�V����*�ړ������E�����̏����E�d�͏����E�����蔻��
	CCharacter::Update();

	// �R�[�X�̓����蔻��
	CGame::GetCourse()->CollisionCharacter((CCharacter*)this);

	// �A�C�e���g�p
	UseItem();

	//***********************************************************
	// ��Ŋ֐���������
	
	if (GetActiveAcceleration())
	{// ������Ԃ̎�

		// �O�Ղ��o��
		// �v���C���[�̌����Ă�������̎擾
		float fAngele = -GetRot().y;

		D3DXVECTOR3 aPos[NUM_VERTEX / 2] =
		{
			{ GetPos().x + cosf(fAngele) *  LOCUS_DISTANCE, GetPos().y + LOCUS_ADJUST, GetPos().z + sinf(fAngele) *  LOCUS_DISTANCE },
			{ GetPos().x + cosf(fAngele) * -LOCUS_DISTANCE, GetPos().y + LOCUS_ADJUST, GetPos().z + sinf(fAngele) * -LOCUS_DISTANCE },
		};

		D3DXVECTOR3 aPosOld[NUM_VERTEX / 2] =
		{
			{ posOld.x + cosf(fAngeleOld) *  LOCUS_DISTANCE , posOld.y + LOCUS_ADJUST, posOld.z + sinf(fAngeleOld) *  LOCUS_DISTANCE },
			{ posOld.x + cosf(fAngeleOld) * -LOCUS_DISTANCE , posOld.y + LOCUS_ADJUST, posOld.z + sinf(fAngeleOld) * -LOCUS_DISTANCE },
		};
		CLocus::Create(aPos, aPosOld, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	}
	//***********************************************************
}

//******************************
// �`�揈��
//******************************
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// �ړ��ʂ̐ݒ菈��
//******************************
void CPlayer::Move(void)
{
	// �ړ������x�N�g��
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W)||GetActiveAcceleration())
	{
		// ���W�̎擾
		D3DXVECTOR3 pos = GetPos();
		// �J�����̈ʒu�̎擾
		D3DXVECTOR3 cameraPos = CGame::GetCamera(m_nPlayerNum)->GetPos();

		// �J�������玩���̕����x�N�g��
		move = pos - cameraPos;
		// Y�����ɂ͈ړ����Ȃ�
		move.y = 0.0f;
		// ���K��
		D3DXVec3Normalize(&move, &move);
		// �X�s�[�h��������
		move *= GetSpeed();
	}
	
	// �ړ��ʂ̃Z�b�g
	SetMoveDist(move);

}

//******************************
// �A�C�e���g�p�̏���
//******************************
void CPlayer::UseItem(void)
{
	if (GetItem() != CItem::ITEM_NONE && CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		
		switch (GetItem())
		{
		case CItem::ITEM_ACCELERATION:
			// �������[�h�ɂ���
			SetActiveAcceleration(true);

			break;
		case CItem::ITEM_ATTACK:
		{
			// ������o��
			D3DXVECTOR3 attackPos = GetPos();
			// �v���C���[�̌����W�̎擾
			float fAngle = (-GetRot().y) + D3DXToRadian(90);
			attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
			attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

			// �U������
			CAttack::Create(attackPos, D3DXVECTOR3(0.0f, GetRot().y + D3DXToRadian(90), 0.0f), GetRankData().nRank ,GetID());

		}
			break;

		case CItem::ITEM_TRAP:
		{
			// ������o��
			D3DXVECTOR3 trapPos = GetPos();
			// �v���C���[�̌����W�̎擾
			float fAngle = (-GetRot().y) + D3DXToRadian(-90);
			trapPos.x += cosf(fAngle) * TRAP_SET_DISTACE;
			trapPos.z += sinf(fAngle) * TRAP_SET_DISTACE;

			// �g���b�v����
			CTrap::Create(trapPos);
		}
			break;
		default:
			break;
		}
		// �A�C�e�����̏�����
		SetItem(CItem::ITEM_NONE);
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		// ������o��
		D3DXVECTOR3 attackPos = GetPos();
		// �v���C���[�̌����W�̎擾
		float fAngle = (-GetRot().y) + D3DXToRadian(90);
		attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
		attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

		// �U������
		CAttack::Create(attackPos, D3DXVECTOR3(0.0f, GetRot().y + D3DXToRadian(90), 0.0f), GetRankData().nRank, GetID());
	}
}
