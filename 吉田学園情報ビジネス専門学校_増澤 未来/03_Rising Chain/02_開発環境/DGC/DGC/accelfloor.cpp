////////////////////////////////////////////////////
//
//    Item�N���X�̏���[Item.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "accelfloor.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "destination.h"
#include "scene3d.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH   "./data/Models/item/Ring.x"     // ���f���̃p�X
#define TEXTURE_PATH "./data/Textures/Speed_Up.png"  //�e�N�X�`���̃p�X
#define RADIUS       550                             // ���a
#define SIZE D3DXVECTOR3(1.8f,1.8f,1.8f)
#define PANEL_SIZE   D3DXVECTOR3(300.0f,0.0,-600.0f) // ���̃T�C�Y
#define SCROLL_SPEED 0.01f                           // �e�N�X�`���̃X�N���[�����x
#define HIT_FLAG_INIT_COUNT 30                       // �q�b�g�t���O������
#define NUM_CHAIN 5                                  // �`�F�C����

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CAccelFloor::m_model = {};         //���b�V�����ւ̃|�C���^
LPDIRECT3DTEXTURE9 CAccelFloor::m_pTexture = NULL; // �e�N�X�`��

//******************************
// �R���X�g���N�^
//******************************
CAccelFloor::CAccelFloor() :CModel(OBJTYPE_ACCELFLOOR)
{
	// �ϐ��̃N���A
	m_pCollision = NULL; // �����蔻��
	m_pScene3d = NULL;   // ���|���S��
	m_fScroll = 0.0f;    // ���e�N�X�`���X�N���[���p�ϐ�
	m_bAdjust = true;    // �ʒu�����t���O
	memset(&m_bHit, 0, sizeof(m_bHit)); // �q�b�g���̃t���O
	memset(&m_nCntHit, 0, sizeof(m_nCntHit)); // �q�b�g�t���O�J�E���g
}

//******************************
// �f�X�g���N�^
//******************************
CAccelFloor::~CAccelFloor()
{
}

//******************************
// �N���G�C�g
//******************************
CAccelFloor * CAccelFloor::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// �������̊m��
	CAccelFloor *pItem;
	pItem = new CAccelFloor;

	// ������
	pItem->Init();

	// �e�l�̑���E�Z�b�g
	pItem->SetPriority(OBJTYPE_MAP); // �I�u�W�F�N�g�^�C�v
	pItem->SetPos(pos);              // ���W�̃Z�b�g
	pItem->SetRot(rot);              // ��]�̃Z�b�g

	// �����蔻��̐���
	pItem->m_pCollision = CCollision::CreateSphere(pos, RADIUS);
	return pItem;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CAccelFloor::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();


	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.nNumMat,
		&m_model.pMesh);

	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CAccelFloor::Unload(void)
{

	//���b�V���̔j��
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}

	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// ����������
//******************************
HRESULT CAccelFloor::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);
	// �e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}
	SetSize(SIZE);
	// ���̃p�l���̐���
	m_pScene3d = CScene3d::Create(D3DXVECTOR3(GetPos().x, GetPos().y - HOVER_RING, GetPos().z), PANEL_SIZE);
	// �v���C�I���e�B�̐ݒ�
	m_pScene3d->SetPriority(OBJTYPE_MAP);
	// �e�N�X�`���̊��蓖��
	m_pScene3d->BindTexture(m_pTexture);

	// ���e�N�X�`���X�N���[���p�ϐ��̏�����
	m_fScroll = 0.0f;
	// �ʒu�����t���O�̏�����
	m_bAdjust = true;
	// �q�b�g���̃t���O�̏�����
	memset(&m_bHit, 0, sizeof(m_bHit));
	// �q�b�g�t���O�J�E���g�̏�����
	memset(&m_nCntHit, 0, sizeof(m_nCntHit));

	return S_OK;
}

//******************************
// �I������
//******************************
void CAccelFloor::Uninit(void)
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
void CAccelFloor::Update(void)
{
	if (m_bAdjust)
	{
		// �����̒���
		AdjustPos();
		// ���ƃT�[�N���̉�]�����킹��
		D3DXVECTOR3 rot = GetRot();
		m_pScene3d->SetRot(D3DXVECTOR3(0.0f, rot.y, 0.0f));

		m_bAdjust = false;
		
	}
	CollisionPlayer();
	// ��]������
	D3DXVECTOR3 addRot = D3DXVECTOR3(0.0f, 0.0f, 0.03f);
	SetRot(GetRot() + addRot);

	if (m_pScene3d != NULL)
	{
		// ���̃e�N�X�`�����X�N���[��������
		m_fScroll += SCROLL_SPEED;
		D3DXVECTOR2 uv[NUM_VERTEX];
		// UV���W�̐ݒ�
		uv[0] = D3DXVECTOR2(0.0f, 0.0f + m_fScroll);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f + m_fScroll);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f + m_fScroll);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f + m_fScroll);

		// UV���W�̃Z�b�g
		m_pScene3d->SetTextureUV(uv);
		// ���W
		m_pScene3d->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y - HOVER_RING, GetPos().z));
	}
}

//******************************
// �`�揈��
//******************************
void CAccelFloor::Draw(void)
{
	CModel::Draw();
}

//******************************
// �v���C���[�Ƃ̓����蔻��
//******************************
void CAccelFloor::CollisionPlayer(void)
{
	// �v���C���[���̎擾
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);

	while (pPlayer != NULL)
	{
		if (!pPlayer->GetGoalFlag())
		{// �S�[����Ԃ���Ȃ��Ƃ�

			// �v���C���[�ԍ��̎擾
			int nPlayerNum = pPlayer->GetPlayerNum();

			if (!m_bHit[nPlayerNum])
			{// �q�b�g�t���O�������ĂȂ��Ƃ�

				// �����蔻��
				if (CCollision::CollisionSphere(m_pCollision, pPlayer->GetCollision()))
				{// �������Ă���
					// ������Ԃɂ���
					pPlayer->SetAccelerationFrag(true);
					for (int nCnt = 0; nCnt < NUM_CHAIN; nCnt++)
					{
						pPlayer->AddChainNum();
					}
					// �q�b�g�t���O
					m_bHit[nPlayerNum] = true;
					// �J�E���g�̏�����
					m_nCntHit[nPlayerNum] = 0;
				}
			}
			else
			{// �q�b�g�t���O�������Ă���Ƃ�

				// �J�E���g��i�߂�
				m_nCntHit[nPlayerNum]++;
				// ���̃J�E���g�ɓ��B������
				if (m_nCntHit[nPlayerNum] >= HIT_FLAG_INIT_COUNT)
				{
					// �J�E���g�̏�����
					m_nCntHit[nPlayerNum] = 0;
					// �q�b�g�t���O�̏�����
					m_bHit[nPlayerNum] = false;
				}
			}
		}
		// �l�N�X�g�̃|�C���^�ɍX�V
		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
	int num = 0;
}

//******************************
// �ʒu����
//******************************
void CAccelFloor::AdjustPos(void)
{
	// �R�[�X���̏���
	CCourse*pCourse = CGame::GetCourse();

	BOOL bHit = FALSE;
	float fDistance = 0.0f;
	// NULL�`�F�b�N
	if (pCourse != NULL)
	{
		// ���C
		D3DXIntersect(pCourse->GetMesh(),
			&GetPos(),
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
			D3DXVECTOR3 pos = GetPos();
			// �v���C���[�̍��W�̍X�V
			pos.y = (pos.y - fDistance) + HOVER_RING;

			SetPos(pos);
			m_pCollision->SetPos(pos);
		}
	}
}
