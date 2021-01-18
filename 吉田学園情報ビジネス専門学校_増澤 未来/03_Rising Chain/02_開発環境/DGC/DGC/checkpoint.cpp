////////////////////////////////////////////////////
//
//    particle�N���X�̏���[particle.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "checkpoint.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "player.h"


//*****************************
// �}�N����`
//*****************************
#define CHECKPOINT_RADIUS_EASY 3500     // �����蔻��̔��a
#define CHECKPOINT_RADIUS_NORMAL 2700     // �����蔻��̔��a

// �e�L�X�g�̃p�X
#define CHECKPOINT_TEXT_EASY   "data/Texts/EasyMapCheckpoint.txt"   // �C�[�W�[
#define CHECKPOINT_TEXT_NORMAL "data/Texts/NormalMapCheckpoint.txt" // �m�[�}��
#define CHECKPOINT_TEXT_HARD   "data/Texts/HardMapCheckpoint.txt"   // �n�[�h

// �`�F�b�N�|�C���g�z�u���[�h�؂�ւ��}�N��
#define SET_MODE
#undef  SET_MODE

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CCheckPoint *CCheckPoint::m_pCheckPoint = NULL;
//******************************
// �R���X�g���N�^
//******************************
CCheckPoint::CCheckPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// �ϐ��̃N���A
	m_type = CCourse::COURSE_EASY;
	m_nNumPoint = 0;
	memset(&m_pCollision, 0, sizeof(m_pCollision)); 
	memset(&m_pointPos, 0, sizeof(m_pointPos));
}

//******************************
// �f�X�g���N�^
//******************************
CCheckPoint::~CCheckPoint()
{
}

//******************************
// �N���G�C�g
//******************************
CCheckPoint * CCheckPoint::Create(CCourse::COURSETYPE type)
{
	// �������̊m��
	m_pCheckPoint = new CCheckPoint;

	// �����̑��
	m_pCheckPoint->m_type = type;// �^�C�v

	// ������
	m_pCheckPoint->Init();

	return m_pCheckPoint;
}

//******************************
// ����������
//******************************
HRESULT CCheckPoint::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetPriority(OBJTYPE_CHECKPOINT);

#ifndef SET_MODE
	// �`�F�b�N�|�C���g�̓ǂݍ���
	LoadCheckpoint(m_type);

#endif // !SET_MODE

	return S_OK;
}

//******************************
// �I������
//******************************
void CCheckPoint::Uninit(void)
{
	Release();
}

//******************************
// �X�V����
//******************************
void CCheckPoint::Update(void)
{
	
#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_pointPos[m_nNumPoint] = CGame::GetPlayer(0)->GetPos();
		if (m_type == CCourse::COURSE_EASY)
		{
			m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS_EASY);
		}
		else
		{
			m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS_NORMAL);
		}
		m_nNumPoint++;
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE))
	{
		m_nNumPoint--;
		m_pCollision[m_nNumPoint]->Uninit();
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveCheckpoint();

		CDebugLog::Init();
		CDebugLog::Print("save");
	}
#else
	// �v���C���[�������[�v
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// �v���C���[���̎擾
		CPlayer*pPlayer = CGame::GetPlayer(nCntPlayer);
		if (pPlayer != NULL)
		{
			// �`�F�b�N�|�C���g�������[�v
			for (int nCntPoint = 0; nCntPoint < m_nNumPoint; nCntPoint++)
			{
				// �`���b�N�|�C���g�ԍ��̔�r
				if (pPlayer->GetCheckNum() == nCntPoint)
				{
					// �`���b�N�|�C���g�Ƃ̓����蔻��
					if (CCollision::CollisionSphere(pPlayer->GetCollision(), m_pCollision[nCntPoint]))
					{
						// �`�F�b�N�|�C���g�𑝂₷
						pPlayer->AddCheckNum();

						// �S�[������
						if (pPlayer->GetCheckNum() >= m_nNumPoint)
						{
							pPlayer->Goal();
							CDebugLog::Init();
							CDebugLog::Print("Goal");
						}
					}
				}
			}
		}
	}
	

#endif
}

//******************************
// �`�揈��
//******************************
void CCheckPoint::Draw(void)
{
}

//******************************
// �Z�[�u
//******************************
void CCheckPoint::SaveCheckpoint(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	switch (m_type)
	{
	case CCourse::COURSE_EASY:
		pFile = fopen(CHECKPOINT_TEXT_EASY, "w");
		break;
	case CCourse::COURSE_NORMAL:
		pFile = fopen(CHECKPOINT_TEXT_NORMAL, "w");
		break;
	case CCourse::COURSE_HARD:
		pFile = fopen(CHECKPOINT_TEXT_HARD, "w");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fprintf(pFile, "%d\n", m_nNumPoint);

		// ���W�̏�������
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_pointPos[nCnt].x, m_pointPos[nCnt].y, m_pointPos[nCnt].z);
		}
		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}

//******************************
// ���[�h
//******************************
void CCheckPoint::LoadCheckpoint(CCourse::COURSETYPE type)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	switch (type)
	{
	case CCourse::COURSE_EASY:
		pFile = fopen(CHECKPOINT_TEXT_EASY, "r");
		break;
	case CCourse::COURSE_NORMAL:
		pFile = fopen(CHECKPOINT_TEXT_NORMAL, "r");
		break;
	case CCourse::COURSE_HARD:
		pFile = fopen(CHECKPOINT_TEXT_HARD, "r");
		break;
	default:
		pFile = NULL;
		break;
	}
	
	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fscanf(pFile, "%d", &m_nNumPoint);

		// �`�F�b�N�|�C���g�������[�v
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// ���W�̓ǂݍ���
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);

			// �R���W�����̐���
			if (m_type == CCourse::COURSE_EASY)
			{
				m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS_EASY);
			}
			else
			{
				m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS_NORMAL);
			}
		}

		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}
