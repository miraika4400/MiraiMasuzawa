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
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "course.h"
#include "player.h"
#include "light.h"
#include "item.h"
#include "rank.h"
#include "file.h"
#include "accelfloor.h"
#include "sound.h"
#include "renderer.h"

//=============================
// �}�N����`
//=============================
#define FILE_NAME "data/Texts/Item.txt"

//=============================
// �}�N����`
//=============================
#define RESULT_COUNT 200

//=============================
// �ÓI�����o�ϐ��錾
//=============================
CCamera *CGame::m_pCamera[MAX_PLAYER_NUM] = {};   // �J����
CPlayer *CGame::m_pPlayer[MAX_PLAYER_NUM] = {};   // �v���C���[
CLight *CGame::m_pLight = NULL;		// ���C�g�N���X�̃|�C���^�ϐ�
CCourse *CGame::m_pCourse = NULL;   // �R�[�X�N���X�̃|�C���^�ϐ�
int CGame::m_nNumPlayer = 1;        // �v���C���[�l��
bool CGame::m_bResult = false;      // ���U���g

//=============================
// �R���X�g���N�^
//=============================
CGame::CGame()
{
	// �ϐ��̃N���A
	m_nCntResult = 0;
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
CGame * CGame::Create()
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
	
	// �}�b�v�̐���
	m_pCourse = CCourse::Create(CCourse::COURSE_EASY);

	float fPosX = (((300 * 2)*m_nNumPlayer) / 2) - 300;// X���ʒu
	float fPosZ = 300.0f*m_nNumPlayer;
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		// �v���C���[�̐���
		m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), nCntPlayer);
		// �J�����̐���
		m_pCamera[nCntPlayer] = CCamera::Create(nCntPlayer);
		// X�������炷
		fPosX -= 300.0f * 2.0f;
		// Y�������炷
		fPosZ -= 200.0f;
	}
	//���C�g�N���X�̐���
	m_pLight = new CLight;
	// ������
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}
	
	// �ϐ��̏�����
	m_bResult = false;
	m_nCntResult = 0;
    // �A�C�e���z�u
    CFile::Create()->Read(FILE_NAME);
	if (m_nNumPlayer == 3)
	{
		LPDIRECT3DTEXTURE9 pTexture;        // �e�N�X�`���ւ̃|�C���^
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/Textures/NoEntry.png", &pTexture);   // ��l
		CScene2d * pScene2d = CScene2d::Create();
		// ���W�̐ݒ�
		pScene2d->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 2, 0.0f));
		// �T�C�Y�̐ݒ�
		pScene2d->SetSize(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f));
		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pScene2d->SetPriority(OBJTYPE_UI);
		// �e�N�X�`���̃Z�b�g
		pScene2d->BindTexture(pTexture);
	}
	
	return S_OK;
}

//=============================
// �I������
//=============================
void CGame::Uninit(void)
{
	// �J����
	for (int nCntCamera = 0; nCntCamera < m_nNumPlayer; nCntCamera++)
	{
		if (m_pCamera[nCntCamera] != NULL)
		{
			m_pCamera[nCntCamera]->Uninit();
			delete m_pCamera[nCntCamera];
			m_pCamera[nCntCamera] = NULL;
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

    CFile::GetInstance()->Uninit();
}


//=============================
// �X�V����
//=============================
void CGame::Update(void)
{
	// �A�C�e����]
	CItem::ItemRotasion();

	// �J�����N���X�̃A�b�v�f�[�g
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		if (m_pCamera[nCntPlayer] != NULL)
		{
			m_pCamera[nCntPlayer]->Update();
		}
	}
	// ���ʂ̊Ǘ�
	CRank::RankManage();

	if (!m_bResult)
	{
		bool bGoal = true;
		// �v���C���[�S�����S�[����ԉ��̊m�F
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (m_pPlayer[nCnt] != NULL)
			{
				if (!m_pPlayer[nCnt]->GetGoalFlag())
				{
					bGoal = false;
					break;
				}
			}
		}

		if (bGoal)
		{
			// �J�E���g��i�߂�
			m_nCntResult++;
			// �����ẴJ�E���g�ɒB������
			if (m_nCntResult >= RESULT_COUNT)
			{
				// �J�E���g�̏�����
				m_nCntResult = 0;
				// ���U���g��Ԃɂ���
				m_bResult = true;
				// �Q�[��BGM��~
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_GAME);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);
			}
		}
	}
	else
	{// ���U���g��Ԃ̎�
		if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
		{
			// �^�C�g���ɑJ��
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
}


//=============================
// �`�揈��
//=============================
void CGame::Draw(void)
{
}