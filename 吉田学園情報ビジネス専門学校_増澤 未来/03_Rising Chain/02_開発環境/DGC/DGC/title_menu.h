//=============================================================================
//
//  [titlemenu.h]
// Author : Masuzawa Mirai
//
//=============================================================================

#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_
//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

class CPolygon;

//***************************************************************************************
// クラス定義
//***************************************************************************************

// ポーズクラス
class CTitleMenu : public CScene
{
public:
	//============
	// 状態列挙
	//============
	// メニューの構成パーツ
	typedef enum
	{
		//BACK = 0, // メニュー背景
		TWO,      // 二人
		THREE,    // 三人
		FOUR,     // 四人
		PARTS_MAX // パーツの数
	}MENU_PARTS;

	//============
	// メンバ関数
	//============
	CTitleMenu();
	~CTitleMenu();

	// static
	static CTitleMenu* Create(void); // クラス生成
	static HRESULT Load(void);       // テクスチャロード
	static void Unload(void);        // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTS_MAX]; // テクスチャ
	CPolygon* m_pPolygon[PARTS_MAX];                  // ポリゴン
	int m_nMenu;                                      // メニュー選択用
	bool m_bMove;                                     // 移動フラグ
};

#endif // !_TITEL_H_