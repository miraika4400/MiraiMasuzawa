////////////////////////////////////////////////////
//
//    course�N���X�̏���[course.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "course.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "camera.h"
#include "destination.h"
#include "item.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH_EASY   "./data/Models/Course_model/Syokyuu_Course.x"     //���f���̃p�X
#define MODEL_PATH_NORMAL "./data/Models/Course_model/CourseLv2.x"          //���f���̃p�X
#define MODEL_PATH_HARD   "./data/Models/Course_model/CourseLv2.x"          //���f���̃p�X
#define ADJUST_HEIGHT 30  // �v���C���[��肿����Əォ�烌�C���o��
#define HOVER_HEIGHT 50   // �v���C���[���ǂꂭ�炢�����Ă��邩
#define HOVER_HEIGHT_ITEM 150   // �A�C�e�����ǂꂭ�炢�����Ă��邩

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
LPD3DXMESH   CCourse::m_pMeshModel[CCourse::COURSE_MAX] = {};   	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CCourse::m_pBuffMatModel[CCourse::COURSE_MAX] = {};	//�}�e���A�����ւ̃|�C���^
DWORD        CCourse::m_nNumMatModel[CCourse::COURSE_MAX] = {};	    //�}�e���A�����̐�
LPDIRECT3DTEXTURE9 CCourse::m_apTexture[MAX_TEXTURE] = {}; // �e�N�X�`��

// �e�N�X�`���̃p�X�̊i�[
char *CCourse::m_pTexPath[CCourse::COURSE_MAX] =
{
	MODEL_PATH_EASY,
	MODEL_PATH_NORMAL,
	MODEL_PATH_HARD
};
//******************************
// �R���X�g���N�^
//******************************
CCourse::CCourse():CModel(OBJTYPE_MAP)
{
	m_nCntInit = 0; // ����������Ă���̃t���[����
}

//******************************
// �f�X�g���N�^
//******************************
CCourse::~CCourse()
{
}

//******************************
// �N���G�C�g
//******************************
CCourse * CCourse::Create(COURSETYPE type)
{
	// �������̊m��
	CCourse *pMap;
	pMap = new CCourse;
	// �^�C�v�̐ݒ�
	pMap->m_courseType = type;
	// ������
	pMap->Init();

	// �e�l�̑���E�Z�b�g
	pMap->SetPriority(OBJTYPE_MAP); // �I�u�W�F�N�g�^�C�v
	
	return pMap;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CCourse::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// �R�[�X�������[�v
	for (int nCntCourse = 0; nCntCourse < COURSE_MAX; nCntCourse++)
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

		if (m_nNumMatModel != 0)
		{
			D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCntCourse]->GetBufferPointer();
			for (int nCnt = 0; nCnt < (int)m_nNumMatModel[nCntCourse]; nCnt++)
			{
				if (pMat[nCnt].pTextureFilename != NULL)
				{
					char cPath[256] = {};
					sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
					// �e�N�X�`���̐���
					D3DXCreateTextureFromFile(pDevice, cPath, &m_apTexture[nCnt]);
				}
			}
		}

	}

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CCourse::Unload(void)
{
	for (int nCnt = 0; nCnt < COURSE_MAX; nCnt++)
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
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//�}�e���A���̔j��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
		
	}
}


//******************************
// ����������
//******************************
HRESULT CCourse::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_nNumMatModel[m_courseType]; nCnt++)
	{
		BindTexture(nCnt, m_apTexture[nCnt]);
	}

	// ���f�����蓖��
	BindModel(m_pMeshModel[m_courseType],m_pBuffMatModel[m_courseType],m_nNumMatModel[m_courseType]);
	
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// �`�F�b�N�|�C���g�̐���
		CCheckPoint::Create(m_courseType);
		// �ǂ̐���
		CWall::Create(m_courseType);
		// �X�^�[�g�̐���
		CStart::Create(m_courseType);
	}

	m_nCntInit = 0;
	return S_OK;
}

//******************************
// �I������
//******************************
void CCourse::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CCourse::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		m_nCntInit++ ;
		// �����蔻��
		CollisionPlayer();
		//CollisionDestination();
		CollisionItem();
	}
}

//******************************
// �`�揈��
//******************************
void CCourse::Draw(void)
{
	//�f�o�C�X���̎擾
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//if (m_courseType == COURSE_NORMAL)
	//{
	//
	//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // �J�����O
	//}

	CModel::Draw();

	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // �J�����O
}

//******************************
// �R�[�X���̓����蔻��*�v���C���[
//******************************
void CCourse::CollisionPlayer(void)
{
	// �v���C���[�������[�v
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// �v���C���[���W�̎擾
		D3DXVECTOR3 playerPos = CGame::GetPlayer(nCntPlayer)->GetPos();
		// �v���C���[��肿����ƍ����ʒu����o��
		playerPos.y += ADJUST_HEIGHT;

		BOOL bHit = FALSE;     // ���C���������Ă��邩
		float fDistance = 0.0f;// ���C���������Ă��鋗��

		// ���C
		D3DXIntersect(m_pMeshModel[m_courseType],
			&playerPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{// ���C���������Ă�����

			if (fDistance <= HOVER_HEIGHT+ ADJUST_HEIGHT -1)
			{// ���ƃv���C���[�̋��������ȏ�ɕۂ�

				// �v���C���[�̍��W�̍X�V
				playerPos.y = (playerPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);

				if (CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
					// �d�͂�؂�
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT+ ADJUST_HEIGHT + 50)
			{// ������x�͈̔͂�������R�[�X�ɋz���t����

				// �v���C���[�̍��W�̍X�V
				playerPos.y = (playerPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);

				if (CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
				    // �d�͂�؂�
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(false);
				}
			}
			else
			{// �����痣�ꂷ���Ă���
				if (!CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// �d�̓t���O�������ĂȂ��Ƃ�
				 // �d�̓t���O�����Ă�
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(true);
				}
			}
		}
		else
		{// �����̉��ɏ����Ȃ�
			if (!CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
			{// �d�̓t���O�������ĂȂ��Ƃ�
			    // �d�̓t���O�����Ă�
				CGame::GetPlayer(nCntPlayer)->SetActiveGravity(true);
			}
		}

		// �n�`�̌����ƃv���C���[�̌��������킹��

		// �q�b�g�t���O�̏�����
		bHit = FALSE;
		// �v���C���[�̐i�s������������Əォ�烌�C���΂�
		D3DXVECTOR3 rayPos;
		rayPos.x = playerPos.x + ( cosf(CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90))) * 50;
		rayPos.y = playerPos.y + HOVER_HEIGHT + ADJUST_HEIGHT;
		rayPos.z = playerPos.z + (-sinf(CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90))) * 50;

		// ���C
		D3DXIntersect(m_pMeshModel[m_courseType],
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{
			// ���C�̓����������W�̎Z�o
			D3DXVECTOR3 hitPos;
			hitPos.x = rayPos.x;
			hitPos.y = rayPos.y - fDistance;
			hitPos.z = rayPos.z;

			// �v���C���[�̃��b�g�̎擾
			D3DXVECTOR3 playerRot = CGame::GetPlayer(nCntPlayer)->GetRot();

			float fRotZ = atan2f(playerPos.y - hitPos.y, 500);
			// �J�����̍����̒���
			if (!CGame::GetCamera(nCntPlayer)->GetBackMirror())
			{// �o�b�N�~���[��Ԃ���Ȃ��Ƃ�
				CGame::GetCamera(nCntPlayer)->SetPhiDist(atan2f(hitPos.y - playerPos.y, 300)*1.2f + D3DXToRadian(-65));
			}
			else
			{// �o�b�N�~���[��Ԃ̎�
				CGame::GetCamera(nCntPlayer)->SetPhiDist(atan2f(hitPos.y - playerPos.y, 300)*-1.2f + D3DXToRadian(-85));
			}
			// �v���C���[�̌���
			CGame::GetPlayer(nCntPlayer)->SetRot(D3DXVECTOR3(playerRot.x, playerRot.y, sinf(fRotZ)));
		}
	}
}

#if 0
//******************************
// �R�[�X���̓����蔻��*�ڕW
//******************************
void CCourse::CollisionDestination(void)
{
	// ���X�g�̐擪�̎擾
	CDestination*pDest = (CDestination*)GetTop(OBJTYPE_DESTINATION);

	while (pDest != NULL)
	{
		// �v���C���[���W�̎擾
		D3DXVECTOR3 destPos = pDest->GetPos();

		BOOL bHit = FALSE;     // ���C���������Ă��邩
		float fDistance = 0.0f;// ���C���������Ă��鋗��

							   // ���C
		D3DXIntersect(m_pMeshModel[m_courseType],
			&destPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{// ���C���������Ă�����

			if (fDistance <= HOVER_HEIGHT - 1)
			{// ���ƃv���C���[�̋��������ȏ�ɕۂ�

			 // �v���C���[�̍��W�̍X�V
				destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT;
				pDest->SetPos(destPos);

				if (pDest->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
				 // �d�͂�؂�
					pDest->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + 50)
			{// ������x�͈̔͂�������R�[�X�ɋz���t����

			 // �v���C���[�̍��W�̍X�V
				destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT;
				pDest->SetPos(destPos);

				if (pDest->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
				 // �d�͂�؂�
					pDest->SetActiveGravity(false);
				}
			}
			else
			{// �����痣�ꂷ���Ă���
				if (!pDest->GetActiveGravity())
				{// �d�̓t���O�������ĂȂ��Ƃ�
				 // �d�̓t���O�����Ă�
					pDest->SetActiveGravity(true);
				}
			}
		}
		else
		{// �����̉��ɏ����Ȃ�
			if (pDest->GetActiveGravity())
			{// �d�̓t���O�������ĂȂ��Ƃ�
			 // �d�̓t���O�����Ă�
				pDest->SetActiveGravity(false);
			}
		}
		// ���X�g�̎��̃|�C���^�ɂ���
		pDest = (CDestination*)pDest->GetNext();
	}
}
#endif

//******************************
// �R�[�X���̓����蔻��*�A�C�e��
//******************************
void CCourse::CollisionItem(void)
{
	// ���X�g�̐擪�̎擾
	CItem*pItem = (CItem*)GetTop(OBJTYPE_ITEM);

	while (pItem != NULL)
	{
		if (pItem->GetDropFlag())
		{
			// ���W�̎擾
			D3DXVECTOR3 destPos = pItem->GetPos();

			BOOL bHit = FALSE;     // ���C���������Ă��邩
			float fDistance = 0.0f;// ���C���������Ă��鋗��

								   // ���C
			D3DXIntersect(m_pMeshModel[m_courseType],
				&destPos,
				&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
				&bHit,
				NULL,
				NULL,
				NULL,
				&fDistance,
				NULL,
				NULL);

			if (bHit)
			{// ���C���������Ă�����

				if (fDistance <= HOVER_HEIGHT_ITEM - 1 || m_nCntInit <= 200)
				{// ���ƃv���������ȏ�ɕۂ�

					// ���W�̍X�V
					destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT_ITEM;
					pItem->SetPos(destPos);

					pItem->SetDropFlag(false);
				}

			}
			else
			{

			}
		}
		// ���X�g�̎��̃|�C���^�ɂ���
		pItem = (CItem*)pItem->GetNext();
	}
}