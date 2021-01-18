
//===================================================
//
//    �v���C���[�N���X�̏���[player.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "bullet.h"
#include "sound.h"
#include "mouse.h"
#include "cursor.h"
#include "camera.h"
#include "stage.h"
#include "game.h"
#include "fade.h"
#include "bomb.h"
#include "enemy.h"
#include "bulletex.h"
#include "effect.h"
#include "particle.h"
#include "boss.h"
#include "gauge.h"
#include "fever_logo.h"
#include "fever_screen.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define PLAYER_SPEED 5.0f         // �ړ����x
#define PLAYER_SIZE 50.0f         // �T�C�Y
#define PLAYER_BULLET_SPEED 10    // �A�j���[�V�������x
#define PLAYER_ATTACK_COUNT 8     // �U���̃C���^�[�o��
#define PLAYER_STATE_COUNT 10     // �X�e�[�g�ω����̃J�E���g
#define PLAYER_GAMEOVER_COUNT 120 // �Q�[���I�[�o�[����ʑJ�ڂ܂ł̃J�E���g
#define PLAYER_MOVE_RATE 0.1f     // �ړ��̊����̌W��
#define PLAYER_MAX_REMAIN 3       // �c�@�̍ő吔
#define PLAYER_BULLET_MP 2        // �ꔭ�̏���MP
#define SLOW_TIME 40              // ���S���̃X���[���o�̒���
#define DEATH_PARTICLE_NUM 50     // ���S���̃p�[�e�B�N����
#define PLAYER_TEXTURE_PATH "./data/Textures/player001.png" // �e�N�X�`���̃p�X

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
bool CPlayer::m_bAlive = false;

//==================================
// �R���X�g���N�^
//==================================
CPlayer::CPlayer() :CScene3d(OBJTYPE_PLAYER)
{
	// �N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_nStateCount = 0;
	m_state = STATE_SPAWN;
	m_bBomb = false;
	m_nRemain = PLAYER_MAX_REMAIN;
	m_fMp = PLAYER_MAX_MP;
	m_fLife = PLAYER_MAX_HP;
	m_fRotAngle = 0;
	m_fRotAngleDist = 0;
	memset(&m_pGauge, 0, sizeof(m_pGauge));
	m_fFeverGauge = 0.0f;
	m_bFever = false;
	m_nBulletInterval = PLAYER_ATTACK_COUNT;
	m_pFeverLogo = NULL;
	m_pFeverScreen = NULL;
}

//==================================
// �f�X�g���N�^
//==================================
CPlayer::~CPlayer()
{
}

//==================================
// �N���G�C�g
//==================================
CPlayer * CPlayer:: Create(const D3DXVECTOR3 pos)
{
	//�� �����̊m��
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	
	// ������
	pPlayer->Init();

	// ���W�̐ݒ�
	pPlayer->SetPos(pos);
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pPlayer->SetPriority(OBJTYPE_PLAYER);

	return pPlayer;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CPlayer::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CPlayer::Unload(void)
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
HRESULT CPlayer::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	
	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// �T�C�Y�̐ݒ�
	SetSize(D3DXVECTOR3(PLAYER_SIZE, PLAYER_SIZE, 0.0f));

	// �����o�ϐ�������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_nStateCount = 0;
	m_state = STATE_SPAWN;
	m_bAlive = true;
	m_fMp   = PLAYER_MAX_MP;
	m_fLife = PLAYER_MAX_HP;

	// �p�x�̏�����
	m_fRotAngle = 90;
	m_fRotAngleDist = 90;
	SetAngle(90);

	// �Q�[�W����
	// �̗�
	m_pGauge[0] =CGauge::Create(&m_fLife, D3DXVECTOR3(PLAYER_LIFE_POS_X, PLAYER_LIFE_POS_Y, 0.0f), PLAYER_LIFE_WIDTH, PLAYER_LIFE_HEGHT,
		PLAYER_MAX_HP, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
	// MP
	m_pGauge[1] = CGauge::Create(&m_fMp, D3DXVECTOR3(PLAYER_MP_POS_X, PLAYER_MP_POS_Y, 0.0f), PLAYER_MP_WIDTH, PLAYER_MP_HEGHT,
		PLAYER_MAX_MP, D3DXCOLOR(0.0f, 0.75f, 1.0f, 1.0f));
	// �t�B�[�o�[
	m_pGauge[2] = CGauge::Create(&m_fFeverGauge, D3DXVECTOR3(FEVER_POS_X, FEVER_POS_Y, 0.0f), FEVER_WIDTH, FEVER_HEGHT,
		IN_FEVER_NUM, D3DXCOLOR(1.0f, 0.8f, 0.2f, 1.0f));

	return S_OK;
}

//==================================
// �I������
//==================================
void CPlayer::Uninit(void)
{
	CScene3d::Uninit();

	// �Q�[�W������
	for (int nCntGauge = 0; nCntGauge < PLAYER_GAUGE_NUM; nCntGauge++)
	{
		if (m_pGauge[nCntGauge] != NULL)
		{
			m_pGauge[nCntGauge]->Uninit();
			m_pGauge[nCntGauge] = NULL;
		}
	}

	// �����t���O��false�ɂ���
	m_bAlive = false;

	// �o�b�N�o�b�t�@�J���[�̏�����
	CManager::GetRenderer()->SetBuffColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================
// �X�V����
//==================================
void CPlayer::Update(void)
{
	//DirectinManager();
	if (m_bAlive)
	{// �����Ă�Ƃ�
		// �R���g���[���̊Ǘ�
		ControlManager();

		// ���W�̎擾
		D3DXVECTOR3 pos = GetPos();

		// �ړ��ʂ𑫂�
		pos += m_move;

		SetPos(pos);

		// �X�e�[�W���W�̎擾
		D3DXVECTOR3 stagePos = STAGE_POS;
		// �����̌v�Z
		float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

		// �X�e�[�W�[�̐ݒ�
		if (fDistanceStage + PLAYER_SIZE / 2 > STAGE_SIZE.x / 2)
		{
			float fAngle = atan2f(pos.y - stagePos.y, pos.x - stagePos.x);

			pos = D3DXVECTOR3(cosf(fAngle)*(STAGE_SIZE.x / 2 - PLAYER_SIZE / 2), sinf(fAngle)*(STAGE_SIZE.x / 2 - PLAYER_SIZE / 2), 0.0f);

			//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (CBoss::IsAlive())
		{// �{�X�������Ă���Ƃ�
			

			// �{�X���W�̎擾
			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();
			// �{�X�T�C�Y�̎擾
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize();
			// �����̌v�Z
			float fDistanceBoss = sqrtf(powf(pos.x - bossPos.x, 2) + powf(pos.y - bossPos.y, 2));

			// �{�X�Ƃ̓����蔻��
			if (fDistanceBoss - PLAYER_SIZE / 2 < bossSize.x / 2)
			{
				// �_���[�W����
				if (m_state == STATE_NORMAL)
				{
					HitAttack(1);
				}

				// �{�X�ɖ��܂�Ȃ��悤��
				float fAngle = atan2f(pos.y - bossPos.y, pos.x - bossPos.x);
				pos = D3DXVECTOR3(cosf(fAngle)*(bossSize.x / 2 + PLAYER_SIZE / 2), sinf(fAngle)*(bossSize.x / 2 + PLAYER_SIZE / 2), 0.0f);
			}
		}

		// MP�̎��R��
		RecoveryMp(0.025f);

		// ���W�̃Z�b�g
		SetPos(pos);
	}
	// �X�e�[�g�̊Ǘ�
	StateManager();
	// �t�B�[�o�[�̏���
	FeverManager();
}

//==================================
// �`�揈��
//==================================
void CPlayer::Draw(void)
{
	if (m_bAlive)
	{
		CScene3d::Draw();
	}
}

//==================================
// �e�q�b�g��
//==================================
void CPlayer::HitAttack(int nDamage)
{
	// ���C�t�����炷
	m_fLife -= nDamage;

	if (m_fLife > 0)
	{// ���C�t���c���Ă���
		m_state = STATE_DAMAGE;
	}
	else
	{// ���C�t��0�ȉ�

		// ���C�t��0�ȉ��ɂȂ�Ȃ��悤��
		m_fLife = 0;
		
		// ���S��Ԃ�
		m_bAlive = false;
		// �Q�[���I�[�o�[�X�e�[�g
		m_state = STATE_GAMEOVER;
	
		CManager::GetSound()->Play(CSound::LABEL_SE_DEATH);
	}
}

//==================================
// HP�񕜏���
//==================================
void CPlayer::RecoveryLife(const int nRecovery)
{
	// ���C�t�Ɉ����𑫂�
	m_fLife += nRecovery;

	// ���C�t���ő�l�𒴂��Ȃ��悤��
	if (m_fLife > PLAYER_MAX_HP)
	{
		m_fLife = PLAYER_MAX_HP;
	}
}

//==================================
// MP�񕜏���
//==================================
void CPlayer::RecoveryMp(float fRecoveryMp)
{
	// MP�Ɉ����𑫂�
	m_fMp += fRecoveryMp;
	// MP���ő�l�𒴂��Ȃ��悤��
	if (m_fMp > PLAYER_MAX_MP)
	{
		m_fMp = PLAYER_MAX_MP;
	}
}

void CPlayer::ProgressFever(float fAdd)
{
	if (!m_bFever)
	{// �t�B�[�o�[��Ԃ���Ȃ��Ƃ�

		// �t�B�[�o�[�l�̉��Z
		m_fFeverGauge+= fAdd;

		// ���l�ɒB������t�B�[�o�[�̓���
		if (m_fFeverGauge >= IN_FEVER_NUM)
		{
			m_bFever = true;
			// BGM�̐؂�ւ�
			CManager::GetSound()->Stop(CSound::LABEL_BGM_GAME);
			CManager::GetSound()->Play(CSound::LABEL_BGM_FEVER);
		}
	}
}

//==================================
// �X�e�[�g�Ǘ�
//==================================
void CPlayer::StateManager(void)
{
	// �X�e�[�g�ŕ���
	switch (m_state)
	{
	case STATE_NORMAL:
		// �m�[�}���X�e�[�g

		// �J�E���g�̏�����
		m_nStateCount = 0;
		break;

	case STATE_DAMAGE:
		// �_���[�W�X�e�[�g

		// �J�E���g��i�߂�
		m_nStateCount++;

		// ���J�E���g�i�񂾂�ʏ�X�e�[�g�ɖ߂�
		if (m_nStateCount >= PLAYER_STATE_COUNT * 6)
		{
			m_nStateCount = 0;
			m_state = STATE_NORMAL;
		}

		// �Ԃ��_�ł�����

		if (m_nStateCount % (PLAYER_STATE_COUNT * 2) == 0)
		{// �ʏ�J���[
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (m_nStateCount % PLAYER_STATE_COUNT == 0)
		{// �Ԃ�����
			SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
		}

		break;

	case STATE_SPAWN:
		// �X�|�[���X�e�[�g
		
		// �J�E���g��i�߂�
		m_nStateCount++;

		// ���J�E���g�i�񂾂�ʏ�X�e�[�g�ɖ߂�
		if (m_nStateCount >= PLAYER_STATE_COUNT * 12)
		{
			m_nStateCount = 0;
			m_state = STATE_NORMAL;
		}
		
		// �_�ł�����

		if (m_nStateCount % (PLAYER_STATE_COUNT * 2) == 0)
		{// �ʏ�J���[
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (m_nStateCount % PLAYER_STATE_COUNT == 0)
		{// ������
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		break;

	case STATE_GAMEOVER:
		// �Q�[���I�[�o�[
		m_nStateCount++;
		if (m_nStateCount <= 50)
		{
			// �X���[���o
			Sleep(SLOW_TIME);

			int nRadSize = rand() % 20 + 20;
			// �p�[�e�B�N������
			CParticle::Create(GetPos(),
				D3DXVECTOR3(rand() % 10 - 5, rand() % 10 - 5, 0.0f),
				D3DXVECTOR3(nRadSize, nRadSize, 0.0f),
				SLOW_TIME + 20,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, 1.0f, (float)(rand() % 100) / 100.0f, 0.8f),
				CParticle::PARTICLE_CIRCLE);
			/*(CParticle::PARTICLE_TYPE)(rand() % CParticle::PARTICLE_MAX)*/

		}
		if (m_nStateCount > PLAYER_GAMEOVER_COUNT)
		{
			// �V�[�� �J��
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			
		}
		break;
	default:
		break;
	}
}


//==================================
// �L�����R���g���[���̊Ǘ�
//==================================
void CPlayer::ControlManager(void)
{
	//�ړ��ʂ̖ڕW�l
	D3DXVECTOR3 moveDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �X�e�B�b�N�̎擾
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W) || js.lY <= -600)
	{// ���ړ�
		m_fRotAngleDist = 90.0f;
		moveDest.y = PLAYER_SPEED;


		if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
		{
			m_fRotAngleDist = 135.0f;

			moveDest.y = sinf(45) * PLAYER_SPEED;
			moveDest.x = cosf(45) * -PLAYER_SPEED;
		}
		if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
		{
			m_fRotAngleDist = 45.0f;

			moveDest.y = sinf(45) * PLAYER_SPEED;
			moveDest.x = cosf(45) * PLAYER_SPEED;
		}
	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_S) || js.lY >= 600)
	{// ���ړ�

		m_fRotAngleDist = -90.0f;

		moveDest.y = -PLAYER_SPEED;

		if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
		{
			m_fRotAngleDist = -135.0f;

			moveDest.y = sinf(45) * -PLAYER_SPEED;
			moveDest.x = cosf(45) * -PLAYER_SPEED;
		}
		if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
		{
			m_fRotAngleDist = -45.0f;

			moveDest.y = sinf(45) * -PLAYER_SPEED;
			moveDest.x = cosf(45) * PLAYER_SPEED;
		}
	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
	{// ���ړ�
		m_fRotAngleDist = 180.0f;
		moveDest.x = -PLAYER_SPEED;

	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
	{// ���ړ�
		m_fRotAngleDist = 0.0f;
		moveDest.x = PLAYER_SPEED;
	}

	// ��]�������Ȃ��悤�ɍ��𒲐�
	while (m_fRotAngleDist - m_fRotAngle > 180.0f)
	{
		m_fRotAngleDist -= 360.0f;
	}
	while (m_fRotAngleDist - m_fRotAngle < -180.0f)
	{
		m_fRotAngleDist += 360.0f;
	}

	m_fRotAngle += (m_fRotAngleDist - m_fRotAngle)*0.1f;
	SetAngle((float)m_fRotAngle);
	//m_bAnim = true;

	D3DXVECTOR3 cursorPos = CGame::GetCursor()->GetPos();

	// �J�[�\���̊p�x
	float fCoursorAngle = atan2f(SCREEN_HEIGHT / 2 - cursorPos.y, cursorPos.x - SCREEN_WIDTH / 2);

	// ����
	m_move += (moveDest - m_move) * PLAYER_MOVE_RATE;

	if (CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(7, 0))
	{
		m_nCntBullet = PLAYER_ATTACK_COUNT - 1;
	}
	// ���N���b�N�Œe�̔���
	if (CManager::GetMouse()->GetMousePress(0))
	{
		// �e�̃C���^�[�o���̃J�E���g
		m_nCntBullet++;
		if (m_nCntBullet > m_nBulletInterval)
		{
			// MP�̊m�F
			if (m_fMp >= PLAYER_BULLET_MP)
			{
				// MP�����炷
				m_fMp -= PLAYER_BULLET_MP;
				if (m_fMp <= 0)
				{
					m_fMp = 0;
				}

				D3DXVECTOR3 cursorPos = CGame::GetCursor()->GetPos();

				// �e�̔��ˊp�x
				float fBulletAngle = atan2f(SCREEN_HEIGHT / 2 - cursorPos.y, cursorPos.x - SCREEN_WIDTH / 2);
				if (!m_bFever)
				{// �ʏ���
					// �e�̐���
					CBullet::Create(
						GetPos(),
						D3DXVECTOR3(cosf(fBulletAngle)*PLAYER_BULLET_SPEED, sinf(fBulletAngle)*PLAYER_BULLET_SPEED, 0.0f),
						60,
						CBullet::BULLETUSER_PLAYER,
						D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
				}
				else
				{// �t�B�[�o�[���
				 // �e�̐���
					CBullet::Create(
						GetPos(),
						D3DXVECTOR3(cosf(fBulletAngle)*PLAYER_BULLET_SPEED*1.5f, sinf(fBulletAngle)*PLAYER_BULLET_SPEED*1.5f, 0.0f),
						60,
						CBullet::BULLETUSER_PLAYER,
						D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
				}

				// SE�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);

			}
			// �e�̃C���^�[�o���̏�����
			m_nCntBullet = 0;

		}
	}
	// �W���C�X�e�B�b�N�Œe�̔���

	if (js.lRz >= 10 || js.lRz <= -10 || js.lZ >= 10 || js.lZ <= -10)
	{
		// �e�̃C���^�[�o���̃J�E���g
		m_nCntBullet++;
		if (m_nCntBullet > PLAYER_ATTACK_COUNT)
		{
			// MP�̊m�F
			if (m_fMp >= PLAYER_BULLET_MP)
			{
				// MP�����炷
				m_fMp -= PLAYER_BULLET_MP;
				// MP���}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
				if (m_fMp < 0)
				{
					m_fMp = 0;
				}

				// �e�����������X�e�B�b�N�̕����ɂ���
				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(45)*js.lZ, -(sinf(45)*js.lRz), 0.0f);
				// ���K��
				D3DXVec3Normalize(&bulletMove, &bulletMove);

				// �e�̐���
				CBullet::Create(
					GetPos(),
					bulletMove*PLAYER_BULLET_SPEED,
					60,
					CBullet::BULLETUSER_PLAYER,
					D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));


				// SE�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);

			}
			// �e�̃C���^�[�o���̏�����
			m_nCntBullet = 0;

		}
	}

	// �{�����g�p�ł��邩
	if (m_bBomb)
	{
		if (CManager::GetMouse()->GetMouseTrigger(1) || CManager::GetJoypad()->GetJoystickTrigger(6, 0))
		{
			// �{���̐���
			CBomb::Create(GetPos(),500.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// �{�����g�p�s��Ԃɂ���
			m_bBomb = false;
		}
	}
}


//==================================
// �t�B�[�o�[�̏���
//==================================
void CPlayer::FeverManager(void)
{

	if (m_bFever)
	{// �t�B�[�o�[��
		m_nBulletInterval = PLAYER_ATTACK_COUNT / 2;
		m_fMp = PLAYER_MAX_MP;

		if (m_pFeverScreen == NULL)
		{// �t�B�[�o�[��ʂ̐���
			m_pFeverScreen = CFeverScreen::Create();
		}
		if (m_pFeverLogo == NULL)
		{// �t�B�[�o�[���S�̐���
			m_pFeverLogo = CFeverLogo::Create();
		}

		// �t�B�[�o�[�l�����炷
		m_fFeverGauge -= 0.1f;
		if (m_fFeverGauge < 0)
		{// �l��0�ȉ��̎��t�B�[�o�[�̏I��
			m_bFever = false;
			m_fFeverGauge = 0;
			// BGM�̐؂�ւ�
			CManager::GetSound()->Stop(CSound::LABEL_BGM_FEVER);
			CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);
		}

		// �o�b�N�o�b�t�@�J���[�̐ݒ�
		CManager::GetRenderer()->SetBuffColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	}
	else
	{
		if (m_pFeverScreen != NULL)
		{// �t�B�[�o�[��ʂ�����
			m_pFeverScreen->Uninit();
			m_pFeverScreen = NULL;
		}
		if (m_pFeverLogo != NULL)
		{// �t�B�[�o�[���S������
			m_pFeverLogo->Uninit();
			m_pFeverLogo = NULL;
		}

		m_nBulletInterval = PLAYER_ATTACK_COUNT;
		// �o�b�N�o�b�t�@�J���[�̐ݒ�
		CManager::GetRenderer()->SetBuffColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
