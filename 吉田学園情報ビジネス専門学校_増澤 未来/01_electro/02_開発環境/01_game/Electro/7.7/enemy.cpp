//===================================================
//
//    �G�l�~�[�N���X�̏���[enemy.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "bulletex.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "stage.h"
#include "score.h"
#include "boss.h"
#include "item.h"
#include "time.h"
#include "particle.h"
#include "manacircle.h"
#include "spawneffect.h"

//*****************************
// �}�N����`
//*****************************
#define ENEMY_SPEED 2.5f         // �G�l�~�[�̃X�s�[�h
#define MOVE_COUNT 100           // �ړ������ύX���̃J�E���g
#define ENEMY_SIZE 40.0f         // �G�l�~�[�̃T�C�Y
#define ENEMY_DEFAULT_LIFE 1     // �f�t�H���g�̃��C�t
#define ENEMY_STATE_COUNT 10     // �X�e�[�g�J�ڗp
#define ENEMY_SPAWN_COUNT 200 
#define ENEMY_SPAWN_DISTANCE 200  // �X�|�[������Ƃ��̋����̍Œ�
#define MOVE_CHANGE_DISTANCE 100

//�e�N�X�`���̃p�X
#define ENEMY_TEXTURE_PATH_YELLOW "./data/Textures/enemy000.png" // ���F�G�l�~�[
#define ENEMY_TEXTURE_PATH_PURPLE "./data/Textures/enemy001.png" // ���F�G�l�~�[
#define ENEMY_TEXTURE_PATH_RED    "./data/Textures/enemy002.png" // �ԐF�G�l�~�[

// ��Փx�ʂ̓ǂݍ��݃f�[�^
#define DIFFECULT_PATH_NORMAL "./data/Texts/Enemy_Manage_NORMAL.txt" // �m�[�}��
#define DIFFECULT_PATH_HARD   "./data/Texts/Enemy_Manage_HARD.txt"   // �n�[�h

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_MAX] = {};
std::list<CEnemy*> CEnemy::m_enemyList; // �G�l�~�[���X�g
int CEnemy::m_nCntSpawn = 0;            // �X�|�[���J�E���g
int CEnemy::m_nNumEnemy = 0;            // �G�l�~�[�A�{�X�̐�
//int *CEnemy::m_pMaxEnemy   = NULL;      // �G�l�~�[�̍ő吔
//int *CEnemy::m_pSpawnCount = NULL;      // �G�l�~�[�̗N���p�x
//int *CEnemy::m_pPhaseTime = NULL;       // �t�F�[�Y��؂�ւ��鎞��
CEnemy::ENEMY_SPAWN * CEnemy::m_pSpawn = NULL;// �G�l�~�[�̂킫�Ǘ��p
int CEnemy::m_nMaxPhaseNum = 0;            // �t�F�[�Y�ő吔
int CEnemy::m_nPhaseNum = 0;               // �t�F�[�Y��

//==================================
// �R���X�g���N�^
//==================================
CEnemy::CEnemy() :CScene3d(OBJTYPE_ENEMY)
{
	// �����o�ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_fMoveAngle = 0.0f;
	m_nLife = ENEMY_DEFAULT_LIFE;
	m_state = STATE_NORMAL;
	m_nStateCount = 0;
	m_nCntEncount = 0;
	m_nCntMove = 0;
	m_bPlusAngle = false;
}

//==================================
// �f�X�g���N�^
//==================================
CEnemy::~CEnemy()
{
}

//==================================
// �N���G�C�g
//==================================
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const ENEMYTYPE type)
{
	// �������̊m��
	CEnemy *pEnemy = new CEnemy;
	// ��ނ̐ݒ�
	pEnemy->m_type = type;
	// ������
	pEnemy->Init();
	// ���W�̐ݒ�
	pEnemy->SetPos(pos);

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pEnemy->SetPriority(OBJTYPE_ENEMY);

	// �G�l�~�[���X�g�ɒǉ�
	m_enemyList.push_back(pEnemy);

	return pEnemy;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_YELLOW, &m_pTexture[ENEMYTYPE_YELLOW]);// ���F�G�l�~�[
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_PURPLE, &m_pTexture[ENEMYTYPE_PURPLE]);// ���G�l�~�[
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_RED, &m_pTexture[ENEMYTYPE_RED]);      // �ԃG�l�~�[

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CEnemy::Unload(void)
{
	// �e�N�X�`���̉������
	for (int nCntTex = 0; nCntTex < sizeof(m_pTexture) / sizeof(LPDIRECT3DTEXTURE9); nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// ��Փx�ʂ̓G�̗N���f�[�^�̓ǂݍ���
//==================================
void CEnemy::LoadDefficult(void)
{
	FILE*pFile = NULL;

	// �t�@�C���I�[�v��
	if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL)
	{// �m�[�}��
		pFile = fopen(DIFFECULT_PATH_NORMAL, "r");
	}
	else
	{// �n�[�h
		pFile = fopen(DIFFECULT_PATH_HARD, "r");
	}

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �t�F�[�Y���̓ǂݍ���
		fscanf(pFile, "%*s%*s%d", &m_nMaxPhaseNum);
		// �������̊m��
		m_pSpawn = new ENEMY_SPAWN[m_nMaxPhaseNum];

		// �t�F�[�Y�������[�v
		for (int nCnt = 0; nCnt < m_nMaxPhaseNum; nCnt++)
		{
			int nIndex;
			// �z��
			fscanf(pFile, "%*s%*s%d", &nIndex);
			// ����
			fscanf(pFile, "%*s%*s%d", &m_pSpawn[nIndex].nPhaseTime);
			// �ő吔
			fscanf(pFile, "%*s%*s%d", &m_pSpawn[nIndex].nMaxEnemy);
			// �p�x
			fscanf(pFile, "%*s%*s%d", &m_pSpawn[nIndex].nSpawnCount);
			// �T�[�N����
			fscanf(pFile, "%*s%*s%d", &m_pSpawn[nIndex].nCircleNum);
		}
		fclose(pFile);
	}
	
}

//==================================
// ��Փx�ʂ̓G�̗N���f�[�^�̃A�����[�h
//==================================
void CEnemy::UnloadDefficult(void)
{
	if (m_pSpawn != NULL)
	{
		// �������̊m��
		delete[] m_pSpawn;
		m_pSpawn = NULL;
	}
	// �t�F�[�Y���̃��Z�b�g
	m_nPhaseNum = 0;
}

//==================================
// �X�|�[���̊Ǘ�
//==================================
void CEnemy::SpawnManager(void)
{
	int nMaxEnemy = 10;
	int nSpawnCount = 200;

	// ���Ԃɂ���ē�Փx����
	if (CTime::GetTime() / 1000 >= BOSS_SPAWN_SEC)
	{// �{�X�t�F�[�Y
		// �T�[�N���̍ő吔
		CManaCircle::SetCircleMax(6);
		// �{�X�̐���
		CGame::SetWave(CGame::WAVE_BOSS);
	}
	else
	{
		for (int nCnt = 0; nCnt < m_nMaxPhaseNum; nCnt++)
		{
			if (CTime::GetTime() / 1000 >= m_pSpawn[nCnt].nPhaseTime)
			{// 60�b

				// �G�̍ő吔�i���悻�j
				nMaxEnemy = m_pSpawn[nCnt].nMaxEnemy;
				// �G�l�~�[�̃X�|�[���p�x
				nSpawnCount = m_pSpawn[nCnt].nSpawnCount;
				// �T�[�N���̍ő吔
				CManaCircle::SetCircleMax(m_pSpawn[nCnt].nCircleNum);
			}
		}
	}

	if (m_nNumEnemy <= nMaxEnemy)
	{
		// �J�E���g��i�߂�
		m_nCntSpawn++;

		// ���̊Ԋu��
		if (m_nCntSpawn%nSpawnCount == 0)
		{
			// �N�����̃����_��
			int nRand = rand() % 5 + 3;

			for (int nCnt = 0; nCnt < nRand; nCnt++)
			{
				// �X�e�[�W���W�̎擾
				D3DXVECTOR3 stagePos = STAGE_POS;

				// �����_���Ȋp�x
				int nRandAngle = rand() % 360;
				// �����_���ȋ���
				int nRandDistance = rand() % (int)STAGE_SIZE.x - (int)STAGE_SIZE.x / 2;
				// ��̏��ō��W�̐���
				D3DXVECTOR3 randPos = D3DXVECTOR3(stagePos.x + cosf(D3DXToRadian(nRandAngle))*nRandDistance, stagePos.y + sinf(D3DXToRadian(nRandAngle))*nRandDistance, 0);

				// �v���C���[�������Ă���Ƃ�
				if (CPlayer::IsAlive())
				{
					// �v���C���[���W�̎擾
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

					// �v���C���[�ɋ߂������������̍~��Ȃ���
					while (ENEMY_SPAWN_DISTANCE > sqrtf(powf(randPos.x - playerPos.x, 2) + powf(randPos.y - playerPos.y, 2)))
					{
						// �����_���Ȋp�x
						nRandAngle = rand() % 361;
						// �����_���ȋ���
						nRandDistance = rand() % (int)STAGE_SIZE.x - (int)STAGE_SIZE.x / 2;
						// ��̏��ō��W�̐���
						randPos = D3DXVECTOR3(stagePos.x + cosf(D3DXToRadian(nRandAngle))*nRandDistance, stagePos.y + sinf(D3DXToRadian(nRandAngle))*nRandDistance, 0);
					}

					// �G�l�~�[�̐���
					CEnemySpawn::Create(randPos, (CEnemy::ENEMYTYPE)(rand() % CEnemy::ENEMYTYPE_MAX));
				}
			}
		}
	}
}

//==================================
// ����������
//==================================
HRESULT CEnemy::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture[m_type]);

	// �T�C�Y�̐ݒ�
	SetSize(D3DXVECTOR3(ENEMY_SIZE, ENEMY_SIZE, 0.0f));
	
	if (m_type == ENEMYTYPE_RED)
	{
		// �ړ��ʂ̏�����
		int nRand = rand() % 360;

		// �ړ��ʂ̐ݒ�
		m_move.x = cosf(D3DXToRadian(nRand))*ENEMY_SPEED*2.0f;
		m_move.y = sinf(D3DXToRadian(nRand))*ENEMY_SPEED*2.0f;
	}

	// �G�l�~�[���̃J�E���g
	m_nNumEnemy++;

	return S_OK;
}

//==================================
// �I������
//==================================
void CEnemy::Uninit(void)
{
	// ���X�g����폜
	std::list<CEnemy*>::iterator iteretor;
	for (iteretor = m_enemyList.begin(); iteretor != m_enemyList.end(); )
	{
		// �C���f�b�N�X�ԍ���������������
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = m_enemyList.erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}
	}

	CScene3d::Uninit();

	// �G�l�~�[�������炷
	m_nNumEnemy--;
}

//==================================
// �X�V����
//==================================
void CEnemy::Update(void)
{

	// ���݂̍��W�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��O�̍��W�̕ۑ�
	D3DXVECTOR3 posOld = pos;

	if (CPlayer::IsAlive())
	{
		// �ړ��ʂ̐ݒ�
		MoveManager();
	}

	// ���W�Ɉړ��ʂ̃v���X
	pos += m_move;

	// �ʂ̃G�l�~�[�Ƃ̓����蔻��
	std::list<CEnemy*>::iterator iteretor;
	for (iteretor = m_enemyList.begin(); iteretor != m_enemyList.end(); iteretor++)
	{
		if (GetID() != (*iteretor)->GetID())
		{
			// ���g�̃T�C�Y
			D3DXVECTOR3 size = GetSize();
			// �ʂ̓G�̍��W
			D3DXVECTOR3 enemyPos = (*iteretor)->GetPos();
			// �ʂ̓G�̃T�C�Y
			D3DXVECTOR3 enemySize = (*iteretor)->GetSize();

			if (powf(pos.x - enemyPos.x, 2) + powf(pos.y - enemyPos.y, 2) <= powf(ENEMY_SIZE, 2))
			{
				// �p�x
				float fAngle = atan2f(enemyPos.y - pos.y, enemyPos.x - pos.x);
				// ���W�̐ݒ�
				enemyPos.x = pos.x + cosf(fAngle)*ENEMY_SIZE;
				enemyPos.y = pos.y + sinf(fAngle)*ENEMY_SIZE;

				// ���W�̃Z�b�g
				(*iteretor)->SetPos(enemyPos);
			}
		}
	}

	// �v���C���[�̓����蔻��
	if (CPlayer::IsAlive())
	{

		D3DXVECTOR3 size = GetSize();  // �e�̃T�C�Y

		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();   // �v���C���[�̍��W
		D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize(); // �v���C���[�̃T�C�Y

		if (powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2) <= powf(playerSize.x / 2 + size.x / 2, 2))
		{
			// �p�x
			/*float fAngle = atan2f(pos.y - playerPos.y, pos.x - playerPos.x);
			pos.x = playerPos.x + cosf(fAngle)*ENEMY_SIZE/2+playerSize.x/2;
			pos.y = playerPos.y + sinf(fAngle)*ENEMY_SIZE/2+playerSize.x/2;
			SetPos(pos);*/

			if (CGame::GetPlayer()->GetState() == CPlayer::STATE_NORMAL)
			{
				CGame::GetPlayer()->HitAttack(1);
			}
		}
	}

	// �X�e�[�W�Ƃ̓����蔻��

	// �X�e�[�W���W�̎擾
	D3DXVECTOR3 stagePos = STAGE_POS;
	// �����̌v�Z
	float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

	// �X�e�[�W�[�̐ݒ�
	if (fDistanceStage + ENEMY_SIZE / 2 > STAGE_SIZE.x / 2)
	{
		// �p�x
		float fAngle = atan2f(pos.y - stagePos.y, pos.x - stagePos.x);
		// ���W�̐ݒ�
		pos.x = stagePos.x + cosf(fAngle)*(STAGE_SIZE.x / 2 - ENEMY_SIZE / 2);
		pos.y = stagePos.y + sinf(fAngle)*(STAGE_SIZE.x / 2 - ENEMY_SIZE / 2);
	}

	//if (CBoss::IsAlive())
	//{
	//	if (fDistanceStage < 200)
	//	{
	//		float fAngle = atan2f(pos.y - stagePos.y, pos.x - stagePos.x);

	//		pos = D3DXVECTOR3(cosf(fAngle)*(200), sinf(fAngle)*(200), 0.0f);

	//		// �ړ������̕ύX
	//		int nRand = rand() % 360;

	//		// �ړ��ʂ̐ݒ�
	//		m_move.x = cosf(D3DXToRadian(nRand))*ENEMY_SPEED;
	//		m_move.y = sinf(D3DXToRadian(nRand))*ENEMY_SPEED;
	//	}
	//}

	// ���W�̃Z�b�g
	SetPos(pos);


	// �U���̏���
	//AttackManager();


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
	default:
		break;
	}

	// �{�X�펞�G�l�~�[������
	if (CGame::GetWave() == CGame::WAVE_BOSS)
	{
		
		// �p�[�e�B�N������
		ParticleCreate();

		// ����
		Uninit();
	}
	
}

//==================================
// �`�揈��
//==================================
void CEnemy::Draw(void)
{
	CScene3d::Draw();
}

//==================================
// �U�����q�b�g��������
//==================================
void CEnemy::HitAttack(int nDamage)
{
	// ���C�t����_���[�W������
	m_nLife -= nDamage;


	if (m_nLife <= 0)
	{// ���C�t��0�ȉ�

		// �p�[�e�B�N������
		ParticleCreate();

		int nScore = 100;
		if (CGame::GetPlayer()->GetFever())
		{// �t�B�[�o�[���X�R�A�{�[�i�X
			nScore *= 1.5f;
		}
		// �X�R�A���Z
		CScore::AddScore(nScore);

		// �t�B�[�o�[�l��i�߂�
		CGame::GetPlayer()->ProgressFever(1.0f);

		if ((rand() % 10) == 0)
		{
			// �A�C�e������
			CItem::Create(GetPos(), (CItem::ITEMTYPE)(rand() % CItem::ITEMTYPE_MAX));
		}
		// ����
		Uninit();

		return;
	}
	else
	{// ���C�t���c���Ă���
		m_state = STATE_DAMAGE;
		m_nStateCount = ENEMY_STATE_COUNT;
	}
}

//==================================
// �ړ��̊Ǘ�
//==================================
void CEnemy::MoveManager(void)
{
	
	// ���݂̍��W�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �v���C���[���W�̎擾
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	

	// �^�C�v�ɂ���Ĉړ��𕪂���
	switch (m_type)
	{
	case ENEMYTYPE_YELLOW:
	{// ���F�G�l�~�[

		// �v���C���[�Ƃ̊p�x
		float fAnglePlayer = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		// �ړ��ʂ̐ݒ�
		m_move.x = cosf(fAnglePlayer)*ENEMY_SPEED;
		m_move.y = sinf(fAnglePlayer)*ENEMY_SPEED;

		// ��]������
		SetAngle(D3DXToDegree(fAnglePlayer));
	}
	break;

	case ENEMYTYPE_PURPLE:
	{// ���G�l�~�[

		// �v���C���[�Ƃ̋����̎擾
		float fDistance = sqrtf(powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2));

		// �v���C���[�Ƃ̊p�x
		m_fMoveAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

		// �W�O�U�O�����������i���̋����ȓ����Ǝ��@�̕����Ɉړ��j
		if (fDistance >= MOVE_CHANGE_DISTANCE)
		{
			// �J�E���g��i�߂�
			m_nCntMove++;
			// ���̃J�E���g�Ńv���X���}�C�i�X���؂�ւ�
			if (m_nCntMove >= MOVE_COUNT)
			{
				m_nCntMove = rand() % 20;
				m_bPlusAngle ^= true;
			}

			if (m_bPlusAngle)
			{// �v���X
				m_fMoveAngle += D3DXToRadian(45);
			}
			else
			{// �}�C�i�X
				m_fMoveAngle -= D3DXToRadian(45);
			}
		}

		// �ړ��ʂ̐ݒ�
		m_move.x = cosf(m_fMoveAngle)*ENEMY_SPEED*1.5f;
		m_move.y = sinf(m_fMoveAngle)*ENEMY_SPEED*1.5f;

		float fAngle = atan2f(m_move.y, m_move.x);
		// ��]������
		SetAngle(D3DXToDegree(fAngle));
	}
	break;

	case ENEMYTYPE_RED:
	{// �ԃG�l�~�[

		// �X�e�[�W���W�̎擾
		D3DXVECTOR3 stagePos = STAGE_POS;
		// �����̌v�Z
		float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

		// �X�e�[�W�[�̐ݒ�
		if (fDistanceStage + ENEMY_SIZE / 2 > STAGE_SIZE.x / 2)
		{
			// �v���C���[�Ƃ̊p�x
			m_fMoveAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

			// �ړ��ʂ̐ݒ�
			m_move.x = cosf(m_fMoveAngle)*ENEMY_SPEED*2.0f;
			m_move.y = sinf(m_fMoveAngle)*ENEMY_SPEED*2.0f;
		}

		SetAngle(GetAngle()+2.0f);
	}
	break;

	default:
		break;
	}

	
}

//==================================
// �G�l�~�[���X�g���̃p�[�e�B�N������
//==================================
void CEnemy::ParticleCreate(void)
{
	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		// �����_���ȑ��x
		int nRandSpeed = rand() % 3 + 1;
		// �����_���Ȋp�x
		int nRandAngle = rand() % 360;
		// ��̏��ňړ��ʂ̐���
		D3DXVECTOR3 particleMove;
		particleMove.x = cosf(D3DXToRadian(nRandAngle))*nRandSpeed;
		particleMove.y = sinf(D3DXToRadian(nRandAngle))*nRandSpeed;
		particleMove.z = 0.0f;

		// �����_���ȃT�C�Y
		int nRandSize = rand() % 10 + 10;

		// �G�l�~�[�̃^�C�v�ɂ�����ĐF����
		D3DXCOLOR randCol = {};
		switch (m_type)
		{
		case ENEMYTYPE_YELLOW:
			// ���F���ۂ�
			randCol = D3DXCOLOR(1.0f, 1.0f, (float)(rand() % 600) / 1000.0f, 1.0f);
			break;
		case ENEMYTYPE_PURPLE:
			// �����ۂ�
			randCol = D3DXCOLOR(1.0f, (float)(rand() % 600) / 1000.0f, 1.0f, 1.0f);
			break;
		case ENEMYTYPE_RED:
			// �Ԃ��ۂ�
			randCol = D3DXCOLOR(1.0f, (float)(rand() % 600) / 1000.0f, (float)(rand() % 600) / 1000.0f, 1.0f);
			break;
		default:
			break;
		}

		// �p�[�e�B�N������
		CParticle::Create(GetPos(),
			particleMove,
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f), rand() % 10 + 20,
			randCol,
			CParticle::PARTICLE_STAR);

	}
}
