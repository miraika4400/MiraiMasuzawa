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

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH     "./data/Models/player.x" // ���f���̃p�X
#define SHADER_PATH    "./data/HLSL/Shader.fx"  // HLSL�t�@�C���̃p�X
#define PLAYER_SPEED 30.0f                      // �ړ��X�s�[�h
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

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();
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

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
	{
		// ���W�̎擾
		D3DXVECTOR3 pos = GetPos();
		// �J�����̈ʒu�̎擾
		D3DXVECTOR3 cameraPos = CGame::GetCamera()->GetPos();

		// �J�������玩���̕����x�N�g��
		move = pos - cameraPos;
		// Y�����ɂ͈ړ����Ȃ�
		move.y = 0.0f;
		// ���K��
		D3DXVec3Normalize(&move, &move);
		// �X�s�[�h��������
		move *= PLAYER_SPEED;
	}
	
	// �ړ��ʂ̃Z�b�g
	SetMoveDist(move);

}
