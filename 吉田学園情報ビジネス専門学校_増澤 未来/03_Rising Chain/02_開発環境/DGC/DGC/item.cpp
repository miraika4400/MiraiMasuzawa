////////////////////////////////////////////////////
//
//    Item�N���X�̏���[Item.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "Item.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "destination.h"
//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH_1         "./data/Models/item/item_Red.x"	// �ԃ��f���̃p�X
#define MODEL_PATH_2         "./data/Models/item/item_Blue.x"	// ���f���̃p�X
#define MODEL_PATH_3         "./data/Models/item/item_Yellow.x"	// ���F���f���̃p�X
#define MODEL_PATH_4         "./data/Models/item/item_Green.x"	// �΃��f���̃p�X
#define MODEL_PATH_COLORLESS "./data/Models/item/item1.x"	    //"./data/Models/player/Player1.x" // 	// ���F���f���̃p�X

#define ITEM_RADIUS 120          // ���a
#define GET_COUNT   20           // �����A�C�e����������擾�\�܂ł̃t���[����
#define DROP_CIRCLE_SPEED 10.0f  // �~�`�Ƀh���b�v����Ƃ����ł����ړ���
#define DROP_DESTINATION  -10.0f // �h���b�v����Ƃ��������x�̖ڕW�l
#define DROP_RATE         0.05f  // �h���b�v����Ƃ��������x�̌W��

#define SIZE D3DXVECTOR3(1.2f,1.2f,1.2f)

#ifdef _DEBUG
#endif

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CItem::m_model[CItem::ITEM_MAX] = {};   	//���b�V�����ւ̃|�C���^
D3DXVECTOR3 CItem::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �S�A�C�e�����ʂŃ��b�g�Œ�

char *CItem::m_pTexPath[CItem::ITEM_MAX] =
{
	
	MODEL_PATH_1,
	MODEL_PATH_2,
	MODEL_PATH_3,
	MODEL_PATH_4,
	MODEL_PATH_COLORLESS
};

//******************************
// �R���X�g���N�^
//******************************
CItem::CItem() :CModel(OBJTYPE_ITEM)
{
	// �ϐ��̃N���A
	m_pCollision = NULL;         // �����蔻��
	m_itemType = ITEM_COLORLESS; // �A�C�e���^�C�v
	m_bDrop = false;             // ������������
	m_move = VEC3_ZERO;          // �h���b�v���̈ړ���
	m_nPlayerNum = -1;           // �h���b�v���ǂ̃v���C���[�����Ƃ�����
	m_nCntGet = 0;               // �����A�C�e���͍ŏ��̐��t���[���E���Ȃ��悤��
}

//******************************
// �f�X�g���N�^
//******************************
CItem::~CItem()
{
}

//******************************
// �N���G�C�g
//******************************
CItem * CItem::Create(const D3DXVECTOR3 pos, const ITEM_TYPE type)
{
	// �������̊m��
	CItem *pItem;
	pItem = new CItem;
	
	// �^�C�v�̐ݒ�
	pItem->m_itemType = type;
	// ������
	pItem->Init();

	// �e�l�̑���E�Z�b�g
	pItem->SetPriority(OBJTYPE_ITEM); // �I�u�W�F�N�g�^�C�v
	pItem->SetPos(pos);             // ���W�̃Z�b�g

	// �����蔻��̐���
	pItem->m_pCollision=CCollision::CreateSphere(pos, ITEM_RADIUS);

	return pItem;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CItem::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �R�[�X�������[�v
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(m_pTexPath[nCntItem],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_model[nCntItem].pBuffMat,
			NULL,
			&m_model[nCntItem].nNumMat,
			&m_model[nCntItem].pMesh);

		if (m_model[nCntItem].nNumMat != 0)
		{
			D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model[nCntItem].pBuffMat->GetBufferPointer();
			for (int nCnt = 0; nCnt < (int)m_model[nCntItem].nNumMat; nCnt++)
			{
				if (pMat[nCnt].pTextureFilename != NULL)
				{
					char cPath[64] = {};
					sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
					// �e�N�X�`���̐���
					D3DXCreateTextureFromFile(pDevice, cPath, &m_model[nCntItem].apTexture[nCnt]);
				}
			}
		}
	}

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CItem::Unload(void)
{
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		//���b�V���̔j��
		if (m_model[nCnt].pMesh != NULL)
		{
			m_model[nCnt].pMesh->Release();
			m_model[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_model[nCnt].pBuffMat != NULL)
		{
			m_model[nCnt].pBuffMat->Release();
			m_model[nCnt].pBuffMat = NULL;
		}
	}
}

//******************************
// ��]����
//******************************
void CItem::ItemRotasion(void)
{
	m_rot.y += D3DXToRadian(3);
}

//******************************
// �h���b�v
//******************************
void CItem::DropItem(const D3DXVECTOR3 pos,  const int nPlayerNum)
{
	// �����A�C�e���̐���
	CItem*pItem = NULL;
	pItem = Create(pos, ITEM_COLORLESS);

	// �h���b�v��Ԃɂ���
	pItem->m_bDrop = true;
	// �v���C���[�ԍ��̐ݒ�
	pItem->m_nPlayerNum = nPlayerNum;
}

//******************************
// �h���b�v
//******************************
void CItem::DropItemCircle(const D3DXVECTOR3 pos, const int nNumDrop, const int nPlayerNum)
{
	// �h���b�v���鐔���[�v
	for (int nCnt = 0; nCnt < nNumDrop; nCnt++)
	{
		// �����A�C�e���̐���
		CItem*pItem = NULL;
		pItem = Create(pos, ITEM_COLORLESS);

		if (pItem != NULL)
		{
			// �����_���Ȋp�x
			float fRandAngle = D3DXToRadian(rand() % 360);

			D3DXVECTOR3 circlePos = pos;
			// �ړ��ʂ̐ݒ�
			circlePos.x += cosf(fRandAngle)*300;
			circlePos.z += sinf(fRandAngle)*300;

			pItem->SetPos(circlePos);
			// �v���C���[�ԍ��̐ݒ�
			pItem->m_nPlayerNum = nPlayerNum;
			// �h���b�v��Ԃɂ���
			pItem->m_bDrop = true;
		}
	}
}

//******************************
// ����������
//******************************
HRESULT CItem::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_model[m_itemType].pMesh, m_model[m_itemType].pBuffMat, m_model[m_itemType].nNumMat);

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_model[m_itemType].nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model[m_itemType].apTexture[nCnt]);
	}

	// �J�E���g�̏�����
	if (m_itemType == ITEM_COLORLESS)
	{// �����A�C�e����
		m_nCntGet = GET_COUNT;
	}
	else
	{// �F�t���A�C�e��
		m_nCntGet = 0;
	}

	// �v���C���[�ԍ��̏�����
	m_nPlayerNum = -1;
	
	m_bDrop = true;
	// �T�C�Y�̐ݒ�
	SetSize(SIZE);
	return S_OK;
}

//******************************
// �I������
//******************************
void CItem::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CItem::Update(void)
{
	if (!m_bDrop)
	{// �h���b�v��Ԃ���Ȃ��Ƃ�

		if (m_nCntGet > 0)
		{// �J�E���g���c���Ă�Ƃ�

			m_nCntGet--;
		}

		// �v���C���[�Ƃ̓����蔻��
		CollisionPlayer();
	}
	else
	{
		Move();
	}
	if (m_pCollision != NULL&&m_pCollision->GetPos() != GetPos())
	{
		m_pCollision->SetPos(GetPos());
	}
}

//******************************
// �`�揈��
//******************************
void CItem::Draw(void)
{
	SetRot(m_rot);
	CModel::Draw();
}

//******************************
// �v���C���[�Ƃ̓����蔻��
//******************************
void CItem::CollisionPlayer(void)
{
	// �v���C���[���̎擾
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);

	while (pPlayer != NULL)
	{
		if (pPlayer->GetPlayerNum() != m_nPlayerNum || m_nCntGet <= 0)
		{
			// �����蔻��
			if (CCollision::CollisionSphere(m_pCollision, pPlayer->GetCollision()))
			{// �������Ă���
				if (pPlayer->GetPlayerNum() == m_itemType || m_itemType == ITEM_COLORLESS)
				{// �����̐F�������͖��F
					pPlayer->HitItem(true);
					Uninit();
				}
				else
				{// ���̑�
					pPlayer->HitItem(false);
					Uninit();
				}
				break;
			}
		}
		// �l�N�X�g�̃|�C���^�ɍX�V
		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
}

//******************************
// �ړ��̏���
//******************************
void CItem::Move(void)
{
	m_move.y += (DROP_DESTINATION - m_move.y)*DROP_RATE;

	// �ړ��ʂ����ݍ��W�ɑ����ăZ�b�g
	SetPos(GetPos() + m_move);
	m_pCollision->SetPos(GetPos());
}
