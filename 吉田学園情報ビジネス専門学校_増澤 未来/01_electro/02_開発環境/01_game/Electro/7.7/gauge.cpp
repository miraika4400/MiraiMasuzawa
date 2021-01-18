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
#include "boss.h"

//**********************************
// �}�N����`
//**********************************
#define  GAUGE_TEXTURE_PATH "./data/Textures/HPBar.png" //�e�N�X�`��

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
	m_leftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMaxNum = 0;
	m_bRedGauge = false;
	m_pData = NULL;
	m_frontCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
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
	pGage->m_leftPos = leftPos;         // ���[
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
	float fWidth = 0.0f;
	fWidth = m_fBarWidht * m_nMaxNum / m_nMaxNum;    // ���̌v�Z
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
			m_leftPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		// ���_���W�̐ݒ�
		D3DXVECTOR3 vtxPos[NUM_VERTEX];

		vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
		vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

		m_apPolygon[nCntGauge]->SetVertexPos(vtxPos);
	}

	m_apPolygon[BAR_RED]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_apPolygon[BAR_FRONT]->SetColor(m_frontCol);
	m_apPolygon[BAR_FRAME]->BindTexture(m_pTexture);

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
	float fWidth = 0.0f;
	fWidth = m_fBarWidht * (*m_pData) / m_nMaxNum;

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

	vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);
	
	m_apPolygon[BAR_FRONT]->SetVertexPos(vtxPos);

	// ���̌v�Z
	fWidth = m_fBarWidht * m_fRedLife / m_nMaxNum;

	// ���_���W�̐ݒ�
	vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

	m_apPolygon[BAR_RED]->SetVertexPos(vtxPos);

	// �|���S���̍ő吔���X�V����
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			m_apPolygon[nCntGage]->Update();
		}
	}
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
