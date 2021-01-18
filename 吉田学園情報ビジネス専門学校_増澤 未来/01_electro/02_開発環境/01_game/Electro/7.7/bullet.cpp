////////////////////////////////////////////////////
//
//    bullet�N���X�̏���[bullet.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "bullet.h"
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
#include "boss.h"

//*****************************
// �}�N����`
//*****************************
#define BULLET_TEXTURE_PATH "./data/Textures/bullet000.png" //�e�N�X�`���̃p�X

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CBullet::m_pTexture = NULL; // �e�N�X�`���|�C���^
std::list<CBullet*> CBullet::m_bulletList;      // ���X�g

//******************************
// �R���X�g���N�^
//******************************
CBullet::CBullet():CScene3d(OBJTYPE_BULLET)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_user = BULLETUSER_PLAYER;
}

//******************************
// �f�X�g���N�^
//******************************
CBullet::~CBullet()
{
}

//******************************
// �N���G�C�g
//******************************
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const  BULLETUSER user,const D3DCOLOR col)
{
	// �������̊m��
	CBullet *pBullet;
	pBullet = new CBullet;
	// ������
	pBullet->Init();

	// �e�l�̑���E�Z�b�g
	pBullet->SetPos(pos);                // ���W
	pBullet->SetColor(col);              // �F
	pBullet->SetPriority(OBJTYPE_BULLET); // �I�u�W�F�N�g�^�C�v
	pBullet->m_move = move;              // �ړ���
	pBullet->m_nLife = nLife;            // ����
	pBullet->m_user = user;        // �o���b�g���[�U�[

	// ���X�g�ɒǉ�
	m_bulletList.push_back(pBullet);
	return pBullet;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE_PATH, &m_pTexture);


	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CBullet::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// ����������
//******************************
HRESULT CBullet::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	
	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	SetSize(D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f));

	return S_OK;
}

//******************************
// �I������
//******************************
void CBullet::Uninit(void)
{
	// ���X�g����폜
	std::list<CBullet*>::iterator iteretor;
	for (iteretor = m_bulletList.begin(); iteretor != m_bulletList.end(); )
	{
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = m_bulletList.erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}

	}

	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = GetPos(); // �e�̍��W

	// �ʒu�X�V
	pos += m_move;
	SetPos(pos);

	// �G�t�F�N�g����
	CEffect::Create(pos, GetSize(), GetColor(), m_pTexture);

	// �e�̎����̏���
	m_nLife--;
	if (m_nLife < 0)
	{
		// ��������
		CExplosion::Create(GetPos(), D3DXVECTOR3(40, 40, 0), GetColor());
		// �e������
		Uninit();

		return;
	}

	// �����蔻��
	CollisionBullet(m_user);
}

//******************************
// �`�揈��
//******************************
void CBullet::Draw(void)
{
	CScene3d::Draw();
}


//******************************
// �����蔻��
//******************************
void CBullet::CollisionBullet(BULLETUSER user)
{
	D3DXVECTOR3 pos = GetPos(); // �e�̍��W

	// �����蔻��

	if (user == BULLETUSER_PLAYER)
	{ // �v���C���[�̒e

		//�G�l�~�[�ւ̓����蔻��

	    // �G�l�~�[���X�g�̎擾
		std::list<CEnemy*>::iterator enemyIteretor;
		for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
		{
			D3DXVECTOR3 size = GetSize();  // �e�̃T�C�Y

			D3DXVECTOR3 enemyPos = (*enemyIteretor)->GetPos();   // �G�̍��W
			D3DXVECTOR3 enemySize = (*enemyIteretor)->GetSize(); // �G�̃T�C�Y

			if (pos.x - size.x <= enemyPos.x + enemySize.x / 3 &&
				pos.x + size.x >= enemyPos.x - enemySize.x / 3 &&
				pos.y - size.y <= enemyPos.y + enemySize.y / 3 &&
				pos.y + size.y >= enemyPos.y - enemySize.y / 3)
			{
				// �����̐���
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// �G�l�~�[����SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
				
				// �G�l�~�[������
				(*enemyIteretor)->HitAttack(1);
				//(*iteretor)->Uninit();
				// �e������
				Uninit();
				return;
			}
		}

		if (CBoss::IsAlive())
		{
			// �{�X�ւ̓����蔻��
			D3DXVECTOR3 size = GetSize();  // �e�̃T�C�Y

			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();   // �G�̍��W
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize(); // �G�̃T�C�Y

			if (pos.x - size.x <= bossPos.x + bossSize.x / 3 &&
				pos.x + size.x >= bossPos.x - bossSize.x / 3 &&
				pos.y - size.y <= bossPos.y + bossSize.y / 3 &&
				pos.y + size.y >= bossPos.y - bossSize.y / 3)
			{
				// �����̐���
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// �G�l�~�[����SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

				// �G�l�~�[������
				CGame::GetBoss()->HitAttack(1);
				// �t�B�[�o�[�l��i�߂�
				CGame::GetPlayer()->ProgressFever(0.2f);
				// �e������
				Uninit();
				return;
			}

		}
	}
	else if (user == BULLETUSER_ENEMY)
	{// �G�̒e
		if (CPlayer::IsAlive())
		{
			CPlayer *pPlayer = CGame::GetPlayer();

			D3DXVECTOR3 size = GetSize();  // �e�̃T�C�Y

			D3DXVECTOR3 playerPos = pPlayer->GetPos();   // Player�̍��W
			D3DXVECTOR3 playerSize = pPlayer->GetSize(); // Player�̃T�C�Y

			if (pos.x - size.x / 2 <= playerPos.x + 20 &&
				pos.x + size.x / 2 >= playerPos.x - 20 &&
				pos.y - size.y / 2 <= playerPos.y + 20 &&
				pos.y + size.y / 2 >= playerPos.y - 20)
			{
				if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
				{
					// ����SE
					CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
					// ���C�t�����炷
					pPlayer->HitAttack(1);
					//pPlayer->SetState(CPlayer::STATE_DAMAGE);
				}
				// �e������
				Uninit();
				return;
			}
		}
	}

}
