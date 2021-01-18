////////////////////////////////////////////////////
//
//    course�N���X�̏���[course.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"
#include "destination.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH_EASY   "./data/Models/Course_model/Syokyuu_Course_Wall.x"    //���f���̃p�X
#define MODEL_PATH_NORMAL "./data/Models/Course_model/CourseLv2_Wall.x"    //���f���̃p�X
#define MODEL_PATH_HARD   "./data/Models/Course_model/"    //���f���̃p�X
#define HOVER_HEIGHT 50

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
LPD3DXMESH   CWall::m_pMeshModel[CCourse::COURSE_MAX] = {};   	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CWall::m_pBuffMatModel[CCourse::COURSE_MAX] = {};	//�}�e���A�����ւ̃|�C���^
DWORD        CWall::m_nNumMatModel[CCourse::COURSE_MAX] = {};	    //�}�e���A�����̐�
																	// �e�N�X�`���̃p�X�̊i�[
char *CWall::m_pTexPath[CCourse::COURSE_MAX] =
{
	MODEL_PATH_EASY,
	MODEL_PATH_NORMAL,
	MODEL_PATH_HARD
};
//******************************
// �R���X�g���N�^
//******************************
CWall::CWall() :CModel(OBJTYPE_MAP)
{
}

//******************************
// �f�X�g���N�^
//******************************
CWall::~CWall()
{
}

//******************************
// �N���G�C�g
//******************************
CWall * CWall::Create(CCourse::COURSETYPE type)
{
	// �������̊m��
	CWall *pWall;
	pWall = new CWall;
	// �^�C�v�̐ݒ�
	pWall->m_courseType = type;
	// ������
	pWall->Init();

	// �e�l�̑���E�Z�b�g
	pWall->SetPriority(OBJTYPE_MAP); // �I�u�W�F�N�g�^�C�v

	return pWall;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CWall::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �R�[�X�������[�v
	for (int nCntCourse = 0; nCntCourse < CCourse::COURSE_MAX; nCntCourse++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(m_pTexPath[nCntCourse],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMatModel[nCntCourse],
			NULL,
			&m_nNumMatModel[nCntCourse],
			&m_pMeshModel[nCntCourse]);

	}

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CWall::Unload(void)
{
	for (int nCnt = 0; nCnt < CCourse::COURSE_MAX; nCnt++)
	{
		//���b�V���̔j��
		if (m_pMeshModel[nCnt] != NULL)
		{
			m_pMeshModel[nCnt]->Release();
			m_pMeshModel[nCnt] = NULL;
		}

		//�}�e���A���̔j��
		if (m_pBuffMatModel[nCnt] != NULL)
		{
			m_pBuffMatModel[nCnt]->Release();
			m_pBuffMatModel[nCnt] = NULL;
		}
	}
}


//******************************
// ����������
//******************************
HRESULT CWall::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_pMeshModel[m_courseType], m_pBuffMatModel[m_courseType], m_nNumMatModel[m_courseType]);
	// �`�F�b�N�|�C���g�̐���
	CCheckPoint::Create(m_courseType);
	return S_OK;
}

//******************************
// �I������
//******************************
void CWall::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CWall::Update(void)
{
	// �����蔻��
	CollisionPlayer();
}

//******************************
// �`�揈��
//******************************
void CWall::Draw(void)
{
#ifdef _DEBUG

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���C���[�t���[���ŕ`��
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//�@�`��
	CModel::Draw();
	// ���C���[�t���[�������ǂ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

#endif // _DEBUG
}

//******************************
// �R�[�X���̓����蔻��
//******************************
void CWall::CollisionPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// �v���C���[���W�̎擾
		D3DXVECTOR3 playerPos = CGame::GetPlayer(nCntPlayer)->GetPos();

		BOOL bHit = FALSE;     // ���C���������Ă��邩
		float fDistance = 0.0f;// ���C���������Ă��鋗��

		// ���C���o��������
		const int c_nRayNum = 24;
		for (int nCntRay = 0; nCntRay < c_nRayNum; nCntRay++)
		{
			// ���C���o�������̐ݒ�
			float fDir = D3DXToRadian(360 / c_nRayNum);
			D3DXVECTOR3 rayDir;
			rayDir.x = cosf(fDir*nCntRay);
			rayDir.y = 0;
			rayDir.z = sinf(fDir*nCntRay);

			// ���C
			D3DXIntersect(m_pMeshModel[m_courseType],
				&playerPos,
				&rayDir,
				&bHit,
				NULL,
				NULL,
				NULL,
				&fDistance,
				NULL,
				NULL);

			// �������Ă���Ƃ�
			if(bHit)
			{
				// �v���C���[�̔��a
				float fRadPlayer = (float)CGame::GetPlayer(nCntPlayer)->GetMaxSpeed()*2.5f;//CGame::GetPlayer(nCntPlayer)->GetCollision()->GetCollisionRadius();

				if (fDistance < fRadPlayer)
				{// ���C�̒������v���C���[�̔��a�ȉ��̎�
					playerPos.x = (playerPos.x + cosf(fDir*nCntRay) * fDistance) - (cosf(fDir*nCntRay) * fRadPlayer);
					playerPos.z = (playerPos.z + sinf(fDir*nCntRay) * fDistance) - (sinf(fDir*nCntRay) * fRadPlayer);

					// �v���C���[�̈ʒu�̐ݒ�
					CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);
					// �v���C���[�̃R���W�����̈ʒu�̐ݒ�
					CGame::GetPlayer(nCntPlayer)->GetCollision()->SetPos(playerPos);

					// �ړ��ڕW�̈ʒu�ݒ�

					// �ړ��ڕW�̎擾
					CDestination*pDest=(CDestination*)GetTop(OBJTYPE_DESTINATION);
					while (pDest != NULL)
					{
						if (pDest->GetPlayerNum() == CGame::GetPlayer(nCntPlayer)->GetPlayerNum())
						{// �v���C���[�ԍ�����v���Ă���Ƃ�
							D3DXVECTOR3 destPos = pDest->GetPos();
							destPos.x = playerPos.x + cosf(-CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90)) * -pDest->GetDistancePlayer();
							destPos.z = playerPos.z + sinf(-CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90)) * -pDest->GetDistancePlayer();
							pDest->SetPos(destPos);
							break;
						}
						pDest = (CDestination*)pDest->GetNext();
					}
					//break;
				}
			}
		}
	}
}
