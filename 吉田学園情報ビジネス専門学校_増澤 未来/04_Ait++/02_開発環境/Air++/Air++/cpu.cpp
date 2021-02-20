//=============================================================================
//
// player�w�b�_ [player.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "cpu.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "collision.h"
#include "cpu_point.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH_STAR "./data/Models/cpu_star.x" // ���f���̃p�X*���[�v�X�^�[
#define MODEL_PATH_RUIN "./data/Models/cpu_ruin.x" // ���f���̃p�X*���C���X�^�[
#define SHADER_PATH     "./data/HLSL/Shader.fx"    // HLSL�t�@�C���̃p�X
#define CPU_DIST_RADIUS 150.0f                     // �ڕW�̒n�_���a
#define DIST_RAND_AMPLITUDE 800                    // �ڕW�n�_�ݒ�̗����̐U��
#define CPU_LEVEL_MAX 10                           // CPU�������x���̏��

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model  CCpu::m_aModel[CCpu::CPU_TYPE_MAX] = {};  //���b�V�����ւ̃|�C���^
CScene::Shader CCpu::m_shader = {};                      // �V�F�[�_�[�\����

//******************************
// �R���X�g���N�^
//******************************
CCpu::CCpu() :CCharacter(OBJTYPE_CPU)
{
	m_nCpuPointNum = 0;        // �R�[�X�f�[�^�z��Ǘ��p
	m_cpuType = CPU_TYPE_STAR; // CPU�̎��
	m_pDIstCollision = NULL;   // �ړ��ڕW�l�����蔻��
	m_nLevel = 0;              // CPU�̋���
}

//******************************
// �f�X�g���N�^
//******************************
CCpu::~CCpu()
{
}

//******************************
// �N���G�C�g
//******************************
CCpu * CCpu::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CPUTYPE cpuType , int nLevel)
{
	// �������̊m��
	CCpu *pCpu;
	pCpu = new CCpu;

	// �^�C�v�̐ݒ�
	pCpu->m_cpuType = cpuType; // �^�C�v
	
	// ������
	pCpu->Init();

	// �e�l�̑���E�Z�b�g
	pCpu->SetPos(pos);       // ���W�̃Z�b�g
	pCpu->SetRot(rot);       // ���W�̃Z�b�g
	pCpu->m_nLevel = nLevel; // ���x���̃Z�b�g 

	// ���x���̏���A�����͈̔�
	pCpu->m_nLevel = min(max(pCpu->m_nLevel, 1), CPU_LEVEL_MAX);

	return pCpu;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CCpu::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	// ���[�v�X�^�[
	D3DXLoadMeshFromX(MODEL_PATH_STAR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_aModel[CPU_TYPE_STAR].pBuffMat, NULL, &m_aModel[CPU_TYPE_STAR].nNumMat, &m_aModel[CPU_TYPE_STAR].pMesh);
	// ���C���X�^�[
	D3DXLoadMeshFromX(MODEL_PATH_RUIN, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_aModel[CPU_TYPE_RUIN].pBuffMat, NULL, &m_aModel[CPU_TYPE_RUIN].nNumMat, &m_aModel[CPU_TYPE_RUIN].pMesh);

	for (int nCnt = 0; nCnt < CPU_TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���ǂݍ���
		if (m_aModel[nCnt].nNumMat != 0)
		{
			D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_aModel[nCnt].pBuffMat->GetBufferPointer();
			for (int nCnt = 0; nCnt < (int)m_aModel[nCnt].nNumMat; nCnt++)
			{
				if (pMat[nCnt].pTextureFilename != NULL)
				{
					char cPath[64] = {};
					sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
					// �e�N�X�`���̐���
					D3DXCreateTextureFromFile(pDevice, cPath, &m_aModel[nCnt].apTexture[nCnt]);
				}
			}
		}
	}

	// HLSL�t�@�C���̓ǂݍ���
	D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CCpu::Unload(void)
{
	for (int nCnt = 0; nCnt < CPU_TYPE_MAX; nCnt++)
	{
		//���b�V���̔j��
		if (m_aModel[nCnt].pMesh != NULL)
		{
			m_aModel[nCnt].pMesh->Release();
			m_aModel[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_aModel[nCnt].pBuffMat != NULL)
		{
			m_aModel[nCnt].pBuffMat->Release();
			m_aModel[nCnt].pBuffMat = NULL;
		}
	}
}

//******************************
// ����������
//******************************
HRESULT CCpu::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_aModel[m_cpuType].pMesh, m_aModel[m_cpuType].pBuffMat, m_aModel[m_cpuType].nNumMat);

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_aModel[m_cpuType].nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_aModel[m_cpuType].apTexture[nCnt]);
	}

	// �V�F�[�_�[�̃Z�b�g
	SetShader(m_shader);

	// �R�[�X�f�[�^�z��Ǘ��p�ϐ��̏�����
	m_nCpuPointNum = 0;
	
	
	// �����ڕW���W�̐ݒ�

	// �R�[�X�f�[�^�̎擾
	CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum);
	// ���̖ڕW�n�_
	D3DXVECTOR3 distPos = pointData.pos;
	// ���̖ڕW�n�_�̊p�x
	float fDistAngle = pointData.fAngle;

	// ��������
	int nRand = (rand() % DIST_RAND_AMPLITUDE) - DIST_RAND_AMPLITUDE / 2;

	// �ڕW�n�_�̐ݒ�
	distPos.x = distPos.x + cosf(-fDistAngle)*nRand;
	distPos.y = distPos.y;
	distPos.z = distPos.z + sinf(-fDistAngle)*nRand;

	// �ڕW�n�_
	m_pDIstCollision = CCollision::CreateSphere(CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum).pos, CPU_DIST_RADIUS);

	return S_OK;
}

//******************************
// �I������
//******************************
void CCpu::Uninit(void)
{
	CCharacter::Uninit();
}

//******************************
// �X�V����
//******************************
void CCpu::Update(void)
{
	if (!GetGoalFlag())
	{
		// �ړ��ʂ̐ݒ�
		Move();

		// �ڕW�n�_�̊Ǘ�
		DistManager();
	}
	else
	{
		// �ړ��ʂ̃Z�b�g
		SetMoveDist(VEC3_ZERO);
	}

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

	// �R�[�X�̓����蔻��
	CGame::GetCourse()->CollisionCharacter((CCharacter*)this);
}

//******************************
// �`�揈��
//******************************
void CCpu::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// �ړ��ʂ̐ݒ菈��
//******************************
void CCpu::Move(void)
{
	// �ړ��ʂ̎擾
	D3DXVECTOR3 move = GetMove();
	
	// ���ݒn�_����ڕW�n�_�ւ̃x�N�g���̎擾
	D3DXVECTOR3 moveDist = m_pDIstCollision->GetPos() - GetPos();
	// Y���͈ړ����Ȃ�
	moveDist.y = 0;
	// ���K��
	D3DXVec3Normalize(&moveDist, &moveDist);
	// �X�s�[�h��������
	moveDist *= GetSpeed();
	
	// �ړ��ʂ̃Z�b�g
	SetMoveDist(moveDist);

}

//******************************
// �ڕW�n�_�̊Ǘ�
//******************************
void CCpu::DistManager(void)
{
	// �ڕW�n�_�ɂԂ������Ƃ�
	if (CCollision::CollisionSphere(GetCollision(), m_pDIstCollision))
	{
		// �ڕW�n�_�C���f�b�N�X��i�߂�
		m_nCpuPointNum++;

		// �ڕW�l�_���𒴂�����C���f�b�N�X��0�ɂ���
		if (m_nCpuPointNum >= CGame::GetCpuPoint()->GetPointNum())
		{
			m_nCpuPointNum = 0;
		}

		CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum);
		// ���̖ڕW�n�_
		D3DXVECTOR3 distPos = pointData.pos;
		// ���̖ڕW�n�_�̊p�x
		float fDistAngle = pointData.fAngle;

		int nRand = 0;

		switch (pointData.inType)
		{
		case CCpuPoint::POINT_TYPE_NORMAL:

			// ��������
			nRand = (rand() % DIST_RAND_AMPLITUDE) - DIST_RAND_AMPLITUDE / 2;

			break;

		case CCpuPoint::POINT_TYPE_IN_LEFT:
		{
			// ��������
			nRand = (rand() % DIST_RAND_AMPLITUDE / 2);

			// ���x���ő���R�[�X�̒���
			int nAdjust = DIST_RAND_AMPLITUDE / 2 - (((DIST_RAND_AMPLITUDE / 2) / CPU_LEVEL_MAX) *m_nLevel);

			nRand -= nAdjust;
		}
			break;

		case CCpuPoint::POINT_TYPE_IN_RIGHT:
		{
			// ��������
			nRand = -(rand() % DIST_RAND_AMPLITUDE / 2);
			// ���x���ő���R�[�X�̒���
			int nAdjust = DIST_RAND_AMPLITUDE / 2 - (((DIST_RAND_AMPLITUDE / 2) / CPU_LEVEL_MAX) *m_nLevel);
			nRand += nAdjust;
		}
			break;

		default:
			break;
		}

		// �ڕW�n�_�̐ݒ�
		distPos.x = distPos.x + cosf(-fDistAngle)*nRand;
		distPos.y = distPos.y;
		distPos.z = distPos.z + sinf(-fDistAngle)*nRand;

		// ���̖ڕW�n�_�Ɉړ�
		m_pDIstCollision->SetPos(distPos);
	}
}
