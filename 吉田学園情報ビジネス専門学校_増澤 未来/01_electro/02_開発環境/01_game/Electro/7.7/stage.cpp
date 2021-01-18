//===================================================
//
//    �X�e�[�W�N���X�̏���[stage.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"
#include "time.h"
#include "boss.h"
#include "warning_logo.h"
#include "game.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define STAGE_COL_CHANGE_COUNT 100    // �F��ς���C���^�[�o��
#define STAGE_COL_RATE 0.05f          // ���X�ɐF��ς���Ƃ��̌W��
// �e�N�X�`���̃p�X
#define STAGE_TEXTURE_PATH1 "./data/Textures/electro_stage001.png"
#define STAGE_TEXTURE_PATH2 "./data/Textures/electro_stage000.png"
#define STAGE_TEXTURE_PATH_10   "./data/Textures/electro_stage000_10.png" 
#define STAGE_TEXTURE_PATH_9    "./data/Textures/electro_stage000_9.png"  
#define STAGE_TEXTURE_PATH_8    "./data/Textures/electro_stage000_8.png"  
#define STAGE_TEXTURE_PATH_7    "./data/Textures/electro_stage000_7.png"  
#define STAGE_TEXTURE_PATH_6    "./data/Textures/electro_stage000_6.png"  
#define STAGE_TEXTURE_PATH_5    "./data/Textures/electro_stage000_5.png"  
#define STAGE_TEXTURE_PATH_4    "./data/Textures/electro_stage000_4.png"  
#define STAGE_TEXTURE_PATH_3    "./data/Textures/electro_stage000_3.png"  
#define STAGE_TEXTURE_PATH_2    "./data/Textures/electro_stage000_2.png"  
#define STAGE_TEXTURE_PATH_1    "./data/Textures/electro_stage000_1.png"  
#define STAGE_TEXTURE_PATH_BOSS "./data/Textures/electro_stage000_boss.png" 

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CStage::m_apTexture[STAGE_MAX] = {};

//==================================
// �R���X�g���N�^
//==================================
CStage::CStage(): CScene(OBJTYPE_MAP)
{
	memset(&pScene3d, 0, sizeof(pScene3d));
	m_distCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nColChangeCnt = 0;
	pWarning = NULL;
}

//==================================
// �f�X�g���N�^
//==================================
CStage::~CStage()
{
}

//==================================
// �N���G�C�g
//==================================
CStage * CStage::Create(void)
{
	//�� �����̊m��
	CStage *pStage;
	pStage = new CStage;
	// ������
	pStage->Init();

	return pStage;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CStage::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH1, &m_apTexture[STAGE_BACK]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH2, &m_apTexture[STAGE_FRONT]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_10, &m_apTexture[STAGE_10]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_9, &m_apTexture[STAGE_9]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_8, &m_apTexture[STAGE_8]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_7, &m_apTexture[STAGE_7]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_6, &m_apTexture[STAGE_6]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_5, &m_apTexture[STAGE_5]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_4, &m_apTexture[STAGE_4]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_3, &m_apTexture[STAGE_3]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_2, &m_apTexture[STAGE_2]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_1, &m_apTexture[STAGE_1]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_BOSS, &m_apTexture[STAGE_BOSS]);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CStage::Unload(void)
{
	for (int nCntTex = 0; nCntTex < STAGE_PATS_NUM; nCntTex++)
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
HRESULT CStage::Init(void)
{
	// �|���S������
	for (int nCntPolygon = 0; nCntPolygon < STAGE_PATS_NUM; nCntPolygon++)
	{
		if (pScene3d[nCntPolygon] == NULL)
		{
			// �N���G�C�g
			pScene3d[nCntPolygon] = CScene3d::Create();
			// ���W�̐ݒ�
			pScene3d[nCntPolygon]->SetPos(STAGE_POS);
			// �T�C�Y�̐ݒ�
			pScene3d[nCntPolygon]->SetSize(STAGE_SIZE);
			// �I�u�W�F�N�g�^�C�v�̐ݒ�
			pScene3d[nCntPolygon]->SetPriority(OBJTYPE_MAP);
		}
	}
	//�e�N�X�`���̊��蓖��
	pScene3d[0]->BindTexture(m_apTexture[STAGE_BACK]);
	pScene3d[1]->BindTexture(m_apTexture[STAGE_FRONT]);
	// ���Z�����ɂ���
	pScene3d[1]->SetAddMode(true);

	// �ڕW�J���[�������_���ɏ�����
	m_distCol.r = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.g = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.b = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.a = 1.0f;
	// �J�E���g�̏�����
	m_nColChangeCnt = 0;

	return S_OK;
}

//==================================
// �I������
//==================================
void CStage::Uninit(void)
{
	for (int nCntPolygon = 0; nCntPolygon < STAGE_PATS_NUM; nCntPolygon++)
	{
		if (pScene3d[nCntPolygon] != NULL)
		{
			// �I������
			pScene3d[nCntPolygon]->Uninit();
			// �N���G�C�g
			pScene3d[nCntPolygon] = NULL;
		}
	}

	Release();
}

//==================================
// �X�V����
//==================================
void CStage::Update(void)
{
	// �J�E���g��i�߂�
	m_nColChangeCnt++;
	

	
	// ���݂̃J���[�̎擾
	D3DXCOLOR col = pScene3d[1]->GetColor();
	// ���X�ɐF��ς���
	col.r += (m_distCol.r - col.r)*STAGE_COL_RATE;
	col.g += (m_distCol.g - col.g)*STAGE_COL_RATE;
	col.b += (m_distCol.b - col.b)*STAGE_COL_RATE;
	col.a = 0.6f;

	// �J���[�̐ݒ�
	pScene3d[1]->SetColor(col);

	
	if (CGame::GetWave() == CGame::WAVE_BOSS)
	{// �{�X�펞

	    // �e�N�X�`���̐؂�ւ�
		pScene3d[1]->BindTexture(m_apTexture[STAGE_BOSS]);
		
		// ���J�E���g��
		if (m_nColChangeCnt%STAGE_COL_CHANGE_COUNT == 0)
		{
			// �ԐF�ɂ���
			m_distCol = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f);
		}
		else if (m_nColChangeCnt % (STAGE_COL_CHANGE_COUNT / 2) == 0)
		{
			// ���F�ɂ���
			m_distCol = D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.6f);
		}

		// �J���[�̐ݒ�
		pScene3d[1]->SetColor(col);
	}
	else
	{// �e�N�X�`���̐؂�ւ��Ń{�X��̃J�E���g�_�E��

		// ���J�E���g��
		if (m_nColChangeCnt%STAGE_COL_CHANGE_COUNT == 0)
		{
			// �ڕW�J���[�������_���ɏ�����
			m_distCol.r = ((float)(rand() % 50) / 100) + 0.5f;
			m_distCol.g = ((float)(rand() % 50) / 100) + 0.5f;
			m_distCol.b = ((float)(rand() % 50) / 100) + 0.5f;
		}

		// 10�b�O����e�N�X�`���̐؂�ւ�
		for (int nCntTime = 0; nCntTime < 10; nCntTime++)
		{
			// �o�ߎ��Ԃ̎擾
			int nTime = (CTime::GetTime() / 1000);
			if (nTime == BOSS_SPAWN_SEC - (nCntTime + 1))
			{
				if (pScene3d[1]->GetTexture() != m_apTexture[(BOSS_SPAWN_SEC - nTime) - 1])
				{
					// �e�N�X�`���̐؂�ւ�
					pScene3d[1]->BindTexture(m_apTexture[(BOSS_SPAWN_SEC - nTime) - 1]);
					// SE�Đ�
					CManager::GetSound()->Play(CSound::LABEL_SE_COUNT);
				}

				// warning���S���o��
				if (pWarning == NULL)
				{
					pWarning = CWarningLogo::Create();
				}
			}
		}
	}

	/*if ((CTime::GetTime() / 1000) == 59)
	{
		
	}
	if ((CTime::GetTime() / 1000) == 58)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_2]);
	}
	if ((CTime::GetTime() / 1000) == 57)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_3]);
	}
	if ((CTime::GetTime() / 1000) == 56)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_4]);
	}
	if ((CTime::GetTime() / 1000) == 55)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_5]);
	}
	if ((CTime::GetTime() / 1000) == 54)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_6]);
	}
	if ((CTime::GetTime() / 1000) == 53)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_7]);
	}
	if ((CTime::GetTime() / 1000) == 52)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_8]);
	}
	if ((CTime::GetTime() / 1000) == 51)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_9]);
	}
	if ((CTime::GetTime() / 1000) == 50)
	{
		pScene3d[STAGE_FRONT]->BindTexture(m_apTexture[STAGE_10]);
	}*/
}

//==================================
// �`�揈��
//==================================
void CStage::Draw(void)
{
}
