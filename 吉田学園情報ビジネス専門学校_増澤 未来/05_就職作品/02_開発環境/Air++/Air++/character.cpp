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

//*****************************
// �}�N����`
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // �d�͗�
#define CHARACTER_GRAVITY_RATE 0.03f                    // �d�͂̌W��
#define CHARACTER_DIRECTION_RATE 0.3f                   // ��]�p�x�̌W��
#define CHARACTER_MOVE_RATE 0.05f                       // �ړ��ʂ̌W��
//#define CHARACTER_MOVE_RATE 1.0f                      // �ړ��ʂ̌W��
#define CHARACTER_RADIUS 100.0f                         // �v���C���[���a(�����蔻��)

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
	m_bGravity   = true;       // �d�̓t���O
	m_pCollision = NULL;       // �R���W����
	m_rankData   = {};         // ���ʕt���p�f�[�^
	m_bGoal = false;           // �S�[���t���O
	m_impact = VEC3_ZERO;      // �Փ˂����Ƃ��̈ړ���

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

	// ���ʕt���f�[�^�̏�����
	m_rankData.nCheck = 0;
	m_rankData.nLap   = 0;
	m_rankData.nRank  = 1;

	// �S�[���t���O�̏�����
	m_bGoal = false;

	return S_OK;
}

//******************************
// �I������
//******************************
void CCharacter::Uninit(void)
{
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

	// �ړ��ʏ��X�ɖڕW�l�ɋ߂Â���
	m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

	// ���W�Ɉړ��ʂ𑫂�
	SetPos(GetPos() + m_move);

	// ���f���N���X�̍X�V����
	CModelShader::Update();

	// �����蔻��̈ʒu�X�V
	m_pCollision->SetPos(GetPos());
}

//******************************
// �`�揈��
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
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
	}
	else
	{// �d�̓t���O�������ĂȂ��Ƃ�
	 // ������
		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// �����̊Ǘ�
//******************************
void CCharacter::Direction(void)
{
	// �ړ��ʂ�0����Ȃ��Ƃ�
	if (m_move != VEC3_ZERO)
	{
		// �����̎擾
		D3DXVECTOR3 rot = GetRot();

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
