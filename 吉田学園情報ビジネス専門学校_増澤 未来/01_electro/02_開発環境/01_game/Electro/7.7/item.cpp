//===================================================
//
//    アイテムクラスの処理[item.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"

//*****************************
// マクロ定義
//*****************************
// テクスチャのパス
#define ITEM_SIZE 40      // アイテムのサイズ
#define RECOVERY_LIFE 2   // 回復アイテムの回復量
// テクスチャのパス
#define ITEM_TEXTURE_PATH_RECOVERY "./data/Textures/HP2.png" // 回復アイテム
#define ITEM_TEXTURE_PATH_BOMB "./data/Textures/Bomb.png"    // ボムアイテム

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::ITEMTYPE_MAX] = {};// テクスチャ
std::list<CItem*> CItem::m_itemList;// アイテムリスト

//==================================
// コンストラクタ
//==================================
CItem::CItem() :CScene3d(OBJTYPE_ITEM)
{
}

//==================================
// デストラクタ
//==================================
CItem::~CItem()
{
}

//==================================
// クリエイト
//==================================
CItem * CItem::Create(const D3DXVECTOR3 pos, const ITEMTYPE type)
{
	// メモリの確保
	CItem *pItem = new CItem;
	
	// タイプの設定
	pItem->m_itemType = type;

	// 初期化処理
	pItem->Init();

	pItem->SetPos(pos);
	// オブジェクトタイプの設定
	pItem->SetObjType(OBJTYPE_ITEM);
	// リストに追加
	m_itemList.push_back(pItem);
	return pItem;
}

//==================================
// ロード
//==================================
HRESULT CItem::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_PATH_RECOVERY, &m_apTexture[ITEMTYPE_RECOVERY]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_PATH_BOMB, &m_apTexture[ITEMTYPE_BOMB]);

	return S_OK;
}

//==================================
// アンロード
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
// 初期化処理
//==================================
HRESULT CItem::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_apTexture[m_itemType]);

	// サイズの設定
	SetSize(D3DXVECTOR3(ITEM_SIZE, ITEM_SIZE, 0.0f));
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CItem::Uninit(void)
{
	// リストから削除
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
// 更新処理
//==================================
void CItem::Update(void)
{
	if (CPlayer::IsAlive())
	{
		// 座標、サイズの取得
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 size = GetSize();

		// プレイヤーの座標、サイズの取得
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
// 描画処理
//==================================
void CItem::Draw(void)
{
	CScene3d::Draw();
}
