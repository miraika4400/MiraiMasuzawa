////////////////////////////////////////////////////
//
//    EvoEffect�N���X�̏���[EvoEffect.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "evolution_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "destination.h"
//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH         "./data/Models/Evolution.x"	// �ԃ��f���̃p�X
#define INIT_SIZE D3DXVECTOR3(0.4f,0.4f,0.4f) // �����T�C�Y
#define DIST_SIZE D3DXVECTOR3(1.0f,1.0f,1.0f) // �ڕW�T�C�Y
#define SIZE_RATE 0.03f                       // �T�C�Y���Z�W��
#define LIFE      60                          // ����

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CEvoEffect::m_model = {};   	//���b�V�����ւ̃|�C���^


//******************************
// �R���X�g���N�^
//******************************
CEvoEffect::CEvoEffect() :CModel(OBJTYPE_EFFECT)
{
	// �ϐ��̃N���A
	m_nPlayerNum = -1;                             // �h���b�v���ǂ̃v���C���[�����Ƃ�����
	m_nLife = 0;                                   // ����
	m_fAlfa = 1.0f;                                // �A���t�@�l
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);        // �J���[
	m_distCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �J���[�ڕW�l
}

//******************************
// �f�X�g���N�^
//******************************
CEvoEffect::~CEvoEffect()
{
}

//******************************
// �N���G�C�g
//******************************
CEvoEffect * CEvoEffect::Create(int nPlayerNum)
{
	// �������̊m��
	CEvoEffect *pEvoEffect;
	pEvoEffect = new CEvoEffect;
	// �v���C���[�ԍ�
	pEvoEffect->m_nPlayerNum = nPlayerNum;
	// ������
	pEvoEffect->Init();
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pEvoEffect->SetPriority(OBJTYPE_UI);
	
	return pEvoEffect;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CEvoEffect::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.nNumMat,
		&m_model.pMesh);

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

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CEvoEffect::Unload(void)
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
HRESULT CEvoEffect::Init(void)
{
	if (FAILED(CModel::Init()))
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

	SetSize(INIT_SIZE);

	m_nLife = 0;
	m_fAlfa = 1.0f;    // �A���t�@�l
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);        // �J���[

	switch (m_nPlayerNum)
	{
	case 0:
		m_distCol = D3DXCOLOR(0.7f, 0.0f, 0.0f, 0.0f); // ��
		break;
	case 1:
		m_distCol = D3DXCOLOR(0.0f, 0.0f, 0.7f, 0.0f); // ��
		break;
	case 2:
		m_distCol = D3DXCOLOR(0.7f, 0.7f, 0.0f, 0.0f); // ��
		break;
	case 3:
		m_distCol = D3DXCOLOR(0.0f, 0.7f, 0.0f, 0.0f); // ��
		break;
	default:
		break;
	}
	return S_OK;
}

//******************************
// �I������
//******************************
void CEvoEffect::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CEvoEffect::Update(void)
{
	// �v���C���[���̎擾
	CPlayer*pPlayer = CGame::GetPlayer(m_nPlayerNum);
	if (pPlayer != NULL)
	{
		SetPos(pPlayer->GetPos());
	}

	// �T�C�Y��傫������
	D3DXVECTOR3 size = GetSize();
	size += (DIST_SIZE - size)*SIZE_RATE;
	SetSize(size);

	if (size.x >= DIST_SIZE.x - 0.01)
	{
		Uninit();
	}

	// ��]������
	D3DXVECTOR3 addRot = D3DXVECTOR3(0.0f, 0.05f, 0.0f);
	SetRot(GetRot() + addRot);

	// �����̏���
	m_nLife++;

	if (m_nLife >= LIFE/3)
	{
		// �F��ڕW�l�ɋ߂Â���
		m_col.r += (m_distCol.r - m_col.r)*0.05;
		m_col.g += (m_distCol.g - m_col.g)*0.05;
		m_col.b += (m_distCol.b - m_col.b)*0.05;
		m_col.a += (m_distCol.a - m_col.a)*0.05;
		m_fAlfa -= 0.05f;
	}
	if (m_nLife >= LIFE)
	{
		Uninit();
	}
}

//******************************
// �`�揈��
//******************************
void CEvoEffect::Draw(void)
{
	// ���ׂẴp�[�c��ԐF�ɂ���

	D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse = m_col;
	}

	CModel::Draw();
}