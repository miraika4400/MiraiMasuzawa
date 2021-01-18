////////////////////////////////////////////////////
//
//    bulletex�N���X�̏���[bulletex.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "bulletex.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "effect.h"
#include "game.h"

//******************************
 // �R���X�g���N�^
 //******************************
CBulletEx::CBulletEx()
{
	// �ϐ��̃N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_fRotAngle = 0.0f;
	m_fDistance = 10.0f;
	m_nTargetNum = 0;
}

//******************************
// �f�X�g���N�^
//******************************
CBulletEx::~CBulletEx()
{
}

CBulletEx * CBulletEx::Create_Rotation(const D3DXVECTOR3 pos, const D3DXVECTOR3 posParent, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col)
{
		// �������̊m��
		CBulletEx *pBulletEx;
		pBulletEx = new CBulletEx;
		// ������
		pBulletEx->Init();
	
		// �e�l�̑���E�Z�b�g
		pBulletEx->SetPos(pos);                        // ���W
		pBulletEx->SetColor(col);                      // �F
		pBulletEx->SetPriority(OBJTYPE_BULLET);         // �I�u�W�F�N�g�^�C�v
		pBulletEx->m_posParent = posParent;            // �e���W
		pBulletEx->m_nLife = nLife;                    // ����
		pBulletEx->SetUser(user);                      // �o���b�g���[�U�[
		pBulletEx->m_bulletType = BULLETTYPE_ROTATION; // �e�̎��
		pBulletEx->m_move = move;                      // �ړ���
		// �e���W�Ƃ̋����̌v�Z
		pBulletEx->m_fDistance = sqrtf(powf(pos.x - posParent.x, 2) + powf(pos.y - posParent.y, 2));

		// ���X�g�ɒǉ�
		GetBulletlist()->push_back(pBulletEx);
		return pBulletEx;
}

//******************************
// �N���G�C�g
//******************************
//CBulletEx * CBulletEx::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const BULLETUSER user, const D3DCOLOR col, const BULLETTYPE type, const float rotAngle)
//{
//	// �������̊m��
//	CBulletEx *pBulletEx;
//	pBulletEx = new CBulletEx;
//	// ������
//	pBulletEx->Init();
//
//	// �e�l�̑���E�Z�b�g
//	pBulletEx->SetPos(pos);                // ���W
//	pBulletEx->SetColor(col);              // �F
//	pBulletEx->SetObjType(OBJTYPE_BULLET); // �I�u�W�F�N�g�^�C�v
//	pBulletEx->m_posParent = pos;          // �e���W
//	pBulletEx->m_move = move;              // �ړ���
//	pBulletEx->m_nLife = nLife;            // ����
//	pBulletEx->m_user = user;              // �o���b�g���[�U�[
//	pBulletEx->m_bulletType = type;        // �e�̎��
//	pBulletEx->m_fRotAngle = rotAngle;
//
//
//	return pBulletEx;
//}

CBulletEx * CBulletEx::Create_Rotation_Spread(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col, const float rotAngle)
{
	// �������̊m��
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// ������
	pBulletEx->Init();

	// �e�l�̑���E�Z�b�g
	pBulletEx->SetPos(pos);                // ���W
	pBulletEx->SetColor(col);              // �F
	pBulletEx->SetPriority(OBJTYPE_BULLET); // �I�u�W�F�N�g�^�C�v
	pBulletEx->SetSize(size);              // �T�C�Y�̃Z�b�g
	pBulletEx->m_posParent = pos;          // �e���W
	pBulletEx->m_nLife = nLife;            // ����
	pBulletEx->SetUser(user);              // �o���b�g���[�U�[
	pBulletEx->m_bulletType = BULLETTYPE_ROTATION_SPREAD;        // �e�̎��
	pBulletEx->m_fRotAngle = rotAngle;

	// ���X�g�ɒǉ�
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

CBulletEx * CBulletEx::Create_Target(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col)
{
	// �������̊m��
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// ������
	pBulletEx->Init();

	// �e�l�̑���E�Z�b�g
	pBulletEx->SetPos(pos);                // ���W
	pBulletEx->SetColor(col);              // �F
	pBulletEx->SetPriority(OBJTYPE_BULLET); // �I�u�W�F�N�g�^�C�v
	pBulletEx->SetSize(size);              // �T�C�Y�̃Z�b�g
	pBulletEx->m_posParent = pos;          // �e���W
	pBulletEx->m_move = move;              // �ړ���
	pBulletEx->m_nLife = nLife;            // ����
	pBulletEx->SetUser(user);              // �o���b�g���[�U�[
	pBulletEx->m_bulletType = BULLETTYPE_TARGET;        // �e�̎��

														// ���X�g�ɒǉ�
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

CBulletEx * CBulletEx::Create_Homing(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col, const int nTargetNum)
{
	// �������̊m��
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// ������
	pBulletEx->Init();

	// �e�l�̑���E�Z�b�g
	pBulletEx->SetPos(pos);                // ���W
	pBulletEx->SetColor(col);              // �F
	pBulletEx->SetPriority(OBJTYPE_BULLET); // �I�u�W�F�N�g�^�C�v
	pBulletEx->SetSize(size);              // �T�C�Y�̃Z�b�g
	pBulletEx->m_posParent = pos;          // �e���W
	pBulletEx->m_move = move;              // �ړ���
	pBulletEx->m_nLife = nLife;            // ����
 	pBulletEx->SetUser(user);              // �o���b�g���[�U�[
	pBulletEx->m_bulletType = BULLETTYPE_HOMING;        // �e�̎��
	pBulletEx->m_nTargetNum = nTargetNum;

	// ���X�g�ɒǉ�
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

//******************************
// ����������
//******************************
HRESULT CBulletEx::Init(void)
{
	CBullet::Init();

	return S_OK;
}

//******************************
// �I������
//******************************
void CBulletEx::Uninit(void)
{
	// ���X�g����폜
	std::list<CBullet*>::iterator iteretor;
	for (iteretor = GetBulletlist()->begin(); iteretor != GetBulletlist()->end(); )
	{
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = GetBulletlist()->erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}

	}

	CBullet::Uninit();
}

//******************************
// �X�V����
//******************************
void CBulletEx::Update(void)
{
	// �G�t�F�N�g����
	CEffect::Create(GetPos(), GetSize(), GetColor(),m_pTexture);

	// �e���W�̐ݒ�
	D3DXVECTOR3 pos = GetPos();

	switch (m_bulletType)
	{
	case BULLETTYPE_ROTATION:

		// �e���W�𒆐S�ɔ��a"m_fDistance"�̊p�x"m_fRotAngle"
		m_fRotAngle = atan2f(pos.y - m_posParent.y, pos.x - m_posParent.x);

		// �p�x�𑫂�
		m_fRotAngle += 0.02f;
		pos.x = m_posParent.x + cosf(m_fRotAngle) * m_fDistance;
		pos.y = m_posParent.y + sinf(m_fRotAngle) * m_fDistance;
		pos.z = 0.0f;

		//
		m_posParent += m_move; 
		pos += m_move;
		break;

	case BULLETTYPE_ROTATION_SPREAD:

		// �e���W�𒆐S�ɔ��a"m_fDistance"�̊p�x"m_fRotAngle"
		pos.x = m_posParent.x + cosf(m_fRotAngle) * m_fDistance;
		pos.y = m_posParent.y + sinf(m_fRotAngle) * m_fDistance;
		pos.z = 0.0f;

		m_fRotAngle += 0.02f;
		// ���a���L����
		m_fDistance += 2;
		break;

	case BULLETTYPE_HOMING:

		if (GetUser() == BULLETUSER_PLAYER)
		{
			// �G�l�~�[���X�g�̎擾
			std::list<CEnemy*>::iterator iteretor;
			for (iteretor = CEnemy::GetEnemylist()->begin(); iteretor != CEnemy::GetEnemylist()->end(); iteretor++)
			{
				if (m_nTargetNum == (*iteretor)->GetID())
				{
					D3DXVECTOR3 targetPos = (*iteretor)->GetPos();
					float fAngle = atan2f(targetPos.y - GetPos().y, targetPos.x - GetPos().x);

					m_move = D3DXVECTOR3(cosf(fAngle)*BULLET_SPEED_PLAYER, sinf(fAngle)*BULLET_SPEED_PLAYER, 0.0f);
					break;
				}
			}
		}
		pos += m_move;
		break;

	default:
		// ���W�Ɉړ��ʂ𑫂�
		pos += m_move;

		break;
	}

	SetPos(pos);

	// �e�̎����̏���
	m_nLife--;

	if (m_nLife < 0)
	{
		if (m_bulletType == BULLETTYPE_TARGET)
		{
			if (GetUser() == BULLETUSER_ENEMY)
			{
				// ���t���[����~
				m_move = D3DXVECTOR3(0, 0, 0);

				// ���@�_���̒e���o��
				if (m_nLife < -50)
				{
					// �G����v���C���[�̊p�x
					float fAngle = 0.0f;
					// �v���C���[���擾
					CPlayer*pPlayer = CGame::GetPlayer();

					if (CPlayer::IsAlive())
					{
						// �G����v���C���[�̊p�x�̎擾
						fAngle = atan2f(pPlayer->GetPos().y - GetPos().y, pPlayer->GetPos().x - GetPos().x);
					}

					// �e�̐���
					CBullet::Create(GetPos(),
						D3DXVECTOR3(cosf(fAngle)*BULLET_SPEED_ENEMY, sinf(fAngle)*BULLET_SPEED_ENEMY, 0.0f),
						150,
						CBullet::BULLETUSER_ENEMY,
						GetColor());

					// �e������
					Uninit();
				}
			}
		}
		else
		{
			// ��������
			CExplosion::Create(GetPos(), D3DXVECTOR3(40, 40, 0), GetColor());
			// �e������
			Uninit();
		}
		
		

		return;
	}

	// �����蔻��
	CollisionBullet(GetUser());
}

//******************************
// �`�揈��
//******************************
void CBulletEx::Draw(void)
{
	CBullet::Draw();
}