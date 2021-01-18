////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "chain_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "polygon.h"

//**********************************
// �}�N����`
//**********************************
#define TEXTURE_PATH "./data/Textures/Chain.png"     // �e�N�X�`���̃p�X
#define NUBER_SIZE   D3DXVECTOR3( 15.0f*1.5f, 30.0f*1.5f, 0.0f)     // �����T�C�Y*�摜�̉𑜓x�ɍ��킹��
#define STRING_SIZE  D3DXVECTOR3( 500.0f/10.0f, 220.0f/10.0f, 0.0f) // "chain"�����T�C�Y*�摜�̉𑜓x�ɍ��킹��

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( 0                   + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3((SCREEN_WIDTH/2)     + 90.0f , 0                 + 85.0f , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3( 0                   + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3((SCREEN_WIDTH/2)     + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // �v���C���[4

// ���U���g���̃T�C�Y
#define RESULT_NUMBER_SIZE D3DXVECTOR3( 15.0f*2.5f, 30.0f*2.5f, 0.0f)     // �����T�C�Y*�摜�̉𑜓x�ɍ��킹��

// ���U���g���̈ʒu
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/2                   +60, 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // �v���C���[3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // �v���C���[4

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CChainUi::m_pTexture = NULL;

//=============================
// �R���X�g���N�^
//=============================
CChainUi::CChainUi() :CScene(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	memset(&m_apNumber, 0, sizeof(m_apNumber)); // �`�F�C������
	m_nPlayerNum = 0;                         // �v���C���[�ԍ�
	m_nDegit = 1;                             // �\������
	m_pos = VEC3_ZERO;                        // ���S���W
	m_nChain = 0;                             // �`�F�C����
	m_pPolygon = NULL;                        // �����摜
	m_resultPos = VEC3_ZERO;                  // ���U���g���̃|�X
	m_resultSize = VEC3_ZERO;                 // ���U���g���̃T�C�Y
}

//=============================
// �f�X�g���N�^
//=============================
CChainUi::~CChainUi()
{
}

//=============================
// �N���G�C�g
//=============================
CChainUi * CChainUi::Create(int nPlayerNum)
{
	// �������̊m��
	CChainUi *pChainUi = new CChainUi;
	// �v���C���[�ԍ��̑��
	pChainUi->m_nPlayerNum = nPlayerNum;
	// ������
	pChainUi->Init();
	return pChainUi;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CChainUi::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CChainUi::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// ����������
//=============================
HRESULT CChainUi::Init(void)
{
	// ���W�̐ݒ�

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();
	// �v���C���[�ԍ��ɂ���ĕ���
	switch (m_nPlayerNum)
	{
	case 0:
		// �v���C���[1

		if (nNumPlayer == 1)
		{// �����Ȃ�
			m_pos = (POS_PLAYER1_1);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if(nNumPlayer == 2)
		{// �񕪊�
			m_pos = (POS_PLAYER1_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// �l����
			m_pos = (POS_PLAYER1_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			m_pos = (POS_PLAYER2_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// �l����
			m_pos = (POS_PLAYER2_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// �v���C���[3
		m_pos = (POS_PLAYER3);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// �v���C���[4
		m_pos = (POS_PLAYER4);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}



	float fPosX = m_pos.x + (((NUBER_SIZE.x * 2)*m_nDegit) / 2) - NUBER_SIZE.x;// X���ʒu
																			   // �i���o�[�N���X�̐���
	for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(0, D3DXVECTOR3(fPosX, m_pos.y, 0.0f), NUBER_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// X�������炷
		fPosX -= NUBER_SIZE.x * 2;
	}
	
	m_pPolygon = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y + NUBER_SIZE.y+5, 0.0f), STRING_SIZE);
	m_pPolygon->SetTexture(m_pTexture);
	return S_OK;
}

//=============================
// �I������
//=============================
void CChainUi::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
			delete m_apNumber[nCnt];
			m_apNumber[nCnt] = NULL;
		}
	}

	if (m_pPolygon != NULL)
	{
		m_pPolygon->Uninit();
		delete m_pPolygon;
		m_pPolygon = NULL;
	}
	Release();
}


//=============================
// �X�V����
//=============================
void CChainUi::Update(void)
{
	if (!CGame::GetResultFlag())
	{// ���U���g��Ԃ���Ȃ��Ƃ�
		m_nChain = CGame::GetPlayer(m_nPlayerNum)->GetChainNum();
		int nIndex = MAX_CHAIN_DIGIT - 1;
		for (int nCntDigit = 0; nCntDigit < MAX_CHAIN_DIGIT; nCntDigit++)
		{

			m_apNumber[nIndex]->Update();

			m_apNumber[nIndex]->SetNumber((m_nChain % (int)(powf(10.0f, (MAX_CHAIN_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_CHAIN_DIGIT - nCntDigit - 1))));
			nIndex--;
		}

		// ����
		if (m_nChain >= 100)
		{
			m_nDegit = MAX_CHAIN_DIGIT;
		}
		else if (m_nChain >= 10)
		{
			m_nDegit = 2;
		}
		else if (m_nChain >= 1)
		{
			m_nDegit = 1;
		}
		else
		{
			m_nDegit = 0;
		}

		float fPosX = m_pos.x + (((NUBER_SIZE.x * 2)*m_nDegit) / 2) - NUBER_SIZE.x;// X���ʒu
																						  // �i���o�[�N���X�̐���
		for (int nCnt = 0; nCnt < m_nDegit; nCnt++)
		{
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3(fPosX, m_pos.y, 0.0f));
			// X�������炷
			fPosX -= NUBER_SIZE.x * 2;
		}
	}
	else
	{// ���U���g��

		// �ő�`�F�C������\��
		m_nChain = CGame::GetPlayer(m_nPlayerNum)->GetMaxChain();
		int nIndex = MAX_CHAIN_DIGIT - 1;
		for (int nCntDigit = 0; nCntDigit < MAX_CHAIN_DIGIT; nCntDigit++)
		{

			m_apNumber[nIndex]->Update();

			m_apNumber[nIndex]->SetNumber((m_nChain % (int)(powf(10.0f, (MAX_CHAIN_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_CHAIN_DIGIT - nCntDigit - 1))));
			nIndex--;
		}
		m_pos = m_resultPos;

		float fPosX = m_pos.x + (((RESULT_NUMBER_SIZE.x * 2)*MAX_CHAIN_DIGIT) / 2) - RESULT_NUMBER_SIZE.x;// X���ʒu
																										  // �i���o�[�N���X�̐���
		for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
		{
			m_apNumber[nCnt] ->SetPos(D3DXVECTOR3(fPosX, m_pos.y, 0.0f));
			m_apNumber[nCnt]->SetSize(RESULT_NUMBER_SIZE);
			// X�������炷
			fPosX -= RESULT_NUMBER_SIZE.x * 2;
		}

		m_pPolygon->SetPos(D3DXVECTOR3(m_resultPos.x+ RESULT_NUMBER_SIZE.x * 4, m_resultPos.y + 30, 0.0f));
	}
}


//=============================
// �`�揈��
//=============================
void CChainUi::Draw(void)
{
	int nLoop = 0;
	if (!CGame::GetResultFlag())
	{
		nLoop = m_nDegit;
	}
	else
	{
		nLoop = MAX_CHAIN_DIGIT;
	}
	// ���[�v���ƌ��������킹��
	for (int nCnt = 0; nCnt < nLoop; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			if (m_pPolygon != NULL&&nCnt == 0)
			{
				// chain������̕`��
				m_pPolygon->Draw();
			}
			// chain�����̕`��
			m_apNumber[nCnt]->Draw();
		}
	}
}