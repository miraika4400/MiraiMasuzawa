//===================================================
//
//    ���C�t�N���X�̏���[life.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
// �C���N���[�h
//**********************************
#include "minimap.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "enemy.h"
#include "stage.h"
#include "game.h"
#include "boss.h"
#include "item.h"
#include "manacircle.h"

//**********************************
// �}�N����`
//**********************************
#define  MINIMAP_TEXTURE_PATH "./data/Textures/Circle.png" // �e�N�X�`��
#define MINIMAP_POS_X SCREEN_WIDTH  - 150.0f  // �~�j�}�b�v��X�ʒu
#define MINIMAP_POS_Y SCREEN_HEIGHT - 150.0f // �~�j�}�b�v��Y�ʒu
#define MINIMAP_SCAIL 0.1f

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CMiniMap::m_pTexture = NULL;
D3DXVECTOR3 CMiniMap::m_stagePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//==================================
// �R���X�g���N�^
//==================================
CMiniMap::CMiniMap() :CScene(OBJTYPE_UI)
{
	// 0�N���A
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
}

//==================================
// �f�X�g���N�^
//==================================
CMiniMap::~CMiniMap()
{
}

//==================================
// �N���G�C�g
//==================================
CMiniMap * CMiniMap::Create(void)
{
	// �������̊m��
	CMiniMap *pMiniMap;
	pMiniMap = new CMiniMap;
	// ������
	pMiniMap->Init();
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pMiniMap->SetPriority(OBJTYPE_UI);

	return pMiniMap;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CMiniMap::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, MINIMAP_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CMiniMap::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// ����������
//==================================
HRESULT CMiniMap::Init(void)
{

	// �X�e�[�W
	D3DXVECTOR3 size; // �T�C�Y�̎擾�p
	size = (STAGE_SIZE / 2)*MINIMAP_SCAIL; // �T�C�Y�̎擾
	m_stagePos = STAGE_POS;                // ���W�̎擾

	// �}�b�v���̃X�e�[�W����
	m_apPolygon[0] = CPolygon::Create(
		D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f),
		size,
		D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.8f));
	m_apPolygon[0]->BindTexture(m_pTexture);

	
	return S_OK;
}

//==================================
// �I������
//==================================
void CMiniMap::Uninit(void)
{
	// ���C�t�̍ő吔���|���S���̏I������
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			// �I������
			m_apPolygon[nCntMiniMap]->Uninit();
			// �������̉��
			delete m_apPolygon[nCntMiniMap];
			m_apPolygon[nCntMiniMap] = NULL;
		}
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CMiniMap::Update(void)
{
	D3DXVECTOR3 size; // �T�C�Y�̎擾�p
	D3DXVECTOR3 pos;  // ���W�v�Z�p
	float fAngle;     // �X�e�[�W��������̊p�x�擾�p
	float fDistance;  // �X�e�[�W��������̋���

	// �}�b�v�̏����� ��0�̓X�e�[�W�Ȃ̂ŏ��������Ȃ�
	for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Uninit();
			delete m_apPolygon[nCntMiniMap];
			m_apPolygon[nCntMiniMap] = NULL;
		}
	}

	// �}�i�T�[�N�����}�b�v�ɕ\��

	// ���X�g�̎擾
	
	for (int nCntCirle = 0; nCntCirle < CIRCLE_MAX; nCntCirle++)
	{
		CManaCircle*pCircle = CManaCircle::GetCircle(nCntCirle);
		if (pCircle != NULL)
		{
			// �T�C�Y�̎擾
			size = pCircle->GetSize() / 2;
			// ���W�̎擾x
			pos = pCircle->GetPos();
			// �X�e�[�W��������̊p�x�̎擾
			fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
			// �X�e�[�W��������̋����̎擾
			fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

			// �k��
			size *= MINIMAP_SCAIL;
			fDistance *= MINIMAP_SCAIL;

			// ���W�̌v�Z
			pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
			pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
			for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
			{
				if (m_apPolygon[nCntMiniMap] == NULL)
				{
					// �|���S���̐���
					m_apPolygon[nCntMiniMap] = CPolygon::Create(
						pos,
						size,
						D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					m_apPolygon[nCntMiniMap]->BindTexture(m_pTexture);

					break;
				}
			}
		}
	}

	// �A�C�e�����}�b�v�ɕ\��

	// ���X�g�̎擾
	std::list<CItem*>::iterator itemIteretor;
	for (itemIteretor = CItem::GetItemlist()->begin(); itemIteretor != CItem::GetItemlist()->end(); itemIteretor++)
	{
		// �T�C�Y�̎擾
		size = (*itemIteretor)->GetSize();
		// ���W�̎擾
		pos = (*itemIteretor)->GetPos();
		// �X�e�[�W��������̊p�x�̎擾
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// �X�e�[�W��������̋����̎擾
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// �k��
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// ���W�̌v�Z
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// �|���S���̐���
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_apPolygon[nCntMiniMap]->BindTexture(m_pTexture);

				break;
			}
		}
	}

	// �G�l�~�[���}�b�v�ɕ\��

	// ���X�g�̎擾
	std::list<CEnemy*>::iterator enemyIteretor;
	for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
	{
		// �T�C�Y�̎擾
		size = (*enemyIteretor)->GetSize();
		// ���W�̎擾
		pos = (*enemyIteretor)->GetPos();
		// �X�e�[�W��������̊p�x�̎擾
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// �X�e�[�W��������̋����̎擾
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// �k��
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// ���W�̌v�Z
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// �|���S���̐���
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apPolygon[nCntMiniMap]->BindTexture(m_pTexture);

				break;
			}
		}
	}

	// �v���C���[���}�b�v�ɕ\��

	if (CPlayer::IsAlive())
	{

		// �T�C�Y�̎擾
		size = CGame::GetPlayer()->GetSize();
		// ���W�̎擾
		pos = CGame::GetPlayer()->GetPos();
		// �X�e�[�W��������̊p�x�̎擾
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// �X�e�[�W��������̋����̎擾
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// �k��
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// ���W�̌v�Z
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;

		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// �|���S���̐���
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

				m_apPolygon[nCntMiniMap]->BindTexture(m_pTexture);

				break;
			}
		}
	}

	// �{�X���}�b�v�ɕ\��
	if (CBoss::IsAlive())
	{

		// �T�C�Y�̎擾
		size = CGame::GetBoss()->GetSize()/2;
		// ���W�̎擾
		pos = CGame::GetBoss()->GetPos();
		// �X�e�[�W��������̊p�x�̎擾
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// �X�e�[�W��������̋����̎擾
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// �k��
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// ���W�̌v�Z
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;

		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// �|���S���̐���
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				m_apPolygon[nCntMiniMap]->BindTexture(m_pTexture);

				break;
			}
		}
	}

	// �|���S���̃A�b�v�f�[�g�̌Ăяo��
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Update();
		}
	}

	
}

//==================================
// �`�揈��
//==================================
void CMiniMap::Draw(void)
{
	// ���݂̃��C�t���`��
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Draw();
		}
	}
}
