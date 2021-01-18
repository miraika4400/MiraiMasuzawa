//===================================================
//
//    �����L���O�N���X�̏���[ranking.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "tcp_client.h"
#include "ranking.h"
#include "number.h"

//**********************************
// �}�N����`
//**********************************
#define RANK_NUMBER_SIZE 30.0f       // �����N�i���o�[�̃T�C�Y
#define NOW_SCORE_SIZE 25.0f         // ���݃X�R�A�i���o�[�̃T�C�Y
#define RANK_INIT_POS_X 440.0f       // �����N�i���o�[�z�u��X�̏����ʒu
#define RANK_INIT_POS_Y 60.0f        // �����N�i���o�[�z�u��Y�̏����ʒu�@�@�@�@
#define NOW_SCORE_INIT_POS_X 510.0f  // ���݃X�R�A�z�u��X�����ʒu
#define NOW_SCORE_POS_Y 665.0f       // ���݃X�R�A��Y�ʒu
#define FLASH_COUNT 30               // �_�ł̃J�E���g
//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
int      CRanking::m_nRanking[MAX_RANK] = {};    // �����L���O�̃X�R�A�̊i�[�p
CNumber* CRanking::m_pNowScore[MAX_SCORE_DIGIT] = {}; // �X�R�A�|���S��
int CRanking::m_nNowScore = 0;

//==================================
// �R���X�g���N�^
//==================================
CRanking::CRanking()
{
	// �i���o�[�̃N���A
	memset(m_apRankNumber, 0, sizeof(m_apRankNumber));
	memset(m_apScoreNumber, 0, sizeof(m_apScoreNumber));
	memset(m_pNowScore, 0, sizeof(m_pNowScore));
	m_nCntFlash = 0;
}

//==================================
// �f�X�g���N�^
//==================================
CRanking::~CRanking()
{
}


//==============================
// �����L���O�擾
//==============================
void CRanking::GetRanking(void)
{
	WSADATA wsaData;
	CTcpClient *pTcpClient = NULL;

	//WinSock�̏�����
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//���炩�̌����ŏ������Ɏ��s����
		printf("WSAStartup failed\n");
		return;
	}

	if (pTcpClient == NULL)
	{
		//�������̊m��
		pTcpClient = CTcpClient::Create();

		if (pTcpClient == NULL)
		{
			//winsock�I������
			WSACleanup();
			return;
		}
	}

	///////////////////////////////////////////////////

	// Send
	char aSendBuf[BUFFER_NUM];
	aSendBuf[0] = CTcpClient::COMMAND_TYPE_GET_RANKING;

	pTcpClient->Send(aSendBuf, strlen(aSendBuf));

	// Recv
	char aRecvBuf[BUFFER_NUM];
	memset(&aRecvBuf[0], 0, sizeof(aRecvBuf));

	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// �X�R�A�̃R�s�[
		int nScore = *(int*)&aRecvBuf[nIndex];
		nScore = ntohl(nScore);
		nIndex += sizeof(int);
		m_nRanking[nCntRank] = nScore;
	}

	///////////////////////////////////////////////////

	//winsock�I������
	WSACleanup();

	//�������̉��
	pTcpClient->Release();
}

//==============================
// �����L���O�ݒ�
//==============================
int CRanking::SetRanking(void)
{
	WSADATA wsaData;
	CTcpClient *pTcpClient = NULL;

	//WinSock�̏�����
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//���炩�̌����ŏ������Ɏ��s����
		printf("WSAStartup failed\n");
		return -1;
	}

	if (pTcpClient == NULL)
	{
		//�������̊m��
		pTcpClient = CTcpClient::Create();

		if (pTcpClient == NULL)
		{
			//winsock�I������
			WSACleanup();
			return -1;
		}
	}

	///////////////////////////////////////////////////

	char aSendBuf[BUFFER_NUM];

	// ���N�G�X�g
	aSendBuf[0] = CTcpClient::COMMAND_TYPE_SET_RANKING;
	// �G���f�B�A���ϊ�
	int nTime = htonl(m_nNowScore);

	// �^�C���̃R�s�[
	memcpy(&aSendBuf[1], &nTime, sizeof(int));

	int nLen = strlen(aSendBuf);
	pTcpClient->Send(aSendBuf, 13);

	// ���X�|���X

	char aRecvBuf[BUFFER_NUM];

	memset(&aRecvBuf[0], 0, sizeof(aRecvBuf));

	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int nRecvRank = (int)aRecvBuf[0];

	//nRecvRank = ntohl(nRecvRank);

	///////////////////////////////////////////////////

	//winsock�I������
	WSACleanup();

	//�������̉��
	pTcpClient->Release();

	return nRecvRank;
}

//==================================
// �N���G�C�g
//==================================
CRanking * CRanking::Create(void)
{
	// �������̊m��
	CRanking *pRanking = new CRanking;

	// ����������
	pRanking->Init();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pRanking->SetPriority(OBJTYPE_UI);

	return pRanking;
}

//==================================
// �Z�b�g�����L���O
//==================================
void CRanking::SetRanking(int nScore)
{
	m_nNowScore = nScore;
	SetRanking();
	GetRanking();

	// ���̃X�R�A�̕\��
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		// �i���o�[�̃Z�b�g
		m_pNowScore[nCntDigit]->SetNumber((nScore % (int)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit)))) / (float)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit - 1))));
	}
}

//==================================
// ����������
//==================================
HRESULT CRanking::Init(void)
{
	// �F
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �ő包�������[�v
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		switch (nCntRank)
		{
		case 0:
			// ��ʂ̎��̃J���[
			col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;

		case 1:
			// ��ʂ̎��̃J���[
			col = D3DXCOLOR(0.45f, 0.8f, 1.0f, 1.0f);
			break;

		case 2:
			// �O�ʂ̎��̃J���[
			col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
			break;

		default:
			// �f�t�H���g�̃J���[
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		// �|���S������
		m_apRankNumber[nCntRank] = CNumber::Create(nCntRank + 1,
			D3DXVECTOR3(RANK_INIT_POS_X, RANK_INIT_POS_Y + (RANK_NUMBER_SIZE* MAX_RANK/2) + (nCntRank * RANK_NUMBER_SIZE*2), 0.0f),
			D3DXVECTOR3(RANK_NUMBER_SIZE, RANK_NUMBER_SIZE, 0),
			col);

		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// �|���S������
			m_apScoreNumber[nCntRank][nCntDigit] = CNumber::Create(0,
				D3DXVECTOR3((float)(RANK_INIT_POS_X+100 + nCntDigit * RANK_NUMBER_SIZE * 2), RANK_INIT_POS_Y + (float)((RANK_NUMBER_SIZE * MAX_RANK / 2) + (nCntRank * RANK_NUMBER_SIZE*2)), 0.0f),
				D3DXVECTOR3(RANK_NUMBER_SIZE, RANK_NUMBER_SIZE, 0),
				col);
		}
	}

	// ���݂̃X�R�A
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit] = CNumber::Create(0,
			D3DXVECTOR3((float)(NOW_SCORE_INIT_POS_X + 100 + nCntDigit * RANK_NUMBER_SIZE * 2), NOW_SCORE_POS_Y, 0.0f),
			D3DXVECTOR3(NOW_SCORE_SIZE, NOW_SCORE_SIZE, 0),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	m_nCntFlash = 0;
	return S_OK;
}

//==================================
// �I������
//==================================
void CRanking::Uninit(void)
{
	// �ő僉���N��
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (m_apRankNumber[nCntRank] != NULL)
		{
			m_apRankNumber[nCntRank]->Uninit();
			delete m_apRankNumber[nCntRank];
			m_apRankNumber[nCntRank] = NULL;
		}
		// �ő包��
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			if (m_apScoreNumber[nCntRank][nCntDigit] != NULL)
			{
				m_apScoreNumber[nCntRank][nCntDigit]->Uninit();
				delete m_apScoreNumber[nCntRank][nCntDigit];
				m_apScoreNumber[nCntRank][nCntDigit] = NULL;
			}
		}
	}

	// ���̃X�R�A�̕`��
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit]->Uninit();
		delete m_pNowScore[nCntDigit];
		m_pNowScore[nCntDigit] = NULL;
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CRanking::Update(void)
{
	// �J�E���g��i�߂�
	m_nCntFlash++;
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// �X�R�A�ƃ����L���O�̔�r
			if (m_nNowScore != 0 && m_nNowScore == m_nRanking[nCntRank])
			{
				// �_�ł�����
				if (m_nCntFlash % (FLASH_COUNT * 2) == 0)
				{// ON
					D3DXCOLOR col = m_apScoreNumber[nCntRank][nCntDigit]->GetColor();
					col.a = 0.4f;
					m_apScoreNumber[nCntRank][nCntDigit]->SetColor(col);
				}
				else if (m_nCntFlash % FLASH_COUNT == 0)
				{// OFF
					D3DXCOLOR col = m_apScoreNumber[nCntRank][nCntDigit]->GetColor();
					col.a = 1.0f;
					m_apScoreNumber[nCntRank][nCntDigit]->SetColor(col);
				}
			}

			// �X�V����
			m_apScoreNumber[nCntRank][nCntDigit]->Update();
			// �e�����̌v�Z
			m_apScoreNumber[nCntRank][nCntDigit]->SetNumber((m_nRanking[nCntRank] % (int)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_SCORE_DIGIT - nCntDigit - 1))));
		}

	}
}

//==================================
// �`�揈��
//==================================
void CRanking::Draw(void)
{
	// �����L���O�`��
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// ����
		m_apRankNumber[nCntRank]->Draw();

		// �X�R�A
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			m_apScoreNumber[nCntRank][nCntDigit]->Draw();
		}
	}

	// ���̃X�R�A�̕`��
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit]->Draw();
	}
}
