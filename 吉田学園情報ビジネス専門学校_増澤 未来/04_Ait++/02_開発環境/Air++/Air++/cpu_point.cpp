////////////////////////////////////////////////////
//
//    cpu_point�N���X�̏���[cpu_point.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "cpu_point.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "character.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************
#define CHECKPOINT_RADIUS 700                   // �����蔻��̔��a
#define CPUPOINT_TEXT "data/Texts/CpuPoint.txt" // �`�F�b�N�|�C���g�e�L�X�g�̃p�X
#define GOAL_LAP_NUM 3                          // ���T�ŃS�[����
#define POINT_SET_INTERVAL 40                   // �|�C���g�Z�b�g�C���^�[�o�[��

// �`�F�b�N�|�C���g�z�u���[�h�؂�ւ��}�N��
#define SET_MODE
#undef  SET_MODE

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************

//******************************
// �R���X�g���N�^
//******************************
CCpuPoint::CCpuPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// �ϐ��̃N���A
	m_nNumPoint = 0;
	m_nCntSet = 0;
	m_nCpuPointNum = 0;
	memset(&m_CpuPoint, 0, sizeof(m_CpuPoint));
}

//******************************
// �f�X�g���N�^
//******************************
CCpuPoint::~CCpuPoint()
{
}

//******************************
// �N���G�C�g
//******************************
CCpuPoint * CCpuPoint::Create(void)
{
	// �������̊m��
	CCpuPoint* pCpuPoint = new CCpuPoint;

	// ������
	pCpuPoint->Init();

	return pCpuPoint;
}

//******************************
// ����������
//******************************
HRESULT CCpuPoint::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetPriority(OBJTYPE_CHECKPOINT);

	m_nCntSet = 0;

#ifndef SET_MODE

	// �`�F�b�N�|�C���g�̓ǂݍ���
	LoadPoint();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// �I������
//******************************
void CCpuPoint::Uninit(void)
{
	Release();
}

//******************************
// �X�V����
//******************************
void CCpuPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD5))
	{// �R�[�i�[����Ȃ��|�C���g�̐ݒu
		// ���W
		m_CpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// ����
		m_CpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// �C���R�[�X����
		m_CpuPoint[m_nNumPoint].inType = POINT_TYPE_NORMAL;
		// ���f���̐���
		CModel::Create(m_CpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_CpuPoint[m_nNumPoint].fAngle, 0.0f));
		// �|�C���g���̉��Z
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD4))
	{// �������C���R�[�X�̃|�C���g�ݒu
		// ���W
		m_CpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// ����
		m_CpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// �C���R�[�X����
		m_CpuPoint[m_nNumPoint].inType = POINT_TYPE_IN_LEFT;
		// ���f���̐���
		CModel::Create(m_CpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_CpuPoint[m_nNumPoint].fAngle, 0.0f));

		// �C���R�[�X�̖ڈ���W
		D3DXVECTOR3 inPos = m_CpuPoint[m_nNumPoint].pos;
		inPos.x = inPos.x + cosf(-m_CpuPoint[m_nNumPoint].fAngle) * 400;
		inPos.y = inPos.y;
		inPos.z = inPos.z + sinf(-m_CpuPoint[m_nNumPoint].fAngle) * 400;

		// ���f���̐���
		CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);

		// �|�C���g���̉��Z
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_NUMPAD6))
	{// �E�����C���R�[�X�̃|�C���g�ݒu
		// ���W
		m_CpuPoint[m_nNumPoint].pos = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		// ����
		m_CpuPoint[m_nNumPoint].fAngle = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetRot().y - D3DXToRadian(180.0f);
		// �C���R�[�X����
		m_CpuPoint[m_nNumPoint].inType = POINT_TYPE_IN_RIGHT;
		// ���f���̐���
		CModel::Create(m_CpuPoint[m_nNumPoint].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_CpuPoint[m_nNumPoint].fAngle, 0.0f));

		// �C���R�[�X�̖ڈ���W
		D3DXVECTOR3 inPos = m_CpuPoint[m_nNumPoint].pos;
		inPos.x = inPos.x + cosf(-m_CpuPoint[m_nNumPoint].fAngle) * -400;
		inPos.y = inPos.y;
		inPos.z = inPos.z + sinf(-m_CpuPoint[m_nNumPoint].fAngle) * -400;

		// ���f���̐���
		CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);

		// �|�C���g���̉��Z
		m_nNumPoint++;
	}

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SavePoint();
	}
#else


#endif
}

//******************************
// �`�揈��
//******************************
void CCpuPoint::Draw(void)
{
}

//**********************************
// ��ԋ߂����W�̃C���f�b�N�X�̎擾
//**********************************
int CCpuPoint::GetNearPosIndex(D3DXVECTOR3 pos)
{
	// �z��0�Ƃ̋����𑪂�
	float fDistance = D3DXVec3Length(&(m_CpuPoint[0].pos - pos));
	// return�p�z��̕ۑ�
	int nIndex = 0;

	// �߂��n�_�̌���
	for (int nCnt = 1; nCnt < m_nNumPoint; nCnt++)
	{
		// �ۑ����Ă��鋗�����߂��Ƃ�
		if (fDistance > D3DXVec3Length(&(m_CpuPoint[nCnt].pos - pos)))
		{
			// �����̍X�V
			fDistance = D3DXVec3Length(&(m_CpuPoint[nCnt].pos - pos));
			// �z��̕ۑ�
			nIndex = nCnt;
		}
	}

	return nIndex;
}

//******************************
// �Z�[�u
//******************************
void CCpuPoint::SavePoint(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(CPUPOINT_TEXT, "w");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fprintf(pFile, "%d\n", m_nNumPoint);

		// ���W�̏�������
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n",m_CpuPoint[nCnt].pos.x, m_CpuPoint[nCnt].pos.y, m_CpuPoint[nCnt].pos.z);
			fprintf(pFile, "%.2f\n", m_CpuPoint[nCnt].fAngle);
			fprintf(pFile , "%d\n", (int)m_CpuPoint[nCnt].inType);
		}
		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}

//******************************
// ���[�h
//******************************
void CCpuPoint::LoadPoint(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(CPUPOINT_TEXT, "r");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̓ǂݍ���
		fscanf(pFile, "%d", &m_nNumPoint);

		// �`�F�b�N�|�C���g�������[�v
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// ���W�̓ǂݍ���
			fscanf(pFile, "%f,%f,%f\n", &m_CpuPoint[nCnt].pos.x, &m_CpuPoint[nCnt].pos.y, &m_CpuPoint[nCnt].pos.z);
			fscanf(pFile, "%f\n", &m_CpuPoint[nCnt].fAngle);
			fscanf(pFile, "%d\n", &m_CpuPoint[nCnt].inType);
#ifdef _DEBUG

			CModel::Create(m_CpuPoint[nCnt].pos, CModel::GENERAL_MODEL_CONE)->SetRot(D3DXVECTOR3(0.0f, m_CpuPoint[nCnt].fAngle, 0.0f));

			if (m_CpuPoint[nCnt].inType == POINT_TYPE_IN_LEFT)
			{
				// �C���R�[�X�̖ڈ���W
				D3DXVECTOR3 inPos = m_CpuPoint[nCnt].pos;
				inPos.x = inPos.x + cosf(-m_CpuPoint[nCnt].fAngle) * 400;
				inPos.y = inPos.y;
				inPos.z = inPos.z + sinf(-m_CpuPoint[nCnt].fAngle) * 400;

				// ���f���̐���
				CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);
			}
			else if (m_CpuPoint[nCnt].inType == POINT_TYPE_IN_RIGHT)
			{
				// �C���R�[�X�̖ڈ���W
				D3DXVECTOR3 inPos = m_CpuPoint[nCnt].pos;
				inPos.x = inPos.x + cosf(-m_CpuPoint[nCnt].fAngle) * -400;
				inPos.y = inPos.y;
				inPos.z = inPos.z + sinf(-m_CpuPoint[nCnt].fAngle) * -400;

				// ���f���̐���
				CModel::Create(inPos, CModel::GENERAL_MODEL_SPHERE);
			}
#endif // _DEBUG
		}

		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}
