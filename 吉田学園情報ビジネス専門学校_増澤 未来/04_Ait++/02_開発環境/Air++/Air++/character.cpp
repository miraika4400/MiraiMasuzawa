//=============================================================================
//
// character�w�b�_ [character.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "collision.h"
#include "course.h"
#include "checkpoint.h"

//*****************************
// �}�N����`
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // �d�͗�
#define CHARACTER_GRAVITY_RATE 0.03f                    // �d�͂̌W��
#define CHARACTER_DIRECTION_RATE 0.3f                   // ��]�p�x�̌W��
#define CHARACTER_MOVE_RATE 0.05f                       // �ړ��ʂ̌W��
//#define CHARACTER_MOVE_RATE 1.0f                      // �ړ��ʂ̌W��
#define CHARACTER_ROT_X_ADJUST_RANGE 300.0f             // �n�ʂƊp�x�����킹��Ƃ��̒n�ʂƂ̋����͈̔�
#define RAY_DISTANCE 50.0f                              // ���C���o������*���������Ɏg�p
#define RAY_HEIGHT   100.0f                             // ���C���o������*���������Ɏg�p
#define CHARACTER_SPEED_BASE 30.0f                      // �ړ��X�s�[�h
#define CHARACTER_SPEED_ACCELERATION 45.0f              // �ړ��X�s�[�h*������
#define CHARACTER_ACCELERATION_RATE 0.06f               // �������̃X�s�[�h�ω��̌W��
#define CHARACTER_ACCELERATION_COUNT 70                 // �����t���[����
#define CHARACTER_STAN_FRAME 70                         // �X�^���̒���(�t���[��)
#define CHARACTER_FALL_COUNT 40                         // �d�̓t���O�����t���[���o���Ă��痎���Ɣ��肷��

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************

//******************************
// �R���X�g���N�^
//******************************
CCharacter::CCharacter(int nPliority) :CModelShader(nPliority)
{
	// �ϐ��̃N���A
	m_move       = VEC3_ZERO;  // �ړ���
	m_moveDist   = VEC3_ZERO;  // �ړ��ʂ̖ڕW�l
	m_gravityVec = VEC3_ZERO;  // �d�͗�
	m_bGravity   = false;       // �d�̓t���O
	m_pCollision = NULL;       // �R���W����
	m_rankData   = {};         // ���ʕt���p�f�[�^
	m_bGoal = false;           // �S�[���t���O
	m_impact = VEC3_ZERO;      // �Փ˂����Ƃ��̈ړ���
	m_item = CItem::ITEM_NONE; // �������Ă���A�C�e��
	m_bAcceleration = false;   // �����t���O
	m_fSpeed = CHARACTER_SPEED_BASE; // �ړ����x
	m_nCntAccleration = 0;     // �������̃J�E���g
	m_bStan = false;           // �X�^���t���O
	m_nCntStan = 0;            // �X�^���J�E���g
	m_nCntGravity = 0;         // �d�̓t���O���o���Ă���Ԃ̃J�E���g

	// �I�u�W�F�N�g�^�C�v���v���C���[�̎��A�v���C���[�t���O��true�ɂ���
	if (nPliority == OBJTYPE_PLAYER) m_bIsPlayer = true;
	else m_bIsPlayer = false;
}

//******************************
// �f�X�g���N�^
//******************************
CCharacter::~CCharacter()
{
}

//******************************
// ����������
//******************************
HRESULT CCharacter::Init(void)
{
	// ���f���N���X�̏���������
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// �R���W�����̐���
	m_pCollision = CCollision::CreateSphere(GetPos(), CHARACTER_RADIUS);

	// �ϐ��̎擾
	m_bGoal = false;                 // �S�[���t���O
	m_item = CItem::ITEM_NONE;       // �������Ă���A�C�e��
	m_bAcceleration = false;         // �����t���O
	m_fSpeed = CHARACTER_SPEED_BASE; // �ړ����x
	m_nCntAccleration = 0;     // �������̃J�E���g
	m_bStan = false;           // �X�^���t���O
	m_nCntStan = 0;            // �X�^���J�E���g
	m_bGravity = false;        // �d�̓t���O
	m_nCntGravity = 0;         // �d�̓t���O���o���Ă���Ԃ̃J�E���g

	// ���ʕt���f�[�^�̏�����
	m_rankData.nCheck = 0;
	m_rankData.nLap = 0;
	m_rankData.nRank = 1;

	return S_OK;
}

//******************************
// �I������
//******************************
void CCharacter::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModelShader::Uninit();
}

//******************************
// �X�V����
//******************************
void CCharacter::Update(void)
{
	// �d�͏���
	Gravity();
	
	// �����̏���
	Direction();

	// �L�������m�̓����蔻��̏���
	CollisionCharacter();

	// �X�s�[�h�̊Ǘ�
	SpeedManager();

	// �ړ��ʏ��X�ɖڕW�l�ɋ߂Â���
	m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

	// ���W�Ɉړ��ʂ𑫂�
	SetPos(GetPos() + m_move);

	// ���f���N���X�̍X�V����
	CModelShader::Update();

	if (m_pCollision != NULL)
	{
		// �����蔻��̈ʒu�X�V
		m_pCollision->SetPos(GetPos());
	}
}

//******************************
// �`�揈��
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// �����t���O�̃Z�b�g
//******************************
void CCharacter::SetActiveAcceleration(bool bBool)
{
	// �����̃Z�b�g
	m_bAcceleration = bBool;

	if (m_bAcceleration)
	{// ������true��������
		// �J�E���g�̏�����
		m_nCntAccleration = 0;

		// �X�^����Ԃ̉���
		m_bStan = false;
	}
}

//******************************
// �X�^���t���O�̃Z�b�g
//******************************
void CCharacter::SetStan(bool bBool)
{
	// �����̃Z�b�g
	m_bStan = bBool;
	
	if (m_bStan)
	{// ������true��������

		// ������Ԃ̉���
		m_bAcceleration = false;
	}
}

//******************************
// �d�͂̊Ǘ�
//******************************
void CCharacter::Gravity(void)
{
	if (m_bGravity)
	{// �d�̓t���O�������Ă���

		// �d�͗ʂ̐ݒ�
		m_gravityVec += (CHARACTER_GRAVITY - m_gravityVec)*CHARACTER_GRAVITY_RATE;

		// ���W�ɏd�͗ʂ̃v���X
		SetPos(GetPos() + m_gravityVec);
		m_nCntGravity++;

		if (m_nCntGravity >= CHARACTER_FALL_COUNT)
		{
			if (m_rankData.nCheck != 0)
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(m_rankData.nCheck - 1)->GetPos());
			}
			else
			{
				SetPos(VEC3_ZERO);
			}
		}
	}
	else
	{// �d�̓t���O�������ĂȂ��Ƃ�
	 // ������
		if (m_nCntGravity != 0)m_nCntGravity = 0;

		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// �����̊Ǘ�
//******************************
void CCharacter::Direction(void)
{
	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// Y��

	// �ړ��ʂ�0����Ȃ��Ƃ�&&�X�^����Ԃ���Ȃ��Ƃ�
	if (m_move != VEC3_ZERO && !m_bStan)
	{
		
		// Y���̖ڕW�l
		float fRotYDist = atan2f(m_move.x, m_move.z);

		// �O����Ɖ�]���Ȃ��悤����
		while (rot.y - fRotYDist > D3DXToRadian(180))
		{
			fRotYDist += D3DXToRadian(360);
		}
		while (rot.y - fRotYDist < D3DXToRadian(-180))
		{
			fRotYDist -= D3DXToRadian(360);
		}

		// ���X�ɖڕW�l�ɋ߂Â���
		rot.y += (fRotYDist - rot.y) * CHARACTER_DIRECTION_RATE;

		// �����̃Z�b�g
		SetRot(rot);
	}

	// X��

	if (CGame::GetCourse() != NULL)
	{
		// ���C���o���ʒu
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);

		// �R�[�X���b�V���̎擾
		LPD3DXMESH pCourseMesh = CGame::GetCourse()->GetMesh();
		// 
		BOOL bHit = false;
		float fHitDistance;

		// ���C�ɂ�铖���蔻��
		D3DXIntersect(pCourseMesh,
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fHitDistance,
			NULL,
			NULL);

		if (bHit)
		{// �������Ă�����

			if (fHitDistance <= CHARACTER_ROT_X_ADJUST_RANGE)
			{
				// �������Ă�����W
				D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X���̖ڕW�l
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// ���X�ɖڕW�l�ɋ߂Â���
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				// �����̃Z�b�g
				SetRot(rot);
			}
		}
	}
}

//*****************************************
// �L�����N�^�[���m�̓����蔻�菈��
//*****************************************
void CCharacter::CollisionCharacter(void)
{
	// �L�����N�^�[�������[�v
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[���̎擾
		CCharacter *pTarget = CGame::GetCharacter(nCnt);

		// NULL�`�F�b�N&��������Ȃ��Ƃ�
		if (pTarget != NULL&&pTarget != this)
		{
			if (CCollision::CollisionSphere(m_pCollision, pTarget->m_pCollision))
			{// �������Ă�����
			 
				// �O�ɉ����o��
				D3DXVECTOR3 vec = (GetPos() - pTarget->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pTarget->GetCollision()->GetCollisionRadius());
				// ���W�̍X�V
				SetPos(pTarget->GetPos() + vec);
			}
		}
	}
}

//*****************************************
// �X�s�[�h�Ǘ�����
//*****************************************
void CCharacter::SpeedManager(void)
{
	if (m_bAcceleration)
	{// ����

		// ������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed += (CHARACTER_SPEED_ACCELERATION- m_fSpeed)*CHARACTER_ACCELERATION_RATE;
		
		// �J�E���g��i�߂�
		m_nCntAccleration++;
		if (m_nCntAccleration > CHARACTER_ACCELERATION_COUNT)
		{// �����J�E���g�����ɒB������
			// �����I��
			m_bAcceleration = false;
			// �J�E���g�̏�����
			m_nCntAccleration = 0;
		}
	}
	else if (m_bStan)
	{// �X�^�����

		// ������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed += (0.0f - m_fSpeed)*CHARACTER_MOVE_RATE;

		// �J�E���g��i�߂�
		m_nCntStan++;
		D3DXVECTOR3 rot = GetRot();
		rot.y += D3DXToRadian(360 / CHARACTER_STAN_FRAME);
		SetRot(rot);
		if (m_nCntStan > CHARACTER_STAN_FRAME)
		{// �����J�E���g�����ɒB������
		 // �����I��
			m_bStan = false;
			// �J�E���g�̏�����
			m_nCntStan = 0;
		}
	}
	else
	{
		// �������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed += (CHARACTER_SPEED_BASE - m_fSpeed)*CHARACTER_MOVE_RATE;
	}
}
