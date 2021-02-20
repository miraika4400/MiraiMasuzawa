////////////////////////////////////////////////////
//
//    �Q�[���N���X�̏���[game.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//=============================
// �C���N���[�h
//=============================
#include "game.h"
#include "bg.h"
#include "number.h"
#include "score.h" 
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "player.h"
#include "course.h"
#include "checkpoint.h"
#include "character.h"
#include "cpu_point.h"
#include "cpu.h"
#include "rank.h"
#include "item_point.h"
#include "item.h"

//=============================
// �ÓI�����o�ϐ��錾
//=============================
CCamera *CGame::m_apCamera[MAX_PLAYER_NUM] = {};           // �J�����N���X�|�C���^
CLight  *CGame::m_pLight = NULL;                           // ���C�g�N���X�|�C���^
CCharacter * CGame::m_apCharacter[MAX_CHARACTER_NUM] = {}; // �L�����N�^�[�N���X�|�C���^
int CGame::m_nNumCaracter = 0;                             // ���������L�����N�^�[�̐�
int CGame::m_nNumPlayer = 1;                               // �v���C���[��
CCpuPoint *CGame::m_pCpuPoint = NULL;                      // CPU�p�R�[�X�f�[�^
CCheckPoint *CGame::m_pCheckPoint = NULL;                  // �`�F�b�N�|�C���g
CItemPoint *CGame::m_pItemPoint = NULL;                    // �A�C�e���|�C���g
CCourse    *CGame::m_pCourse = NULL;                       // �R�[�X�|�C���^

//=============================
// �R���X�g���N�^
//=============================
CGame::CGame()
{
	// �ϐ��̃N���A
	m_nNumCaracter = 0;
}

//=============================
// �f�X�g���N�^
//=============================
CGame::~CGame()
{
}

//=============================
// �N���G�C�g
//=============================
CGame * CGame::Create(void)
{
	// �������̊m��
	CGame *pGame = new CGame;
	// ������
	pGame->Init();
	return pGame;
}

//=============================
// ����������
//=============================
HRESULT CGame::Init(void)
{

	// �|�[�Y�̏�����
	CManager::SetActivePause(false);

	// ���C�g�N���X�̐���
	m_pLight = new CLight;
	// ���C�g�N���X�̏�����
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}

	// �w�i�̐���
	CBg::Create();

	// ���ʊǗ��N���X�̐���
	CRank::Create();

	// �R�[�X����
	m_pCourse = CCourse::Create();

	// �`�F�b�N�|�C���g�̐���
	m_pCheckPoint = CCheckPoint::Create();

	// CPU�p�̃R�[�X�f�[�^
	m_pCpuPoint = CCpuPoint::Create();
	
	// �A�C�e�����W
	m_pItemPoint = CItemPoint::Create();

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		// �J�����N���X�̐���
		m_apCamera[nCnt] = CCamera::Create(nCnt);
		// �v���C���[����
		m_apCharacter[m_nNumCaracter] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), 0);
		// �L�����N�^�[�����Z
		m_nNumCaracter++;	
	}
	
	// CPU����
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(-150.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_RUIN, 5);
	// �L�����N�^�[�����Z
	m_nNumCaracter++;

	// CPU����
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_STAR , 4);
	// �L�����N�^�[�����Z
	m_nNumCaracter++;
	
	// CPU����
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(-150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_RUIN, 2);
	// �L�����N�^�[�����Z
	m_nNumCaracter++;

	// CPU����
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(150.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_STAR, 10);
	// �L�����N�^�[�����Z
	m_nNumCaracter++;



	return S_OK;
}

//=============================
// �I������
//=============================
void CGame::Uninit(void)
{
	// �J�����N���X�̉������
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Uninit();
		}
	}

	// ���C�g
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CGame::Update(void)
{
#ifdef _DEBUG
	// �f�o�b�O�p��ʑJ�ڃR�}���h
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

#endif // _DEBUG
	CItem::RotationItem();

	// �J�����N���X�̍X�V����
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Update();
		}
	}
}


//=============================
// �`�揈��
//=============================
void CGame::Draw(void)
{
	// �J�����̃Z�b�g
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}