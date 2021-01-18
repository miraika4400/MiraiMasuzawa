//===================================================
//
//    �A�C�e���N���X�̏���[item.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************
// �e�N�X�`���̃p�X
#define ITEM_SIZE 40
#define RECOVERY_LIFE 2
#define ITEM_TEXTURE_PATH_RECOVERY "./data/Textures/HP2.png"
#define ITEM_TEXTURE_PATH_BOMB "./data/Textures/Bomb.png"

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::ITEMTYPE_MAX] = {};
std::list<CItem*> CItem::m_itemList;// �A�C�e�����X�g

//==================================
// �R���X�g���N�^
//==================================
CItem::CItem() :CScene3d(OBJTYPE_ITEM)
{
}

//==================================
// �f�X�g���N�^
//==================================
CItem::~CItem()
{
}

//==================================
// �N���G�C�g
//==================================
CItem * CItem::Create(const D3DXVECTOR3 pos, const ITEMTYPE type)
{
	// �������̊m��
	CItem *pItem = new CItem;
	
	
	pItem->m_itemType = type;

	// ����������
	pItem->Init();

	pItem->SetPos(pos);
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pItem->SetPriority(OBJTYPE_ITEM);
	// ���X�g�ɒǉ�
	m_itemList.push_back(pItem);
	return pItem;
}

//==================================
// ���[�h
//==================================
HRESULT CItem::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_PATH_RECOVERY, &m_apTexture[ITEMTYPE_RECOVERY]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_PATH_BOMB, &m_apTexture[ITEMTYPE_BOMB]);

	return S_OK;
}

//==================================
// �A�����[�h
//==================================
void CItem::Unload(void)
{
	for (int nCntTex = 0; nCntTex < sizeof(m_apTexture) / sizeof(LPDIRECT3DTEXTURE9); nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// ����������
//==================================
HRESULT CItem::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_apTexture[m_itemType]);

	// �T�C�Y�̐ݒ�
	SetSize(D3DXVECTOR3(ITEM_SIZE, ITEM_SIZE, 0.0f));
	return S_OK;
}

//==================================
// �I������
//==================================
void CItem::Uninit(void)
{
	// ���X�g����폜
	std::list<CItem*>::iterator iteretor;
	for (iteretor = m_itemList.begin(); iteretor != m_itemList.end(); )
	{
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = m_itemList.erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}
	}

	CScene3d::Uninit();
}

//==================================
// �X�V����
//==================================
void CItem::Update(void)
{
	if (CPlayer::IsAlive())
	{
		// ���W�A�T�C�Y�̎擾
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		// �v���C���[�̍��W�A�T�C�Y�̎擾
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
		D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();

		if (pos.x - size.x <= playerPos.x + playerSize.x / 3 &&
			pos.x + size.x >= playerPos.x - playerSize.x / 3 &&
			pos.y - size.y <= playerPos.y + playerSize.y / 3 &&
			pos.y + size.y >= playerPos.y - playerSize.y / 3)
		{
			switch (m_itemType)
			{
			case ITEMTYPE_RECOVERY:
				CGame::GetPlayer()->RecoveryLife(RECOVERY_LIFE);
				break;
			case ITEMTYPE_BOMB:
				CGame::GetPlayer()->SetBomb(true);
				break;
			default:
				break;
			}
			Uninit();
		}

	}

}

//==================================
// �`�揈��
//==================================
void CItem::Draw(void)
{
	CScene3d::Draw();
}
