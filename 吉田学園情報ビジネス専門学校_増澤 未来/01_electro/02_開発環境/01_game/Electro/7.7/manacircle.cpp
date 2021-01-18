////////////////////////////////////////////////////
//
//    bullet�N���X�̏���[bullet.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "manacircle.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "effect.h"
#include "game.h"
#include "boss.h"
#include "stage.h"
#include "bomb.h"
#include "particle.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MANACIRCRE_TEXTURE_PATH "./data/Textures/circle3.png" // �e�N�X�`���̃p�X
#define CIRCLE_SIZE D3DXVECTOR3(140.0f,140.0f,0.0f)          // �T�C�Y
#define RECOVERYCOUNT 50                                     // �T�[�N���������J�E���g
#define CIRCLE_NUM_BOSS_WAVE 6                               // �{�X�펞�̃T�[�N����
#define CIRCLE_RESPAWN_COUNT_NORMAL 150                      // �{�X�펞�̃T�[�N���̃��X�|�[������
#define CIRCLE_RESPAWN_COUNT_HARD   200                      // �{�X�펞�̃T�[�N���̃��X�|�[������
#define CIRCLE_ANIM_SPEED 10      // �A�j���[�V�������x
#define CIRCLE_MAX_ANIMATION_X 16 // �A�j���[�V������ ��
#define CIRCLE_MAX_ANIMATION_Y 1 // �A�j���[�V������ �c
#define CIRCLE_DISTANCE_BOSSWAVE 400 // �{�X�펞�̃T�[�N���̒��S����̋���
#define CIRCLE_BOMB_SIZE         300 // �T�[�N���̔����̃T�C�Y
#define MP_RECOVERY_S 10.0f          // �T�[�N����MP�񕜗ʁ���
#define MP_RECOVERY_M 15.0f          // ����
// �T�[�N���ɓ����Ă���Ԃ�MP�̉񕜗�
#define REGENE_MP_NORMAL 0.4f  // �m�[�}��
#define REGENE_MP_HARD 0.25f   // �n�[�h

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9 CManaCircle::m_pTexture = NULL; // �e�N�X�`���|�C���^
//std::list<CManaCircle*> CManaCircle::m_circleList; // �T�[�N�����X�g
int CManaCircle::m_nNumCircle = 0;
int CManaCircle::m_nMaxCircle = 0;
CManaCircle *CManaCircle::m_apCircle[CIRCLE_MAX] = {};

//******************************
// �R���X�g���N�^
//******************************
CManaCircle::CManaCircle() :CScene3d(OBJTYPE_MANACIRCLE)
{
	// �ϐ��̃N���A
	m_nCntRecovery = 0;
	bInCircle = false;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	// �T�[�N���̐��̃J�E���g
	m_nNumCircle++;

	// �T�[�N���̍ő吔�����[�v
	for (int nCntCircle = 0; nCntCircle < CIRCLE_MAX; nCntCircle++)
	{
		if (m_apCircle[nCntCircle] == NULL)
		{
			m_apCircle[nCntCircle] = this;
			m_nIndex = nCntCircle;
			break;
		}
	}
}

//******************************
// �f�X�g���N�^
//******************************
CManaCircle::~CManaCircle()
{
}

//******************************
// �N���G�C�g
//******************************
CManaCircle * CManaCircle::Create(const D3DXVECTOR3 pos)
{
	// �������̊m��
	CManaCircle *pMCircle;
	pMCircle = new CManaCircle;
	// ������
	pMCircle->Init();

	// �e�l�̑���E�Z�b�g
	pMCircle->SetPos(pos);                    // ���W
	pMCircle->SetPriority(OBJTYPE_MANACIRCLE); // �I�u�W�F�N�g�^�C�v

	// �T�[�N�����X�g�ɒǉ�
	//m_circleList.push_back(pMCircle);
	return pMCircle;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CManaCircle::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, MANACIRCRE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CManaCircle::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// �����̊Ǘ�
//******************************
void CManaCircle::SpawnManager(void)
{

	// ���X�|�[�����̃J�E���g
	static int nCntReSpawn[CIRCLE_NUM_BOSS_WAVE] = {};

	if (CGame::GetWave() == CGame::WAVE_NORMAL)
	{// �ʏ�WAVE
		// �ÓI�ϐ��̏�����
		for (int nCntCircle = 0; nCntCircle < CIRCLE_NUM_BOSS_WAVE; nCntCircle++)
		{
			if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL)
			{// �m�[�}��
				nCntReSpawn[nCntCircle] = CIRCLE_RESPAWN_COUNT_NORMAL;
			}
			else
			{// �n�[�h
				nCntReSpawn[nCntCircle] = CIRCLE_RESPAWN_COUNT_HARD;
			}
		}

		// �T�[�N�����ő吔�ɂ���
		if (m_nNumCircle < m_nMaxCircle)
		{
			// �����_���ȍ��W�̎擾
			D3DXVECTOR3 randPos = GetRandCirclePos();

			// �T�[�N������
			CManaCircle::Create(randPos);
		}
	}
	else
	{//BOSSWAVE

		// �X�e�[�W�̍��W�̎擾
		D3DXVECTOR3 stagePos = STAGE_POS;

		// �T�[�N���̍ő吔
		for (int nCntCircle = 0; nCntCircle < CIRCLE_NUM_BOSS_WAVE; nCntCircle++)
		{
			// CIRCLE_NUM_BOSS_WAVE�����̈ʒu�̎擾
			D3DXVECTOR3 pos;
			pos.x = stagePos.x + cosf(nCntCircle*D3DXToRadian(360 / CIRCLE_NUM_BOSS_WAVE)) * CIRCLE_DISTANCE_BOSSWAVE;
			pos.y = stagePos.y + sinf(nCntCircle*D3DXToRadian(360 / CIRCLE_NUM_BOSS_WAVE)) * CIRCLE_DISTANCE_BOSSWAVE;
			pos.z = 0.0f;

			if (m_apCircle[nCntCircle] == NULL)
			{
				// �J�E���g��i�߂�
				nCntReSpawn[nCntCircle]++;
				if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL && nCntReSpawn[nCntCircle] >= CIRCLE_RESPAWN_COUNT_NORMAL||
					CGame::GetDefficult() == CGame::DEFFICULT_HARD   && nCntReSpawn[nCntCircle] >= CIRCLE_RESPAWN_COUNT_HARD)
				{
					// �J�E���g�̏�����
					nCntReSpawn[nCntCircle] = 0;
					// �T�[�N���̐���
					m_apCircle[nCntCircle] = CManaCircle::Create(pos);

					// �z�񂪑O�l�߂Ő��������̂Œ���
					if (nCntCircle != m_apCircle[nCntCircle]->m_nIndex)
					{
						m_apCircle[m_apCircle[nCntCircle]->m_nIndex] = NULL;
						m_apCircle[nCntCircle]->m_nIndex = nCntCircle;
					}
				}

			}
			else
			{
				// �J�E���g�̏�����
				nCntReSpawn[nCntCircle] = 0;

				// ���W�̐ݒ�
				if (m_apCircle[nCntCircle]->GetPos() != pos)
				{
					m_apCircle[nCntCircle]->Init();
					m_apCircle[nCntCircle]->SetPos(pos);
				}
			}
		}
	}
}


//******************************
// ����������
//******************************
HRESULT CManaCircle::Init(void)
{
	// ������
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);
	// �T�C�Y
	SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
	bInCircle = false;

	// UV���W�̐ݒ�

	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / CIRCLE_MAX_ANIMATION_X;
	float fv = 1.0f / CIRCLE_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	// �F�̐ݒ�
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	SetColor(col);

	return S_OK;
}

//******************************
// �I������
//******************************
void CManaCircle::Uninit(void)
{
	// NULL
	m_apCircle[m_nIndex] = NULL;

	// �T�[�N���̐������炷
	m_nNumCircle--;

	// �I������
	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CManaCircle::Update(void)
{

	// �A�j���[�V����
	AnimationManager();

	// �T�[�N���̃T�C�Y
	D3DXVECTOR3 size = GetSize();

	size += (CIRCLE_SIZE - size)*0.05f;

	SetSize(size);

	// �����蔻��
	CollisionCircle();
}

//******************************
// �`�揈��
//******************************
void CManaCircle::Draw(void)
{
	CScene3d::Draw();
}

//******************************************
// �T�[�N����u�������_���ȍ��W�̎擾
//******************************************
D3DXVECTOR3 CManaCircle::GetRandCirclePos(void)
{
	// �X�e�[�W���W�̎擾
	D3DXVECTOR3 stagePos = STAGE_POS;

	// �����_���Ȋp�x
	int nRandAngle = rand() % 360;
	// �����_���ȋ���
	float fRandDistance = fmodf((float)rand(), STAGE_SIZE.x / 2 - CIRCLE_SIZE.x) - (STAGE_SIZE.x / 2 - CIRCLE_SIZE.x);
	// �����_���ȍ��W
	D3DXVECTOR3 randPos = stagePos + D3DXVECTOR3(cosf(D3DXToRadian(nRandAngle))*fRandDistance, sinf(D3DXToRadian(nRandAngle))*fRandDistance, 0);

	// �ق��̃T�[�N���Əd�Ȃ��ĂȂ���
	for (int nCntCircle = 0; nCntCircle < CIRCLE_MAX; nCntCircle++)
	{
		if (m_apCircle[nCntCircle] != NULL)
		{
			// �T�[�N���̍��W
			D3DXVECTOR3 circlePos = m_apCircle[nCntCircle]->GetPos();
			// �T�[�N���̃T�C�Y
			D3DXVECTOR3 circleSize = m_apCircle[nCntCircle]->GetSize();

			while (powf(randPos.x - circlePos.x, 2) + powf(randPos.y - circlePos.y, 2) <= powf(CIRCLE_SIZE.x + circleSize.x, 2))
			{
				// �����_���ȍ��W
				randPos = GetRandCirclePos();

				nCntCircle = 0;
			}
		}
	}

	return randPos;
}


//******************************
// �A�j���[�V�����̊Ǘ�
//******************************
void CManaCircle::AnimationManager(void)
{
	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	if (m_nCntAnim % CIRCLE_ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX++;

		if (m_nAnimX >= CIRCLE_MAX_ANIMATION_X)
		{
			// �A�j���[�V����X���̏�����
			m_nAnimX = 0;
		}
		else
		{
			// UV���W�̐ݒ�
			D3DXVECTOR2 uv[NUM_VERTEX];

			float fu = 1.0f / CIRCLE_MAX_ANIMATION_X;
			float fv = 1.0f / CIRCLE_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY - fv);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			// UV���W�Z�b�g
			SetTextureUV(uv);
		}


	}
}

//******************************
// �����蔻��
//******************************
void CManaCircle::CollisionCircle(void)
{
	// �T�[�N���̍��W
	D3DXVECTOR3 pos = GetPos();
	// �T�[�N���̃T�C�Y
	D3DXVECTOR3 size = GetSize();
	// SE�Đ��̃`�F�b�N
	static bool bSE = false;
	// �v���C���[�Ƃ̓����蔻��

	// �v���C���[�������Ă���Ƃ�
	if (CPlayer::IsAlive())
	{
		// �v���C���[�̍��W
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
		// �v���C���[�̃T�C�Y
		D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();

		// �����蔻��
		if (powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2) <= powf(size.x / 2 + playerSize.x / 2, 2))
		{
			if (!bSE)
			{
				bSE = true;
				// SE�Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_CIRCLE);
			}
			
			// �����Ă���Ƃ�a�l���グ��
			D3DXCOLOR col = GetColor();
			col.a += (1.0f - col.a)*0.1f;
			SetColor(col);

			// �T�[�N���ɓ�����
			bInCircle = true;

			// MP�̉�
			if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL)
			{// �m�[�}��
				CGame::GetPlayer()->RecoveryMp(REGENE_MP_NORMAL);
			}
			else
			{// �n�[�h
				CGame::GetPlayer()->RecoveryMp(REGENE_MP_HARD);
			}

			m_nCntRecovery++;

			// �p�[�e�B�N���̐���
			// ���������邩�����_��
			int nRandNum = rand() % 5 + 3;//3�`8��
			for (int nCntPartcle = 0; nCntPartcle < nRandNum; nCntPartcle++)
			{
				// �����_���ȋ���
				int nRandDistance = rand() % 30 + 10;// 10�`40
													 // �����_���Ȋp�x
				int nRandAngle = rand() % 360;//0�`360
											  // ���̏��ō��W�̐���
				D3DXVECTOR3 randPos;
				randPos.x = cosf(D3DXToRadian(nRandAngle))*nRandDistance;
				randPos.y = sinf(D3DXToRadian(nRandAngle))*nRandDistance;
				randPos.z = 0;
				randPos += playerPos;

				// �����_���ȃT�C�Y
				int nRandSize = rand() % 10 + 5;//5�`15

												// �ړ��ʂ��v���C���[�����ɐݒ�
				D3DXVECTOR3 move = playerPos - randPos;
				move.z = 0;
				D3DXVec3Normalize(&move, &move);
				// �p�[�e�B�N���̐���
				CParticle::Create(randPos,
					move*(float)(rand() % 100 + 1) / 50,
					D3DXVECTOR3((float)nRandSize,
					(float)nRandSize, 0.0f),
					30,
					D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f, 1.0f));

			}

			// �T�[�N���ɓ����Ă�������E
			if (m_nCntRecovery >= RECOVERYCOUNT)
			{

				// MP�̉�
				CGame::GetPlayer()->RecoveryMp(MP_RECOVERY_M);
				// �{���̐���
				CBomb::Create(pos, CIRCLE_BOMB_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				LostCircle();
				// SE���Đ���Ԃɂ���
				bSE = false;
				return;
			}
		}
		else
		{
			m_nCntRecovery = 0;
			if (bInCircle)
			{
				// MP�̉�
				CGame::GetPlayer()->RecoveryMp(MP_RECOVERY_S);
				// �{���̐���
				CBomb::Create(pos, CIRCLE_BOMB_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				LostCircle();

				// SE���Đ���Ԃɂ���
				bSE = false;
				return;
			}
		}
	}

	// �G�l�~�[�Ƃ̓����蔻�� 

	// �G�l�~�[���X�g�̎擾
	std::list<CEnemy*>::iterator iteretorEnemy;
	for (iteretorEnemy = CEnemy::GetEnemylist()->begin(); iteretorEnemy != CEnemy::GetEnemylist()->end(); iteretorEnemy++)
	{
		// �G�l�~�[�̍��W
		D3DXVECTOR3 enemyPos = (*iteretorEnemy)->GetPos();
		// �G�l�~�[�̃T�C�Y
		D3DXVECTOR3 enemySize = (*iteretorEnemy)->GetSize();

		// �����蔻��
		if (powf(pos.x - enemyPos.x, 2) + powf(pos.y - enemyPos.y, 2) <= powf(size.x / 2 + enemySize.x / 2, 2))
		{
			//	�T�[�N������G�̊p�x
			float fAngle = atan2f(enemyPos.y - pos.y, enemyPos.x - pos.x);
			enemyPos.x = pos.x + cosf(fAngle)*(size.x / 2 + enemySize.x / 2);
			enemyPos.y = pos.y + sinf(fAngle)*(size.x / 2 + enemySize.x / 2);
			(*iteretorEnemy)->SetPos(enemyPos);

		}
	}
}

//******************************
// �T�[�N���̏�������
//******************************
void CManaCircle::LostCircle(void)
{
	// SE�̒�~
	CManager::GetSound()->Stop(CSound::LABEL_SE_CIRCLE);
	
	if (CGame::GetWave() == CGame::WAVE_NORMAL)
	{
		// ������
		Init(); 
		// �ʒu�������_���ňړ�������
		SetPos(GetRandCirclePos());
	}
	else
	{
		Uninit();
	}
}
