//===================================================
//
//    �w�i�N���X�̏���[bg.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"
#include "player.h"
#include "camera.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************

// �e�N�X�`���̃p�X
#define BG_TEXTURE_PATH1 "./data/Textures/bg001.png"
#define BG_TEXTURE_PATH2 "./data/Textures/bg000.png"
#define BG_TEXTURE_PATH3 "./data/Textures/bg002.png"

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CBg::m_apTexture[BG_TEXTURE_NUM] = {};

//==================================
// �R���X�g���N�^
//==================================
CBg::CBg():CScene(OBJTYPE_BG)
{
	// O�N���A
	memset(m_afTexMoveU, 0, sizeof(m_afTexMoveU));
	memset(m_afTexMoveV, 0, sizeof(m_afTexMoveV));
	memset(m_apScene3d, 0, sizeof(m_apScene3d));
}

//==================================
// �f�X�g���N�^
//==================================
CBg::~CBg()
{
}

//==================================
// �N���G�C�g
//==================================
CBg * CBg::Create(void)
{
	// �������̊m��
	CBg *pBg = new CBg;
	// ����������
	pBg->Init();

	return pBg;
}

//==================================
// ���[�h
//==================================
HRESULT CBg::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH1, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH2, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH3, &m_apTexture[2]);

	return S_OK;
}

//==================================
// �A�����[�h
//==================================
void CBg::Unload(void)
{
	// �e�N�X�`�������A�e�N�X�`���̊J������
	for (int nCntTex = 0; nCntTex < BG_TEXTURE_NUM; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// ����������
//==================================
HRESULT CBg::Init(void)
{
	for (int nCntBg = 0; nCntBg < BG_PARTS_NUM; nCntBg++)
	{
		// �|���S������
		m_apScene3d[nCntBg] = CScene3d::Create();
		if (m_apScene3d[nCntBg] != NULL)
		{
			// ���S���W�̐ݒ�
			m_apScene3d[nCntBg]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 10));
			// �T�C�Y�̐ݒ�
			m_apScene3d[nCntBg]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
			// �I�u�W�F�N�g�^�C�v�̐ݒ�
			m_apScene3d[nCntBg]->SetPriority(OBJTYPE_BG);

			// �e�N�X�`�����蓖��
			m_apScene3d[nCntBg]->BindTexture(m_apTexture[nCntBg]);
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==================================
// �I������
//==================================
void CBg::Uninit(void)
{
	// �w�i�̃p�[�c�����J������
	for (int nCntBg = 0; nCntBg < BG_PARTS_NUM; nCntBg++)
	{
		if (m_apScene3d[nCntBg] != NULL)
		{
			m_apScene3d[nCntBg]->Uninit();
			m_apScene3d[nCntBg] = NULL;
		}
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CBg::Update(void)
{
	D3DXVECTOR2 uv[NUM_VERTEX];

	// �e�N�X�`�������ɂ��炷
	m_afTexMoveV[0] -= 0.002f;
	m_afTexMoveV[1] -= 0.001f;
	m_afTexMoveV[2] -= 0.0009f;

	

	for (int nCntBg = 0; nCntBg < BG_PARTS_NUM; nCntBg++)
	{
		if (m_apScene3d[nCntBg] != NULL)
		{
			// UV���W�̐ݒ�
			uv[0] = D3DXVECTOR2(0.0f + m_afTexMoveU[nCntBg], 0.0f + m_afTexMoveV[nCntBg]);
			uv[1] = D3DXVECTOR2(1.0f + m_afTexMoveU[nCntBg], 0.0f + m_afTexMoveV[nCntBg]);
			uv[2] = D3DXVECTOR2(0.0f + m_afTexMoveU[nCntBg], 1.0f + m_afTexMoveV[nCntBg]);
			uv[3] = D3DXVECTOR2(1.0f + m_afTexMoveU[nCntBg], 1.0f + m_afTexMoveV[nCntBg]);


			// UV���W�̃Z�b�g
			m_apScene3d[nCntBg]->SetTextureUV(uv);
			if (CPlayer::IsAlive())
			{
				// �v���C���[�̎擾
				CPlayer*pPlayer = CGame::GetPlayer();
				if (pPlayer != NULL)
				{
					// �v���C���[���W�̎擾
					D3DXVECTOR3 playerPos = pPlayer->GetPos();
					// �ʒu��XY���v���C���[�Ɠ����ɂ���
					m_apScene3d[nCntBg]->SetPos(D3DXVECTOR3(playerPos.x, playerPos.y, 10));
				}
			}
		}

		// �T�C�Y���J�����̎���͈͂�
		m_apScene3d[nCntBg]->SetSize(D3DXVECTOR3(CGame::GetCamera()->GetCameraWidth(), CGame::GetCamera()->GetCameraHeight(), 0.0f));
	}
}

//==================================
// �`�揈��
//==================================
void CBg::Draw(void)
{
}
