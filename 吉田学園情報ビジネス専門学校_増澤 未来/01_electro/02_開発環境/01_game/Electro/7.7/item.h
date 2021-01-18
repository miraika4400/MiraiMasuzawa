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
	//============
	// 状態列挙
	//============
	typedef enum
	{
		ITEMTYPE_RECOVERY = 0, // 回復
		ITEMTYPE_BOMB,         // ボム
		ITEMTYPE_MAX,          // タイプの最大数
	}ITEMTYPE;

	//============
	//メンバ関数
	//============
	CItem();
	~CItem();

	static CItem *Create(const D3DXVECTOR3 pos, const ITEMTYPE type);   // クラス生成
	static HRESULT Load(void);                                          // テクスチャロード
	static void Unload(void);                                           // テクスチャアンロード
	static std::list<CItem*>*GetItemlist(void) { return &m_itemList; }  // リストの取得

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	
private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[ITEMTYPE_MAX]; // テクスチャ
	static std::list<CItem*> m_itemList;                 // エネミーリスト
	ITEMTYPE m_itemType;   // アイテムタイプ
};

#endif