////////////////////////////////////////////////////
//
//    particle�N���X�̏���[particle.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "camera.h"
#include "game.h"
#include "debug_log.h"
#include "collision.h"
#include "destination.h"
#include "mouse.h"
#include "item.h"
#include "rank.h"
#include "gauge.h"
#include "chain_ui.h"
#include "goal_ui.h"
#include "evolution_effect.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH           "./data/Models/testplayer.x"            //���f���̃p�X
#define HIERARCHY_TEXT_PATH1 "./data/Texts/hierarchy/Player1.txt"    //�K�w�\���e�L�X�g�̃p�X
#define HIERARCHY_TEXT_PATH2 "./data/Texts/hierarchy/Player2.txt"    //�K�w�\���e�L�X�g�̃p�X
#define HIERARCHY_TEXT_PATH3 "./data/Texts/hierarchy/Player3.txt"    //�K�w�\���e�L�X�g�̃p�X
#define HIERARCHY_TEXT_PATH4 "./data/Texts/hierarchy/Player4.txt"    //�K�w�\���e�L�X�g�̃p�X

#define PLAYER_GRAVITY D3DXVECTOR3(0.0f,-120.0f,0.0f)   // �d�͗�
#define PLAYER_GRAVITY_RATE 0.3f                     // �d�͂̌W��
#define PLAYER_DIRECTION_RATE 0.1f                   // �����̌W��
#define PLAYER_RADIUS 150                            // �����蔻��̔��a
#define HIT_FRAME  100                               // �q�b�g��Ԃ̃t���[����
#define ACCELERATION_FRAME  100                      // ���������t���[��
#define ACCELERATION_DIST   50                       // �������̉��Z�l
#define ACCELERATION_RATE   0.03f                    // �����l�̌W��
#define ACCELERATION_ITEM_DROP_INTERVAL 20           // �������ɃA�C�e���𗎂Ƃ��C���^�[�o��
#define CHAIN_SPEED_BONUS 1                          // �`�F�C���X�s�[�h�{�[�i�X�̍ő�
#define CHAIN_SPEED_RATE  0.25f                      // �`�F�C���X�s�[�h�{�[�i�X�̌W��

// �v���C���[���x
#define PLAYER_SPEED_E1 100       // �v���C���[���x*1�i�K��
#define PLAYER_SPEED_E2 120       // �v���C���[���x*2�i�K��
#define PLAYER_SPEED_E3 140       // �v���C���[���x*3�i�K��
#define PLAYER_SPEED_E4 160       // �v���C���[���x*4�i�K��
#define PLAYER_SPEED_HIT 20       // ��Q���q�b�g���̃v���C���[���x
// �����x
#define PLAYER_MOVE_RATE_E1 0.03f  // �����̌W��*1�i�K��
#define PLAYER_MOVE_RATE_E2 0.03f  // �����̌W��*2�i�K��
#define PLAYER_MOVE_RATE_E3 0.025f // �����̌W��*3�i�K��
#define PLAYER_MOVE_RATE_E4 0.02f  // �����̌W��*4�i�K��
// �i�����̃p�[�c��
#define EVOLUTION_0 1             // ����
#define EVOLUTION_1 2             // 2�i�K��
#define EVOLUTION_2 3             // 3�i�K��
#define EVOLUTION_3 m_nNumModel   // 4�i�K��

//�@�e�i���܂ł̒l
#define EVOLUTION_NUM_1 5   // 2�i�K��
#define EVOLUTION_NUM_2 10   // 3�i�K��
#define EVOLUTION_NUM_3 15   // 4�i�K��

// �i�����Q�[�W�T�C�Y
#define GAUGE_SIZE D3DXVECTOR3(20.0f,150.0f,0.0f)

// �i���Q�[�W�ʒu
#define GAUGE_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2)                  , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define GAUGE_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define GAUGE_POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define GAUGE_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define GAUGE_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define GAUGE_POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // �v���C���[3
#define GAUGE_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // �v���C���[4

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model CPlayer::m_model[MAX_PLAYER_NUM][MAX_PARTS_NUM] = {};
int CPlayer::m_nNumModel[MAX_PLAYER_NUM] = {};

// �i�����Ƃ̃p�[�c��
int CPlayer::m_nNumEvolutionParts[MAX_EVOLUTION]=
{
	EVOLUTION_0,
	EVOLUTION_1,
	EVOLUTION_2,
	4,
};
// �i�����Ƃ̍ō����x
int CPlayer::m_nSpeedData[MAX_EVOLUTION] =
{
	PLAYER_SPEED_E1,       // �v���C���[���x*1�i�K��
	PLAYER_SPEED_E2,       // �v���C���[���x*2�i�K��
	PLAYER_SPEED_E3,       // �v���C���[���x*3�i�K��
	PLAYER_SPEED_E4,       // �v���C���[���x*4�i�K��
};

// �i�����Ƃ̉����x
float CPlayer::m_fRateData[MAX_EVOLUTION] =
{
	PLAYER_MOVE_RATE_E1,   // �����̌W��*1�i�K��
	PLAYER_MOVE_RATE_E2,   // �����̌W��*2�i�K��
	PLAYER_MOVE_RATE_E3,   // �����̌W��*3�i�K��
	PLAYER_MOVE_RATE_E4,   // �����̌W��*4�i�K��
};

int CPlayer::m_nEvoData[MAX_EVOLUTION] =
{
	EVOLUTION_NUM_1, // 2�i�K��
	EVOLUTION_NUM_2, // 3�i�K��
	EVOLUTION_NUM_3, // 4�i�K��
	1,
};

char CPlayer::m_acModelText[MAX_PLAYER_NUM][64]=
{
	{ HIERARCHY_TEXT_PATH1 },
	{ HIERARCHY_TEXT_PATH2 },
	{ HIERARCHY_TEXT_PATH3 },
	{ HIERARCHY_TEXT_PATH4 },
};

//******************************
// �R���X�g���N�^
//******************************
CPlayer::CPlayer() :CModelHierarchy(OBJTYPE_PLAYER)
{
	// �ϐ��̃N���A
	//m_move = VEC3_ZERO;              // �ړ���
	m_nPlayerNum = 0;                  // �v���C���[�ԍ�
	m_gravityVec = VEC3_ZERO;          // �d�͗�
	m_bGravity = true;                 // �d�̓t���O
	m_pCollision = NULL;               // �R���W����
	m_nNumCheckpoint = 0;              // �`���b�N�|�C���g��
	m_bGoal = false;                   // �S�[���t���O
	m_bMove = false;                   // �ړ��t���O
	m_nChain = 0;                      // �`�F�C����
	m_nMaxChain = 0;                   // �ő�`�F�C����
	m_nCollectItem = 0;                // ��������A�C�e����
	m_nNumEvolution = 0;               // �i����
	m_fEvoGauge = 0.0f;                // �i���Q�[�W�̒l
	m_pDest = NULL;                    // �ړ��ڕW�N���X
	m_bDriftLeft = false;              // �h���t�g��
	m_bDriftRight = false;             // �h���t�g�E
	m_bHit = false;                    // �q�b�g��ԃt���O
	m_nCntHit = 0;                     // �q�b�g��ԉ������̃J�E���g
	m_fMaxSpeed = PLAYER_SPEED_E1;     // �ő呬�x
	m_bAccelerationFlag = false;       // �����t���O
	m_nCntAcceleration = 0;            // �������̃J�E���g
	m_fAcceleration = 0.0f;            // ������Ԃ̒l
	m_fMoveRate = PLAYER_MOVE_RATE_E1; // �����̌W��
	m_nRank = 0;                       // ����
	m_pGauge = NULL;
}

//******************************
// �f�X�g���N�^
//******************************
CPlayer::~CPlayer()
{
}

//******************************
// �N���G�C�g
//******************************
CPlayer * CPlayer::Create(D3DXVECTOR3 pos,int nPlayerNum)
{
	// �������̊m��
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	pPlayer->m_nPlayerNum = nPlayerNum;
	// ������
	pPlayer->Init();
	
	// �e�l�̑���E�Z�b�g
	pPlayer->SetPos(pos);
	pPlayer->SetPriority(OBJTYPE_PLAYER); // �I�u�W�F�N�g�^�C�v
	// �����蔻��̐���
	pPlayer->m_pCollision = CCollision::CreateSphere(pos, PLAYER_RADIUS);
	
	// �ړ��ڕW�̐���
	pPlayer->m_pDest = CDestination::Create(nPlayerNum, pos);
	return pPlayer;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CPlayer::Load(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER_NUM; nCnt++)
	{
		// ���f���̓ǂݍ���
		LoadModels(m_acModelText[nCnt], &m_model[nCnt][0], &m_nNumModel[nCnt]);
		// �ŏI�i�����p�[�c��
		m_nNumEvolutionParts[MAX_EVOLUTION - 1] = m_nNumModel[nCnt];
	}

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CPlayer::Unload(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		for (int nCnt = 0; nCnt < m_nNumModel[nCntPlayer]; nCnt++)
		{
			//���b�V���̔j��
			if (m_model[nCntPlayer][nCnt].pMesh != NULL)
			{
				m_model[nCntPlayer][nCnt].pMesh->Release();
				m_model[nCntPlayer][nCnt].pMesh = NULL;
			}
			//�}�e���A���̔j��
			if (m_model[nCntPlayer][nCnt].pBuffMat != NULL)
			{
				m_model[nCntPlayer][nCnt].pBuffMat->Release();
				m_model[nCntPlayer][nCnt].pBuffMat = NULL;
			}
		}
	}
}


//******************************
// ����������
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CModelHierarchy::Init(m_nNumEvolutionParts[m_nNumEvolution], &m_model[m_nPlayerNum][0], m_acModelText[m_nPlayerNum])))
	{
		return E_FAIL;
	}

	// ���f�����̊��蓖��
	//BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);
	//
	//// �e�N�X�`���̊��蓖��
	//for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	//{
	//	if (m_model.apTexture[nCnt] != NULL)
	//	{
	//		BindTexture(nCnt, m_model.apTexture[nCnt]);
	//	}
	//}

	// �ϐ��̏�����
	m_bGravity = true;                 // �d�̓t���O
	m_nNumCheckpoint = 0;              // �`�F�b�N�|�C���g��
	m_bGoal = false;                   // �S�[���t���O
	m_bMove = false;                   // �ړ��t���O
	m_nChain = 0;                      // �`�F�C����
	m_nCollectItem = 0;                // ��������A�C�e����
	m_bDriftLeft = false;              // �h���t�g��
	m_bDriftRight = false;             // �h���t�g�E
	m_bHit = false;                    // �q�b�g�t���O
	m_nCntHit = 0;                     // �q�b�g���̃J�E���g
	m_fMaxSpeed = PLAYER_SPEED_E1;     // �ő呬�x
	m_bAccelerationFlag = false;       // �����t���O
	m_fMoveRate = PLAYER_MOVE_RATE_E1; // �����̌W��
	m_nRank = m_nPlayerNum + 1;        // ����
	m_fEvoGauge = 0.0f;                // �i���Q�[�W�̒l
	m_nMaxChain = 0;                   // �ő�`�F�C����

	// �����NUI�̐���
	CRank::Create(m_nPlayerNum);

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();
	D3DXVECTOR3 gaugePos;
	D3DXVECTOR3 gaugeSize;
	// �v���C���[�ԍ��ɂ���ĕ���
	switch (m_nPlayerNum)
	{
	case 0:
		// �v���C���[1
		// �����Ȃ�
		if (nNumPlayer == 1)
		{
			gaugePos = GAUGE_POS_PLAYER1_1;
			gaugeSize = GAUGE_SIZE;
		}
		// �񕪊�
		else if (nNumPlayer == 2) 
		{
			gaugePos = GAUGE_POS_PLAYER1_2; 
			gaugeSize = GAUGE_SIZE / 1.5f;
		}
		// �l����
		else
		{
			gaugePos = GAUGE_POS_PLAYER1_4; 
			gaugeSize = GAUGE_SIZE / 2;
		}

		break;
	case 1:
		// �v���C���[2

		// �񕪊�
		if (nNumPlayer == 2)
		{
			gaugePos = GAUGE_POS_PLAYER2_2;
			gaugeSize = GAUGE_SIZE / 1.5f;
		}
		// �l����
		else
		{
			gaugePos = GAUGE_POS_PLAYER2_4;
			gaugeSize = GAUGE_SIZE / 2;
		}

		break;
	case 2:
		// �v���C���[3
		gaugePos = GAUGE_POS_PLAYER3;
		gaugeSize = GAUGE_SIZE / 2;
		break;
	case 3:
		// �v���C���[4
		gaugePos = GAUGE_POS_PLAYER4;
		gaugeSize = GAUGE_SIZE / 2;
		break;
	default:
		break;
	}
	gaugePos.y -= gaugeSize.y / 2;
	m_pGauge = CGauge::Create(&m_fEvoGauge, D3DXVECTOR3(gaugePos.x, gaugePos.y + gaugeSize.y, 0.0f), gaugeSize.x, gaugeSize.y * 2, EVOLUTION_NUM_1, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	// �`�F�C��UI�̐���
	CChainUi::Create(m_nPlayerNum);

	return S_OK;
}

//******************************
// �I������
//******************************
void CPlayer::Uninit(void)
{
	CModelHierarchy::Uninit();
}

//******************************
// �X�V����
//******************************
void CPlayer::Update(void)
{
	// �d�͂̏���
	Gravity();
	// �h���t�g�̏���
	Drift();
	
	// �����蔻��̈ʒu�X�V
	m_pCollision->SetPos(GetPos());

	// �q�b�g��Ԃ̊Ǘ�
	if (m_bHit)
	{// �q�b�g��Ԃ̎�
		m_nCntHit--;
		if (m_nCntHit <= 0)
		{// �J�E���g��0�ȉ��̎�
			// �q�b�g�t���O��false��
			m_bHit = false;
		}
		m_fMaxSpeed = PLAYER_SPEED_HIT;
	}
	else
	{
		// �ō����x�̍X�V
		m_fMaxSpeed = m_nSpeedData[m_nNumEvolution];

		// �`�F�C�����ɂ���čō����x�ɉ��Z
		m_fMaxSpeed += (CHAIN_SPEED_BONUS*m_nChain)*CHAIN_SPEED_RATE;
	}

	// �����̏���
	Acceleration();
	// �v���C���[���m�̓����蔻��
	CollisionPlayer();
#ifdef _DEBUG
	// �f�o�b�O�R�}���h

	// �i��
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_E))
	{
		Evolution();
	}

	// �A�C�e���h���b�v
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_D))
	{
		if (m_nPlayerNum == 0)
		{
			// �A�C�e�����юU�点��
			CItem::DropItemCircle(D3DXVECTOR3(GetPos().x, GetPos().y + 80, GetPos().z), 5, m_nPlayerNum);
		}
		
	}
	if (CManager::GetKeyboard()->GetKeyPress(DIK_S))
	{
	//	CItem::DropItem(GetPos(), m_nPlayerNum);
	}

	// �X�g�b�v
	if (CManager::GetKeyboard()->GetKeyPress(DIK_S))
	{
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyRelease(DIK_S))
	{
		m_bMove = true;
	}

	// ����
	if (!m_bAccelerationFlag)
	{
		if (CManager::GetKeyboard()->GetKeyPress(DIK_A))
		{
			SetAccelerationFrag(true);
		}
	}
#endif
}

//******************************
// �`�揈��
//******************************
void CPlayer::Draw(void)
{
	CModelHierarchy::Draw();
}

//******************************
// �S�[�����̏���
//******************************
void CPlayer::Goal(void)
{
	// �S�[���t���O��true��
	m_bGoal = true;
	// �S�[��UI�̐���
	CGoalUi::Create(m_nPlayerNum);
}

//******************************
// �`�F�C�����̉��Z
//******************************
void CPlayer::AddChainNum(void)
{
	// ���Z
	m_nChain++;

	if (m_nChain >= m_nMaxChain)
	{
		// �ő剻�`�F�C�����̕ۑ�
		m_nMaxChain = m_nChain;
	}
}

//******************************
// �����t���O�̃Z�b�g
//******************************
void CPlayer::SetAccelerationFrag(bool bAccele)
{
	m_bAccelerationFlag = bAccele;
	
	if (m_bAccelerationFlag)
	{
		// �J�E���g�̏�����
		m_nCntAcceleration = 0;
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ACCELERATION);
	}
}

//******************************
// �A�C�e���ɓ��������Ƃ��̏���
//******************************
void CPlayer::HitItem(bool bSafe)
{
	if (bSafe)
	{// �����A�C�e���̓��������Ƃ�
		m_nCollectItem++;
		AddChainNum();
		if (m_nNumEvolution < MAX_EVOLUTION)
		{
			m_fEvoGauge++;
			if (m_nEvoData[m_nNumEvolution] <= m_fEvoGauge)
			{
				Evolution();
			}
		}
		else
		{
			m_nNumEvolution = 1;
		}
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM);
	}
	else
	{// �����ȊO�̃A�C�e���ɓ��������Ƃ�
		
		// �`�F�C������0�ɂ���
		m_nChain = 0;
		// �i���̒l��0�ɂ���
		m_fEvoGauge = 0.0f;
		// �q�b�g�t���O�𗧂Ă�
		m_bHit = true;
		// �J�E���g�̏�����
		m_nCntHit = HIT_FRAME;
		// ������false�ɂ���
		m_bAccelerationFlag = false;
		// �A�C�e�����юU�点��
		CItem::DropItemCircle(D3DXVECTOR3(GetPos().x, GetPos().y + 80, GetPos().z), 5, m_nPlayerNum);
		// �J������h�炷
		CGame::GetCamera(m_nPlayerNum)->Shake(true);

		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT);
	}
}

//******************************
// ����̊Ǘ�
//******************************
void CPlayer::MoveControll(void)
{
//	// ���W
//	D3DXVECTOR3 pos = GetPos();
//	//�ړ��ʂ̖ڕW�l
//	D3DXVECTOR3 moveDest = VEC3_ZERO;
//
//	// �J�����̌����̎擾
//	float fCameraAngle = CGame::GetCamera(m_nPlayerNum)->GetAngle();
//#if 0
//	if (m_bMove)
//	{
//		moveDest.x = cosf(fCameraAngle)* PLAYER_SPEED;
//		moveDest.z = sinf(fCameraAngle)* PLAYER_SPEED;
//	}
//
//#else
//	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
//	{
//		moveDest.x = cosf(fCameraAngle)* PLAYER_SPEED;
//		moveDest.z = sinf(fCameraAngle)* PLAYER_SPEED;
//	}
//
//#endif
//	// ����
//	m_move += (moveDest - m_move) * PLAYER_MOVE_RATE;
//
//	// �ړ��ʂ𑫂�
//	pos += m_move;
//
//	// ���W�̃Z�b�g
//	SetPos(pos);
//
//	// �v���C���[�̌������J�����ɍ��킹��
//	D3DXVECTOR3 rot = GetRot();
//	rot.y = -(fCameraAngle + D3DXToRadian(90));
//	//rot.x += 0.01f;
//	SetRot(rot);
}

//******************************
// �d�͂̊Ǘ�
//******************************
void CPlayer::Gravity(void)
{
	if (m_bGravity)
	{// �d�̓t���O�������Ă���

		// �d�͗ʂ̐ݒ�
		m_gravityVec += (PLAYER_GRAVITY - m_gravityVec)*PLAYER_GRAVITY_RATE;

		// ���W�ɏd�͗ʂ̃v���X
		SetPos(GetPos() + m_gravityVec);

	}
	else
	{// �d�̓t���O�������ĂȂ��Ƃ�
		// ������
		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// �i��
//******************************
void CPlayer::Evolution(void)
{
	// �i���񐔂̃J�E���g
	m_nNumEvolution++;
	if (m_nNumEvolution >= MAX_EVOLUTION)
	{// �i�������ő�𒴂��Ȃ��悤��
		m_nNumEvolution = MAX_EVOLUTION - 1;
	}
	else
	{
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EVOLUTION);
		// �i���G�t�F�N�g�Ƃ̐���
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			float fRandX = 0.0f;
			float fRandY = D3DXToRadian(rand() % 360);
			float fRandZ = 0.0f;
			CEvoEffect::Create(m_nPlayerNum)->SetRot(D3DXVECTOR3(fRandX, fRandY, fRandZ));
		}

		// �A�C�e�����юU�点��
		CItem::DropItemCircle(GetPos(), 5, m_nPlayerNum);
	}

	// �ō����x�̍X�V
	m_fMaxSpeed = m_nSpeedData[m_nNumEvolution];
	// �����x
	m_fMoveRate = m_fRateData[m_nNumEvolution];
	// �p�[�c���̓ǂݍ���
	CModelHierarchy::Init(m_nNumEvolutionParts[m_nNumEvolution], &m_model[m_nPlayerNum][0], m_acModelText[m_nPlayerNum]);
	// �i���l�̏�����
	m_fEvoGauge = 0.0f;
	// �Q�[�W�̍ő吔�̍X�V
	m_pGauge->SetMaxNum(m_nEvoData[m_nNumEvolution]);
}

//******************************
// �h���t�g
//******************************
void CPlayer::Drift(void)
{
	float fDistance = 0.0f;// �ړ��ڕW�Ƃ̋���

	if (!m_bDriftLeft && !m_bDriftRight)
	{// �h���t�g���t�@���X��Ԃ̎�

		// �L�[�{�[�h
		if (CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			if (CManager::GetMouse()->GetMouseMove().x > 0)
			{// �E�h���t�g
				m_bDriftRight = true;

			}
			else if (CManager::GetMouse()->GetMouseMove().x < 0)
			{// ���h���t�g
				m_bDriftLeft = true;
			}
		}

		// �R���g���[���[
		if (CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum))
		{
			if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX >= 10)
			{// �E�h���t�g
				m_bDriftRight = true;
			}
			else if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX <= -10)
			{// ���h���t�g
				m_bDriftLeft = true;
			}
		}
	}
	else
	{
		// �h���t�g��Ԃ̉���
		if (CManager::GetJoypad()->GetJoystickRelease(5, m_nPlayerNum) || CManager::GetKeyboard()->GetKeyRelease(DIK_LSHIFT))
		{
			m_bDriftLeft = false;
			m_bDriftRight = false;
		}
	}
	//if (m_pDest->GetDistanceDest() != fDistance)
	//{
	//	m_pDest->SetDistanceDest(fDistance);
	//}
}

//******************************
// �����̏���
//******************************
void CPlayer::Acceleration(void)
{
	if (m_bAccelerationFlag)
	{// �����t���O�������Ă���Ƃ�
		// �q�b�g�t���O��false�ɂ���
		m_bHit = false;
		// �����l��ڕW�ɋ߂Â���
		m_fAcceleration += (ACCELERATION_DIST - m_fAcceleration)*ACCELERATION_RATE;
		// �J�E���g��i�߂�
		m_nCntAcceleration++;

		if (m_nCntAcceleration%ACCELERATION_ITEM_DROP_INTERVAL == 0)
		{
			CItem::DropItem(GetPos(), m_nPlayerNum);
		}
		//���̃J�E���g�ɒB������
		if (m_nCntAcceleration > ACCELERATION_FRAME)
		{
			// �J�E���g�̏�����
			m_nCntAcceleration = 0;
			// �����t���O��false�ɂ���
			m_bAccelerationFlag = false;
		}
	}
	else
	{
		// �����l��0�߂Â���
		m_fAcceleration += (0 - m_fAcceleration)*ACCELERATION_RATE;
	}

	m_fMaxSpeed += m_fAcceleration;
}

//******************************
// �v���C���[���m�̓����蔻��
//******************************
void CPlayer::CollisionPlayer(void)
{
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);
	while (pPlayer != NULL)
	{
		if (pPlayer->GetID() != GetID())
		{
			// �����蔻��
			if (CCollision::CollisionSphere(m_pCollision, pPlayer->GetCollision()))
			{// �������Ă���

				// �O�ɉ����o��
				D3DXVECTOR3 vec = (GetPos() - pPlayer->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pPlayer->GetCollision()->GetCollisionRadius());

				D3DXVECTOR3 pos = GetPos();
				SetPos(pPlayer->GetPos() + vec);
			}
		}

		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
}
