//===================================================
//
//    �G�l�~�[�N���X�̏���[enemy.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "renderer.h"
#include "bullet.h"
#include "bulletex.h"
#include "player.h"
#include "explosion.h"
#include "mouse.h"
#include "game.h"
#include "stage.h"
#include "score.h"
#include "fade.h"
#include "enemy.h"
#include "sound.h"
#include "particle.h"
#include "gauge.h"

//*****************************
// �}�N����`
//*****************************
#define BOSS_SIZE 200.0f            // �G�l�~�[�̃T�C�Y
#define BOSS_STATE_COUNT 10         // �X�e�[�g�J�ڗp
#define BOSS_STATE_COUNT_DEATH 100  // �X�e�[�g�J�ڗp
#define BOSS_ATTACK_INTERVAL 60     // �U���̃C���^�[�o��
#define BOSS_ATTACK_INTERVAL2 20    // �U���̃C���^�[�o��
#define BOSS_BULLET_SPEED 4         // �{�X�̒e�̑��x
#define BOSS_ANIM_SPEED 14          // �A�j���[�V�������x
#define BOSS_MAX_ANIMATION_X 3      // �A�j���[�V������ ��
#define BOSS_MAX_ANIMATION_Y 1      // �A�j���[�V������ �c
#define BOSS_ATTACK_PATARN 3        // �U���p�^�[����
#define BOSS_ATTACK_ON_COUNT 300    // �U����Ԃֈڍs����J�E���g
#define BOSS_ATTACK_OFF_COUNT 300   // ��U����Ԃֈڍs����J�E���g
#define BOSS_TEXTURE_PATH "./data/Textures/balor000.png"//�e�N�X�`���̃p�X

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture  = NULL;
bool CBoss::m_bIsAlive = false;

//==================================
// �R���X�g���N�^
//==================================
CBoss::CBoss():CScene3d(OBJTYPE_ENEMY)
{
	// �����o�ϐ��̃N���A
	m_fBulletAngle = 0.0f;
	m_state = STATE_NORMAL;
	m_nStateCount = 0;
	m_fLife = BOSS_MAX_LIFE;
	m_nCntBullet = 0;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_bAttack = false;
	m_nRandAttack = 0;
	m_pGauge = NULL;
}

//==================================
// �f�X�g���N�^
//==================================
CBoss::~CBoss()
{
}

//==================================
// �N���G�C�g
//==================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos)
{
	// �������̊m��
	CBoss *pBoss = new CBoss;

	// ������
	pBoss->Init();
	// ���W�̐ݒ�
	pBoss->SetPos(pos);

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pBoss->SetPriority(OBJTYPE_ENEMY);

	return pBoss;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CBoss::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CBoss::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}

//==================================
// ����������
//==================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// �����t���O
	m_bIsAlive = true;
	
	// �U���p�^�[���������_����
	m_nRandAttack = rand() % BOSS_ATTACK_PATARN;

	// �e�N�X�`��UV���W�̏�����
	m_nAnimX = 2;
	m_nAnimY = 0;

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / BOSS_MAX_ANIMATION_X;
	float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);
	// �G�l�~�[���̃J�E���g
	CEnemy::PlusEnemy();
	// �x��SE�̒�~
	CManager::GetSound()->Stop(CSound::LABEL_SE_WARNING);
	// �̗�
	m_pGauge = CGauge::Create(&m_fLife, D3DXVECTOR3(BOSS_HP_POS_X, BOSS_HP_POS_Y, 0.0f), BOSS_HP_WIDTH, BOSS_HP_HEGHT,
		BOSS_MAX_LIFE, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);
	return S_OK;
}

//==================================
// �I������
//==================================
void CBoss::Uninit(void)
{
	m_bIsAlive = false;
	// �G�l�~�[���̃J�E���g�����炷
	CEnemy::MinusEnemy();

	// �Q�[�W������
	if (m_pGauge != NULL)
	{
		m_pGauge->Uninit();
		m_pGauge = NULL;
	}

	CScene3d::Uninit();
}

//==================================
// �X�V����
//==================================
void CBoss::Update(void)
{
	// �����Ă�Ƃ�
	if (m_bIsAlive)
	{
		// �A�^�b�N�̏���
		AttackManager();
	}
	// �X�e�[�g�̊Ǘ�
	switch (m_state)
	{
	case STATE_NORMAL:
		// �ʏ펞�̃J���[�ݒ�
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case STATE_DAMAGE:
		// �_���[�W���̃J���[�ݒ�
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		m_nStateCount--;
		if (m_nStateCount <= 0)
		{
			m_state = STATE_NORMAL;
		}

		break;
	case STATE_DEATH:

		m_bIsAlive = false;
		m_nStateCount--;
		if (m_nStateCount <= 0)
		{
			Uninit();
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
		break;
	default:
		break;
	}

}

//==================================
// �`�揈��
//==================================
void CBoss::Draw(void)
{
	// �����Ă�Ƃ�
	if (m_bIsAlive)
	{
		CScene3d::Draw();
	}
}

//==================================
// �U�����q�b�g��������
//==================================
void CBoss::HitAttack(int nDamage)
{
	m_fLife -= nDamage;


	if (m_fLife <= 0.0f)
	{// ���C�t��0�ȉ�
	    
		// ���C�t���}�C�i�X�ɂȂ�Ȃ��悤��
		m_fLife = 0.0f;
	    // �����̐���
		CExplosion::Create(GetPos(), D3DXVECTOR3(2200, 2200, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f))->SetAddMode(true);

		// �p�[�e�B�N��200����
		for (int nCntPart = 0; nCntPart < 100; nCntPart++)
		{
			// �����_���ȃT�C�Y
			int nRadSize = rand() % 20 + 70;
			// �����_���Ȋp�x
			float fRandAngle = D3DXToRadian(rand() % 360);
			// �����_���ȃX�s�[�h
			float fRandSpeed = (float)(rand() % 600 + 700) / 100.0f;
			// �p�[�e�B�N������
			CParticle::Create(GetPos(),
				D3DXVECTOR3(cosf(fRandAngle)*fRandSpeed, sinf(fRandAngle)*fRandSpeed, 0.0f),
				D3DXVECTOR3(nRadSize, nRadSize, 0.0f),
				rand() % 30 + 60,
				D3DXCOLOR(((float)(rand() % 100) / 100.0f), ((float)(rand() % 100) / 100.0f) , ((float)(rand() % 100) / 100.0f) , 0.8f),
				CParticle::PARTICLE_STAR);
		}

		// �X�R�A���Z
		CScore::AddScore(1000);
		// �X�e�[�g�����S��Ԃ�
		m_state = STATE_DEATH;
		// �J�E���g�Z�b�g
		m_nStateCount = BOSS_STATE_COUNT_DEATH;
		return;
	}
	else
	{// ���C�t���c���Ă���
		m_state = STATE_DAMAGE;
		m_nStateCount = BOSS_STATE_COUNT;
	}
}

//==================================
// �U���̊Ǘ�
//==================================
void CBoss::AttackManager(void)
{
	// �J�E���g���₷
	m_nCntBullet++;
	
	if (m_bAttack)
	{	// �U����Ԃ̎�

		// �U���p�^�[���̕���
		switch (m_nRandAttack)
		{
		case 0:

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL2 == 0)
			{
				if (CPlayer::IsAlive())
				{
					// �����̍��W
					D3DXVECTOR3 pos = GetPos();
					// �v���C���[���W�̎擾
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

					// 8way�ɒe�̐���
					for (int nCntAngle = 0; nCntAngle < 8; nCntAngle++)
					{
						float fAngle = nCntAngle*D3DXToRadian(180 / 8);

						CBullet::Create(GetPos(),
							D3DXVECTOR3(cosf(m_fBulletAngle + fAngle)*BOSS_BULLET_SPEED, sinf(m_fBulletAngle + fAngle)*BOSS_BULLET_SPEED, 0.0f),
							200,
							CBullet::BULLETUSER_ENEMY,
							D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					}
					// �e���o���p�x�𑝂₷
					m_fBulletAngle += 2.0f;
				}
			}

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL == 0)
			{
				if (CPlayer::IsAlive())
				{
					// �����̍��W
					D3DXVECTOR3 pos = GetPos();
					// �v���C���[���W�̎擾
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
					// �e�̊p�x��ݒ�
					m_fBulletAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

					// �e�����̒e��
					const int nNumBullet = 4;
					// ���^�ɒe�������
					float fStarAngle = 0;
					D3DXVECTOR3 startPos = pos - D3DXVECTOR3(cosf(D3DXToRadian(90)) * 50, sinf(D3DXToRadian(90)) * 50, 0.0f);
				
					float fDistance = sqrtf(powf(startPos.x + (startPos.x + cosf(D3DXToRadian(D3DXToRadian(360 / nNumBullet) * 2)) * 100), 2) -
						powf(startPos.x + (startPos.x + sinf(D3DXToRadian(D3DXToRadian(360 / nNumBullet) * 2)) * 100), 2));

					for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++)
					{

						for (int nCntAngle2 = 0; nCntAngle2 < nNumBullet; nCntAngle2++)
						{
							//
							// �e�̐���
							CBulletEx::Create_Rotation(startPos, pos, D3DXVECTOR3(cosf(m_fBulletAngle) * 6, sinf(m_fBulletAngle) * 6, 0.0f),
								D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f), 120, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));

							// �p�x���v���X����
							startPos += D3DXVECTOR3(cosf(fStarAngle) * fDistance / nNumBullet, sinf(fStarAngle) * fDistance / nNumBullet, 0.0f);
						}
						// �v���X����p�x�̐ݒ�
						fStarAngle += D3DXToRadian(360 / 5) * 2;
					}
				}
			}
			break;

		case 1:

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL == 0)
			{
				const int nNumBullet = 8;
				// �������ɒe�������Ă��玩�@�_��
				for (int nCntAngle = 0; nCntAngle < nNumBullet; nCntAngle++)
				{
					CBulletEx::Create_Target(GetPos(),
						D3DXVECTOR3(cosf(D3DXToRadian(360 / nNumBullet)*nCntAngle) * BOSS_BULLET_SPEED, sinf(D3DXToRadian(360 / nNumBullet)*nCntAngle) * BOSS_BULLET_SPEED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f),
						80,
						CBullet::BULLETUSER_ENEMY,
						D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
				}

			}

			// ���˂��邽�тɔ��ˊp�x��0.2f�����₷�e��4�����ɏo��
			if (m_nCntBullet % BOSS_ATTACK_INTERVAL2 == 0)
			{
				// �ړ���
				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(m_fBulletAngle) * BOSS_BULLET_SPEED, sinf(m_fBulletAngle) * BOSS_BULLET_SPEED, 0.0f);
				// �e�̐���
				CBullet::Create(GetPos(), bulletMove, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				CBullet::Create(GetPos(), bulletMove*-1, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				// �ړ���
				bulletMove = D3DXVECTOR3(cosf(-m_fBulletAngle) * BOSS_BULLET_SPEED, sinf(-m_fBulletAngle) * BOSS_BULLET_SPEED, 0.0f);
				// �e�̐���
				CBullet::Create(GetPos(), bulletMove, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				CBullet::Create(GetPos(), bulletMove*-1, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				// ���ˊp�x�𑝂₷
				m_fBulletAngle += 0.2f;
			}
			break;

		case 2:

			
			if (rand() % 15 == 0)
			{// ���@�_���̒e

				// �����̍��W
				D3DXVECTOR3 pos = GetPos();
				// �v���C���[���W�̎擾
				D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
				// �e�̊p�x��ݒ�
				m_fBulletAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
				m_fBulletAngle += D3DXToRadian(rand() % 20 - 10);
				// �e�̃X�s�[�h
				float fSpeed = BOSS_BULLET_SPEED * 1.3f;

				// �e�̐���
				CBullet::Create(GetPos(),                                                                              // ���W
					D3DXVECTOR3(cosf(m_fBulletAngle)*fSpeed, sinf(m_fBulletAngle)*fSpeed, 0.0f),                       // �ړ���
					450,                                                                                               // ����
					CBullet::BULLETUSER_ENEMY,                                                                         // �e�̎�����
					D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f))->                                                               // �F
				SetSize(D3DXVECTOR3(BULLET_SIZE*1.5f, BULLET_SIZE*1.5f, 0.0f));                                          // �T�C�Y

			}

			if (m_nCntBullet % (BOSS_ATTACK_INTERVAL2 / 2) == 0)
			{

				const int nNumBullet = 6;
				for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
				{
					// �e�̊p�x��ݒ�
					m_fBulletAngle = D3DXToRadian((360 / nNumBullet)*nCntBullet);
					m_fBulletAngle += D3DXToRadian((360 / nNumBullet) / 2);
					// �e�̃X�s�[�h
					float fSpeed = BOSS_BULLET_SPEED * 1.5f;

					// �e�̐���
					CBullet::Create(GetPos(),                                                        // ���W
						D3DXVECTOR3(cosf(m_fBulletAngle)*fSpeed, sinf(m_fBulletAngle)*fSpeed, 0.0f), // �ړ���
						150,                                                                         // ����
						CBullet::BULLETUSER_ENEMY,                                                   // �e�̎�����
						D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));                                          // �F

				}
			}
			break;

		default:
			break;
		}

		if (m_nCntBullet >= BOSS_ATTACK_OFF_COUNT)
		{
			// �U����Ԃ�����
			m_bAttack = false;
			// �J�E���g�̏�����
			m_nCntBullet = 0;

			m_nAnimX = 2;

			// UV���W�̐ݒ�
			D3DXVECTOR2 uv[NUM_VERTEX];
			float fu = 1.0f / BOSS_MAX_ANIMATION_X;
			float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV���W�Z�b�g
			SetTextureUV(uv);
		}


	}
	else
	{// ��U����Ԏ�

		if (m_nCntBullet >= BOSS_ATTACK_ON_COUNT)
		{
			// �U����Ԃɂ���
			m_bAttack = true;
			// �J�E���g�̏�����
			m_nCntBullet = 0;

			// �U���p�^�[���������_����
			m_nRandAttack = rand() % BOSS_ATTACK_PATARN;

			m_nAnimX = 1;

			// UV���W�̐ݒ�
			D3DXVECTOR2 uv[NUM_VERTEX];
			float fu = 1.0f / BOSS_MAX_ANIMATION_X;
			float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV���W�Z�b�g
			SetTextureUV(uv);
		}
	}
}
