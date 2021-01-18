//===================================================
//
//    �^�C���N���X�̏���[time.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "time.h"
#include "number.h"
#include "manager.h"

//==============================
//�ÓI�����o�ϐ��錾
//==============================
int CTime::m_nTime = 0;
int CTime::m_nPauseCurrentTime = 0;
//==================================
// �R���X�g���N�^
//==================================
CTime::CTime() :CScene(OBJTYPE_UI)
{
	// �i���o�[�̃N���A
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nCurrentTime = timeGetTime();
	m_nPauseTime = 0;
	m_nPauseCurrentTime = 0;
}

//==================================
// �f�X�g���N�^
//==================================
CTime::~CTime()
{
}

//==================================
// �N���G�C�g
//==================================
CTime * CTime::Create(void)
{
	// �������̊m��
	CTime *pTime = new CTime;

	// ����������
	pTime->Init();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pTime->SetPriority(OBJTYPE_UI);

	return pTime;
}

//==================================
// ����������
//==================================
HRESULT CTime::Init(void)
{
	// �ő啪���[�v
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		// �i���o�[����
		m_apNumber[nCntDigit] = CNumber::Create(0,
			D3DXVECTOR3(SCREEN_WIDTH/2-30*2+(nCntDigit*40), 120.0f, 0.0f),
			D3DXVECTOR3(20, 20, 0),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// �^�C���̏�����
	m_nTime = 0;
	// �Q�[���J�n�����̎擾
	m_nCurrentTime = timeGetTime();
	return S_OK;
}

//==================================
// �I������
//==================================
void CTime::Uninit(void)
{
	// �ő啪���[�v
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		if (m_apNumber[nCntDigit] != NULL)
		{
			// �I������
			m_apNumber[nCntDigit]->Uninit();
			delete m_apNumber[nCntDigit];
			m_apNumber[nCntDigit] = NULL;
		}
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CTime::Update(void)
{
	// ���ݎ����̎擾
	int nNowTime = timeGetTime();
	// �Q�[���̌o�ߎ��Ԃ̌v�Z
	m_nTime = (nNowTime - m_nCurrentTime);

	// �ő啪���[�v
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		// �i���o�[�̃A�b�v�f�[�g
		m_apNumber[nCntDigit]->Update();

		// �e���̌v�Z
		m_apNumber[nCntDigit]->SetNumber(((m_nTime/10) % (int)(powf(10.0f, (MAX_TIME_NUM - nCntDigit)))) / (float)(powf(10.0, (MAX_TIME_NUM - nCntDigit - 1))));
	}

	// �|�[�Y��ʒ��̃^�C���̃v���X
	if (!CManager::GetActivePause() && m_nPauseCurrentTime != 0)
	{
		// �|�[�Y���̃^�C���̉��Z
		m_nCurrentTime += (nNowTime - m_nPauseCurrentTime);
		// �|�[�Y�^�C���̏�����
		m_nPauseCurrentTime = 0;
	}
}

//==================================
// �`�揈��
//==================================
void CTime::Draw(void)
{
#ifdef _DEBUG
	// �ő啪���[�v
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		m_apNumber[nCntDigit]->Draw();
	}
#endif // _DEBUG
}
