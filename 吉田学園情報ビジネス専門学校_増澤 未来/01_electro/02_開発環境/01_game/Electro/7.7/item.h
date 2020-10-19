//=============================================================================
//
// itemヘッダ [item.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ITEME_H_
#define _ITEME_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// クラス定義
//*****************************

//アイテムクラス
class CItem : public CScene3d
{
public:

	// 状態列挙
	typedef enum
	{
		ITEMTYPE_RECOVERY = 0,
		ITEMTYPE_BOMB,
		ITEMTYPE_MAX,
	}ITEMTYPE;

	//メンバ関数
	CItem();
	~CItem();

	static CItem *Create(const D3DXVECTOR3 pos, const ITEMTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static std::list<CItem*>*GetItemlist(void) { return &m_itemList; }
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[ITEMTYPE_MAX]; // テクスチャ
	static std::list<CItem*> m_itemList;  // エネミーリスト
	ITEMTYPE m_itemType;                  // アイテムタイプ
};

#endif