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
	// メンバ関数
	CWarningLogo();
	~CWarningLogo();

	static CWarningLogo *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
};

#endif