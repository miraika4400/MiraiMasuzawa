//=============================================================================
//
// titleヘッダ [title.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//=============================
// 前方宣言
//=============================
class CPolygon;
class CTitleCamera;

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CTitle : public CScene
{
public:
	// メンバ関数
	CTitle();
	~CTitle();
	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
	CPolygon *m_pPolygon;
	static CTitleCamera*m_pCamera;
};

#endif