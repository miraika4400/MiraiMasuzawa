////////////////////////////////////////////////////
//
//    collison�N���X�̏���[collison.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************


//******************************
// �R���X�g���N�^
//******************************
CCollision::CCollision() :CModel(OBJTYPE_COLLISION)
{
	// �ϐ��̃N���A
	m_pMeshModel = NULL;   	//���b�V�����ւ̃|�C���^
	m_pBuffMatModel = NULL;	//�}�e���A�����ւ̃|�C���^
	m_nNumMatModel = 0;	    //�}�e���A�����̐�
}

//******************************
// �f�X�g���N�^
//******************************
CCollision::~CCollision()
{
}

//******************************
// �N���G�C�g(��)
//******************************
CCollision * CCollision::CreateSphere(D3DXVECTOR3 pos, float fRadius)
{
	// �������̊m��
	CCollision *pCollision;
	pCollision = new CCollision;

	// init�Ŏg�������ɑ��
	pCollision->m_type = COLLISIONTYPE_SPHERE;
	pCollision->m_fRadius = fRadius;

	// ������
	pCollision->Init();

	// �e�l�̑���E�Z�b�g
	pCollision->SetPos(pos);
	pCollision->SetPriority(OBJTYPE_COLLISION); // �I�u�W�F�N�g�^�C�v

	return pCollision;
}

//******************************
// �N���G�C�g(BOX)
//******************************
CCollision * CCollision::CreateBox(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �������̊m��
	CCollision *pCollision;
	pCollision = new CCollision;

	// init�Ŏg�������ɑ��
	pCollision->m_type = COLLISIONTYPE_BOX;
	pCollision->m_size = size;

	// ������
	pCollision->Init();

	// �e�l�̑���E�Z�b�g
	pCollision->SetPos(pos);
	pCollision->SetPriority(OBJTYPE_COLLISION); // �I�u�W�F�N�g�^�C�v

	return pCollision;
}

//******************************
// ���Ƌ��̓����蔻��
//******************************
bool CCollision::CollisionSphere(CCollision * pCollision1, CCollision * pCollision2)
{
	if (pow(pCollision1->GetPos().x - pCollision2->GetPos().x, 2) +
		pow(pCollision1->GetPos().y - pCollision2->GetPos().y, 2) +
		pow(pCollision1->GetPos().z - pCollision2->GetPos().z, 2) <= pow(pCollision1->m_fRadius + pCollision2->m_fRadius, 2))
	{

		return true;
	}
	else
	{
		return false;
	}
}

//******************************
// BOX��BOX�̓����蔻��
//******************************
bool CCollision::CollisionBox(CCollision * pCollision1, CCollision * pCollision2)
{
	return false;
}

//******************************
// ����BOX�̓����蔻��
//******************************
bool CCollision::CollisionSphereToBox(CCollision *pCollSphere, CCollision * pCollBox)
{
	// ���̍��W
	D3DXVECTOR3 spherePos = pCollSphere->GetPos();
	// BOX�̍��W
	D3DXVECTOR3 boxPos = pCollBox->GetPos();
	D3DXVECTOR3 boxSize = pCollBox->m_size;

	// �{�b�N�X���狅�̃{�b�N�X���̍ŒZ�ʒu�̎擾
	D3DXVECTOR3 shortrectPos;
	shortrectPos.x = OnRange(spherePos.x, boxPos.x - boxSize.x / 2, boxPos.x + boxSize.x / 2);
	shortrectPos.y = OnRange(spherePos.y, boxPos.y - boxSize.y / 2, boxPos.y + boxSize.y / 2);
	shortrectPos.z = OnRange(spherePos.z, boxPos.z - boxSize.z / 2, boxPos.z + boxSize.z / 2);

	float fDistance = sqrtf(powf(shortrectPos.x - spherePos.x, 2) +
		powf(shortrectPos.y - spherePos.y, 2) +
		powf(shortrectPos.z - spherePos.z, 2));

	// �������Ă��邩�̔���
	if (fDistance < pCollSphere->m_fRadius)
	{
		return true;
	}

	return false;
}

//******************************
// �͈͓�������
//******************************]
float CCollision::OnRange(float fPoint, float fMin, float fMax)
{

	if (fPoint < fMin)
	{
		return fMin;
	}
	else if (fPoint > fMax)
	{
		return fMax;
	}
	else
	{
		return fPoint;
	}
}


//******************************
// ����������
//******************************
HRESULT CCollision::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}
#ifdef _DEBUG

	CreateMesh();
	// ���f�����蓖��
	BindModel(m_pMeshModel, m_pBuffMatModel, 1);

#endif

	return S_OK;
}

//******************************
// �I������
//******************************
void CCollision::Uninit(void)
{
	//���b�V���̔j��
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	//�}�e���A���̔j��
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CCollision::Update(void)
{
}

//******************************
// �`�揈��
//******************************
void CCollision::Draw(void)
{
#ifdef _DEBUG

	// �F�̐ݒ�
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	mat->MatD3D.Ambient  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	mat->MatD3D.Diffuse  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	mat->MatD3D.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	mat->MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���C���[�t���[���ŕ`��
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//�@�`��
	CModel::Draw();
	// ���C���[�t���[�������ǂ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#endif // _DEBUG
}

//******************************
// ���b�V������
//******************************
void CCollision::CreateMesh(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	switch (m_type)
	{
	case COLLISIONTYPE_BOX:
		D3DXCreateBox(pDevice, m_size.x, m_size.y, m_size.z, &m_pMeshModel, &m_pBuffMatModel);
		break;
	case COLLISIONTYPE_SPHERE:
		// ���̂̐���
		D3DXCreateSphere(pDevice, m_fRadius, 8, 8, &m_pMeshModel, &m_pBuffMatModel);
		break;
	default:
		break;
	}
}
