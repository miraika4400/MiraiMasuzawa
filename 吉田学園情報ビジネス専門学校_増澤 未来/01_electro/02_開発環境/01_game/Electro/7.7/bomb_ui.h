//=============================================================================
//
// bomb_uiヘッダ [bomb_ui.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BOMB_UI_H_
#define _BOMB_UI_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//前方宣言
//*****************************
class CPolygon;

//*****************************
//クラス定義
//*****************************

//ボムUIクラス
class CBombUi : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CBombUi();
	~CBombUi();
	// static
	static CBombUi *Create(void); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
	CPolygon* m_pPolygon;                 // ライフ描画用のポリゴン

};

#endif