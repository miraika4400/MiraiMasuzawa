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
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "number.h"
#include "score.h" 
#include "gauge.h"
#include "effect.h"
#include "mouse.h"
#include "cursor.h"
#include "stage.h"
#include "camera.h"
#include "minimap.h"
#include "boss.h"
#include "fade.h"
#include "item.h"
#include "bomb.h"
#include "bomb_ui.h"
#include "manacircle.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "manager.h"
#include "renderer.h"

//=============================
// �}�N����`
//=============================
#define DEFFICULT_UI_POS  D3DXVECTOR3(150.0f,50.0f,0.0f)             // ��ՓxUI�ʒu
#define DEFFICULT_UI_SIZE D3DXVECTOR3(120.0f,40.0f ,0.0f)             // ��ՓxUI�T�C�Y
#define DEFFICULT_UI_COL_NORMAL D3DXCOLOR(0.8f,0.8f ,0.0f,1.0f)      // ��ՓxUI�J���[*�m�[�}��
#define DEFFICULT_UI_COL_HARD   D3DXCOLOR(0.8f,0.0f ,0.0f,1.0f)      // ��ՓxUI�J���[*�n�[�h

//=============================
// �ÓI�����o�ϐ��錾
//=============================
CPlayer *CGame::m_pPlayer = NULL;   // �v���C���[
CCursor *CGame::m_pCursor = NULL;   // �J�[�\��
CCamera *CGame::m_pCamera = NULL;   // �J����
CStage  *CGame::m_pStage  = NULL;   // �X�e�[�W
CBoss   *CGame::m_pBoss   = NULL;   // �{�X

CGame::GAME_WAVES CGame::m_wave = CGame::WAVE_NORMAL;                // Wave��
CGame::GAME_DEFFICULT CGame::m_defficult = CGame::DEFFICULT_NORMAL;  // ��Փx

//=============================
// �R���X�g���N�^
//=============================
CGame::CGame()
{
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
	// �^�C���N���X�̐���
	CTime::Create();
	
	// �w�i����
	CBg::Create();
	
	// �X�e�[�W�̐���
	m_pStage = CStage::Create();

	// �v���C���[����
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0, -400.0f, 0.0f));
	
	// �J�[�\������
	m_pCursor = CCursor::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

	// Wave�̏�����
	m_wave = WAVE_NORMAL;
	SetWave(m_wave);

	// �J�����̐���
	m_pCamera = CCamera::Create();

	// �X�R�A����
	CScore::Create();
	
	// �~�j�}�b�v�̐���
	CMiniMap::Create();
	
	// �{��UI�̐���
	CBombUi::Create();

	// �T�[�N���̐��̏�����
	CManaCircle::SetCircleNum(0);

	// �|�[�Y�̏�����
	CManager::SetActivePause(false);

	// ��Փx�̓ǂݍ���
	CEnemy::LoadDefficult();

	// ��ՓxUI�̐���
	
	// �|���S���̃|�C���^
	CScene2d*pScene2d = NULL;
	// �|���S���̐���
	pScene2d = CScene2d::Create();
	// NULL�`�F�b�N
	if (pScene2d != NULL)
	{
		// �T�C�Y�̐ݒ�
		pScene2d->SetSize(DEFFICULT_UI_SIZE);
		// ���W�̐ݒ�
		pScene2d->SetPos(DEFFICULT_UI_POS);
		// �v���C�I���e�B�̐ݒ�
		pScene2d->SetPriority(OBJTYPE_UI);
		// �e�N�X�`��
		LPDIRECT3DTEXTURE9 pTex = NULL;
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		// ��Փx�ɂ���ēǂݍ��ރe�N�X�`���E�F��ς���
		if (m_defficult == DEFFICULT_NORMAL)
		{// �m�[�}��
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Normal.png", &pTex);
			// �F�̐ݒ�
			pScene2d->SetColor(DEFFICULT_UI_COL_NORMAL);
		}
		else
		{// �n�[�h
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Hard.png", &pTex);
			// �F�̐ݒ�
			pScene2d->SetColor(DEFFICULT_UI_COL_HARD);
		}
		// �e�N�X�`���̊��蓖��
		pScene2d->BindTexture(pTex);
	}
	
	return S_OK;
}

//=============================
// �I������
//=============================
void CGame::Uninit(void)
{
	// �J������
	CCamera::Release();
	// ��Փx�̓ǂݍ���
	CEnemy::UnloadDefficult();
	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CGame::Update(void)
{
	// �T�[�N�����̊Ǘ�
	CManaCircle::SpawnManager();

	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}
	
	if (m_wave == WAVE_NORMAL)
	{
		CEnemy::SpawnManager();
	}

#ifdef _DEBUG
	// �f�o�b�N�R�}���h
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SetWave(WAVE_BOSS);
	}
#endif // _DEBUG

}


//=============================
// �`�揈��
//=============================
void CGame::Draw(void)
{
	CGame::GetCamera()->SetCamera();
}

//=============================
// WAVE�Z�b�g
//=============================
void CGame::SetWave(GAME_WAVES wave)
{
	m_wave = wave;

	if (m_wave == WAVE_BOSS)
	{
		// �{�X�̐���
		m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//CGauge::Create(CGauge::GAUGE_BOSS_HP);
	}
}
