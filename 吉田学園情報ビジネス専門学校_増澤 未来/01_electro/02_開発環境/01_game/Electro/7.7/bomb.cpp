////////////////////////////////////////////////////
//
//    bomb�N���X�̏���[bomb.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "bomb.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "boss.h"
#include "particle.h"
#include "bullet.h"
#include "score.h"
#include "game.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************
#define BOMB_TEXTURE_PATH "./data/Textures/bomb000.png" //�e�N�X�`���̃p�X
#define BOMB_ANIM_SPEED 4      // �A�j���[�V�������x
#define BOMB_MAX_ANIMATION_X 1 // �A�j���[�V������ ��
#define BOMB_MAX_ANIMATION_Y 1 // �A�j���[�V������ �c
#define BOMB_RATE_SIZE 0.1f;

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9 CBomb::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CBomb::CBomb():CScene3d(OBJTYPE_BOMB)
{
	m_bHitBoss = false;
	m_fMaxsize = 0.0f;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//******************************
// �f�X�g���N�^
//******************************
CBomb::~CBomb()
{
}

//******************************
// �N���G�C�g
//******************************
CBomb * CBomb::Create(const D3DXVECTOR3 pos, const float fMaxsize, const D3DCOLOR col)
{
	// �������̊m��
	CBomb *pBomb;
	pBomb = new CBomb;
	// ������
	pBomb->Init();

	// �e�l�̑���E�Z�b�g
	pBomb->SetPos(pos);                // ���W
	pBomb->SetColor(col);              // �F
	pBomb->SetSize(D3DXVECTOR3(1.0f,1.0f,1.0f));              // �T�C�Y
	pBomb->SetPriority(OBJTYPE_BOMB);   // �I�u�W�F�N�g�^�C�v
	pBomb->m_fMaxsize = fMaxsize;

	// SE�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_BOMB);

	return pBomb;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CBomb::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BOMB_TEXTURE_PATH, &m_pTexture);


	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CBomb::Unload(void)
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
HRESULT CBomb::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / BOMB_MAX_ANIMATION_X;
	float fv = 1.0f / BOMB_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	return S_OK;
}

//******************************
// �I������
//******************************
void CBomb::Uninit(void)
{

	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CBomb::Update(void)
{
	// ���g�̍��W�̎擾
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 size = GetSize();
	size.x += ((m_fMaxsize + 3.0f) - size.x)*BOMB_RATE_SIZE;
	size.y += ((m_fMaxsize + 3.0f) - size.y)*BOMB_RATE_SIZE;
	SetSize(size);

	// �����蔻��

	// �G�l�~�[���X�g�̎擾
	std::list<CEnemy*>::iterator enemyIteretor;
	for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
	{

		D3DXVECTOR3 size = GetSize()/2;  // �e�̃T�C�Y

		D3DXVECTOR3 enemyPos = (*enemyIteretor)->GetPos();   // �G�̍��W
		D3DXVECTOR3 enemySize = (*enemyIteretor)->GetSize(); // �G�̃T�C�Y

		if (pos.x - size.x <= enemyPos.x + enemySize.x &&
			pos.x + size.x >= enemyPos.x - enemySize.x &&
			pos.y - size.y <= enemyPos.y + enemySize.y &&
			pos.y + size.y >= enemyPos.y - enemySize.y)
		{
			// �����̐���
			//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// �G�l�~�[����SE
			CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

			// �G�l�~�[������
			(*enemyIteretor)->HitAttack(5);
			
			int nScore = 50;
			if (CGame::GetPlayer()->GetFever())
			{// �t�B�[�o�[���X�R�A�{�[�i�X
				nScore *= 1.5f;
			}
			// �X�R�A���Z
			CScore::AddScore(nScore);

			// �t�B�[�o�[�l��i�߂�
			CGame::GetPlayer()->ProgressFever(0.5f);
			break;

		}
	}

	// �o���b�g���X�g�̎擾
	std::list<CBullet*>::iterator bulletIteretor;
	for (bulletIteretor = CBullet::GetBulletlist()->begin(); bulletIteretor != CBullet::GetBulletlist()->end(); )
	{
		if ((*bulletIteretor)->GetUser() == CBullet::BULLETUSER_ENEMY)
		{
			D3DXVECTOR3 size = GetSize() / 2;  // �e�̃T�C�Y

			D3DXVECTOR3 bulletPos = (*bulletIteretor)->GetPos();   // �G�̍��W
			D3DXVECTOR3 bulletSize = (*bulletIteretor)->GetSize(); // �G�̃T�C�Y

			if (pos.x - size.x <= bulletPos.x + bulletSize.x &&
				pos.x + size.x >= bulletPos.x - bulletSize.x &&
				pos.y - size.y <= bulletPos.y + bulletSize.y &&
				pos.y + size.y >= bulletPos.y - bulletSize.y)
			{
				// �����̐���
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				
				CBullet*pBullet = *bulletIteretor;
				bulletIteretor = CBullet::GetBulletlist()->erase(bulletIteretor);
				//	�e������
				pBullet->Uninit();
				//// ���X�g�̏�����
				//bulletIteretor = CBullet::GetBulletlist()->begin();

				int nScore = 20;
				if (CGame::GetPlayer()->GetFever())
				{// �t�B�[�o�[���X�R�A�{�[�i�X
					nScore *= 1.5f;
				}
				// �X�R�A���Z
				CScore::AddScore(nScore);
				// �t�B�[�o�[�l��i�߂�
				CGame::GetPlayer()->ProgressFever(0.5f);
			}
			else
			{
				bulletIteretor++;
			}
		}
		else
		{
			bulletIteretor++;
		}
	}

	if (CBoss::IsAlive())
	{
		if (!m_bHitBoss)
		{
			// �{�X�ւ̓����蔻��
			D3DXVECTOR3 size = GetSize();  // �e�̃T�C�Y

			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();   // �G�̍��W
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize(); // �G�̃T�C�Y

			if (pos.x - size.x/2 <= bossPos.x + bossSize.x / 2 &&
				pos.x + size.x/2 >= bossPos.x - bossSize.x / 2 &&
				pos.y - size.y/2 <= bossPos.y + bossSize.y / 2 &&
				pos.y + size.y/2 >= bossPos.y - bossSize.y / 2)
			{
				// �����̐���
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// �G�l�~�[����SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

				// �G�l�~�[������
				CGame::GetBoss()->HitAttack(5);
				// �X�R�A���Z
				CScore::AddScore(50);
				m_bHitBoss = true;
			}
		}
	}
	int nRnd = rand() % 10 + 5;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// �����_���ȋ���
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// �����_���Ȋp�x
		int nRandAngle = rand() % 360;
		
		// �����_���ȍ��W
		D3DXVECTOR3 randPos;
		randPos.x = pos.x + cosf(D3DXToRadian(nRandAngle))*(size.x/2 + fRandDistance);
		randPos.y = pos.y + sinf(D3DXToRadian(nRandAngle))*(size.y/2 + fRandDistance);
		randPos.z = 0.0f;
	
		if (m_fMaxsize >= 500)
		{// �A�C�e���̃{��

			// �����_���ȃT�C�Y
			int nRandSize = rand() % 30 + 20;
			// �p�[�e�B�N���̐���
			CParticle::Create(randPos, 
				D3DXVECTOR3((float)(rand() % 3 - 1),
				(float)(rand() % 3 - 1), 0.0f), 
				D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
				rand() % 10 + 10,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f));
		}
		else
		{// �}�i�T�[�N���̃{��

			// �����_���ȃT�C�Y
			int nRandSize = rand() % 20 + 15;
			// �p�[�e�B�N���̐���
			CParticle::Create(randPos,
				D3DXVECTOR3((float)(rand() % 3 - 1),
				(float)(rand() % 3 - 1), 0.0f),
				D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
				rand() % 10 + 10,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f, 1.0f));
		}
	}
	
	// �ő�T�C�Y�ɓ��B���������
	if (size.x > m_fMaxsize)
	{
		Uninit();
		return;
	}


	//// �A�j���[�V�����J�E���g��i�߂�
	//m_nCntAnim++;

	//if (m_nCntAnim % BOMB_ANIM_SPEED == 0)
	//{
	//	// �A�j���[�V����X���̉��Z
	//	m_nAnimX++;

	//	if (m_nAnimX >= BOMB_MAX_ANIMATION_X)
	//	{
	//		// ����
	//		Uninit();
	//		return;
	//	}

	//	// UV���W�̐ݒ�
	//	D3DXVECTOR2 uv[NUM_VERTEX];
	//	float fu = 1.0f / BOMB_MAX_ANIMATION_X;
	//	float fv = 1.0f / BOMB_MAX_ANIMATION_Y;

	//	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	//	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	//	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	//	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	//	// UV���W�Z�b�g
	//	SetTextureUV(uv);
	//}

}

//******************************
// �`�揈��
//******************************
void CBomb::Draw(void)
{
//	CScene3d::Draw();
}

