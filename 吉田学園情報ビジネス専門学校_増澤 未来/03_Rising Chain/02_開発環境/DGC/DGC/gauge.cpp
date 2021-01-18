//===================================================
//
//    �Q�[�W�N���X�̏���[gage.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
// �C���N���[�h
//**********************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "particle.h"

//**********************************
// �}�N����`
//**********************************
#define GAUGE_TEXTURE_PATH "./data/Textures/ChainGauge.png" //�e�N�X�`��
#define NUM_PARTICLE 1    // �p�[�e�B�N��
#define REDBAR_RATE 0.02f // �Ԃ��o�[�����X�Ɍ����Ă����̌W��

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CGauge::m_pTexture = NULL;

//==================================
// �R���X�g���N�^
//==================================
CGauge::CGauge() :CScene(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_fRedLife = 0;
	m_fBarWidht = 0.0f;
	m_fBarHeight = 0.0f;
	m_downPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMaxNum = 0;
	m_bRedGauge = false;
	m_pData = NULL;
	m_frontCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_parPos = VEC3_ZERO;
}

//==================================
// �f�X�g���N�^
//==================================
CGauge::~CGauge()
{
}

//==================================
// �N���G�C�g
//==================================
CGauge * CGauge::Create(float * pData, D3DXVECTOR3 leftPos, float fBarWidht, float m_fBarHeight, int m_nMaxNum, D3DXCOLOR frontCol, bool bRedGaouge)
{
	// �������̊m��
	CGauge *pGage;
	pGage = new CGauge;

	// �����̑��
	pGage->m_downPos = leftPos;         // ���[
	pGage->m_fBarWidht = fBarWidht;     // HP�o�[�̕�
	pGage->m_fBarHeight = m_fBarHeight; // HP�o�[�̍���
	pGage->m_nMaxNum = m_nMaxNum;       // �ő僉�C�t��
	pGage->m_bRedGauge = bRedGaouge;    // �ԃQ�[�W���g�����̃t���O     
	pGage->m_pData = pData;             // �Q�[�W�ƕR�Â��鐔�l�̃|�C���^
	pGage->m_frontCol = frontCol;       // �t�����g�J���[

	// ������
	pGage->Init();
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pGage->SetPriority(OBJTYPE_UI);

	return pGage;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CGauge::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, GAUGE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CGauge::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// ����������
//==================================
HRESULT CGauge::Init(void)
{
	// ���C�t�o�[�̒��_�̐ݒ�
	float fHeight = 0.0f;
	fHeight = m_fBarHeight * m_nMaxNum / m_nMaxNum;    // ���̌v�Z
	if (m_bRedGauge)
	{// �ԃQ�[�W���g����
		m_fRedLife = (*m_pData); // �ԃQ�[�W�̃��C�t��
	}
	else
	{// �g��Ȃ��Ƃ�
		m_fRedLife = 0.0f;
	}

	// �p�[�c�������[�v
	for (int nCntGauge = 0; nCntGauge < BAR_PARTS_MAX; nCntGauge++)
	{
		// �|���S���̐���
		m_apPolygon[nCntGauge] = CPolygon::Create(
			m_downPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		
		// ���_���W�̐ݒ�
		D3DXVECTOR3 vtxPos[NUM_VERTEX];

		if (nCntGauge == BAR_FRAME)
		{
			int nAdjust = 0;
			if (CGame::GetPlayerNum() <= 2)
			{
				nAdjust = 10;
			}
			else
			{
				nAdjust = 7;
			}
			vtxPos[0] = D3DXVECTOR3(m_downPos.x - m_fBarWidht - nAdjust, m_downPos.y - fHeight, 0.0f);
			vtxPos[1] = D3DXVECTOR3(m_downPos.x + m_fBarWidht          , m_downPos.y - fHeight, 0.0f);
			vtxPos[2] = D3DXVECTOR3(m_downPos.x - m_fBarWidht - nAdjust, m_downPos.y, 0.0f);
			vtxPos[3] = D3DXVECTOR3(m_downPos.x + m_fBarWidht          , m_downPos.y, 0.0f);
		}
		else
		{
			vtxPos[0] = D3DXVECTOR3(m_downPos.x - m_fBarWidht + 2, m_downPos.y - fHeight, 0.0f);
			vtxPos[1] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y - fHeight, 0.0f);
			vtxPos[2] = D3DXVECTOR3(m_downPos.x - m_fBarWidht + 2, m_downPos.y, 0.0f);
			vtxPos[3] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y, 0.0f);
		}

		m_apPolygon[nCntGauge]->SetVertexPos(vtxPos);
	}

	m_apPolygon[BAR_RED]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_apPolygon[BAR_FRONT]->SetColor(m_frontCol);
	m_apPolygon[BAR_FRAME]->SetTexture(m_pTexture);
	m_apPolygon[BAR_FRAME]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_parPos = m_downPos;
	return S_OK;
}

//==================================
// �I������
//==================================
void CGauge::Uninit(void)
{
	// ���C�t�̍ő吔���|���S���̏I������
	for (int nCntGauge = 0; nCntGauge < BAR_PARTS_MAX; nCntGauge++)
	{
		if (m_apPolygon[nCntGauge] != NULL)
		{
			// �I������
			m_apPolygon[nCntGauge]->Uninit();
			// �������̉��
			delete m_apPolygon[nCntGauge];
			m_apPolygon[nCntGauge] = NULL;
		}
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CGauge::Update(void)
{
	// ���C�t�o�[�̒��_�̐ݒ�

	// ���̌v�Z
	float fHeight = 0.0f;
	fHeight = m_fBarHeight * (*m_pData) / m_nMaxNum;

	if (m_bRedGauge)
	{// �ԃQ�[�W���g����
		m_fRedLife += ((*m_pData) - m_fRedLife)*REDBAR_RATE;
	}
	else
	{// �g��Ȃ��Ƃ�
		m_fRedLife = 0.0f;
	}

	// ���_���W�̐ݒ�
	D3DXVECTOR3 vtxPos[NUM_VERTEX];

	vtxPos[0] = D3DXVECTOR3(m_downPos.x - m_fBarWidht    , m_downPos.y - fHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y - fHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_downPos.x - m_fBarWidht    , m_downPos.y          , 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y          , 0.0f);
	
	m_apPolygon[BAR_FRONT]->SetVertexPos(vtxPos);

	// ���̌v�Z
	fHeight = m_fBarHeight * m_fRedLife / m_nMaxNum;

	// ���_���W�̐ݒ�

	vtxPos[0] = D3DXVECTOR3(m_downPos.x - m_fBarWidht    , m_downPos.y - fHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y - fHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_downPos.x - m_fBarWidht    , m_downPos.y          , 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_downPos.x + m_fBarWidht - 2, m_downPos.y          , 0.0f);

	m_apPolygon[BAR_RED]->SetVertexPos(vtxPos);

	// �|���S���̍ő吔���X�V����
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			m_apPolygon[nCntGage]->Update();
		}
	}
	
	// �p�[�e�B�N��
	//ParticleManager();
}

//==================================
// �`�揈��
//==================================
void CGauge::Draw(void)
{
	// �O�p�[�c���`��
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			m_apPolygon[nCntGage]->Draw();
		}
	}
}

//==================================
// �p�[�e�B�N��
//==================================
void CGauge::ParticleManager(void)
{
	m_parPos.y -= 3.0f;
	if (m_parPos.y <= m_downPos.y - (m_fBarHeight * (*m_pData) / m_nMaxNum))
	{
		m_parPos.y = m_downPos.y - 5.0f;
	}

	for (int nCnt = 0; nCnt < NUM_PARTICLE; nCnt++)
	{
		float fRand = (rand() % (int)m_fBarWidht) - m_fBarWidht / 2;
		CParticle::Create(D3DXVECTOR3(m_parPos.x + fRand, m_parPos.y+ (fRand/2), 0.0f), 
			VEC3_ZERO,
			D3DXVECTOR3(8.0f, 8.0f, 0.0f),
			rand()%20+20,
			D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 0.7f))->SetAddMode(true);
	}
	
}
