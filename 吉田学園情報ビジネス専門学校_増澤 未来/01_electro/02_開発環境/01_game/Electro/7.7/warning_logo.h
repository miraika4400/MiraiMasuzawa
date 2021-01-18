//=============================================================================
//
// warning_logoヘッダ [warning_logo.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _WARNING_LOGO_H_
#define _WARNING_LOGO_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//=============================
// 前方宣言
//=============================
class CPolygon;

//*****************************
//クラス定義
//*****************************

//ワーニングロゴクラス
class CWarningLogo : public CScene3d
{
public:
	//============
	// メンバ関数
	//============
	CWarningLogo();
	~CWarningLogo();
	// static
	static CWarningLogo *Create(void); // クラス生成
	static HRESULT Load(void);         // テクスチャロード
	static void Unload(void);          // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
};

#endif