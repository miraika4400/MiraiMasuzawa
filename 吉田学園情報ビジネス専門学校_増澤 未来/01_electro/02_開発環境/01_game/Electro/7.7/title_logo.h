//=============================================================================
//
// TitleLogoヘッダ [TitleLogo.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//クラス定義
//*****************************

//タイトルロゴクラス
class CTitleLogo : public CScene2d
{
public:
	// メンバ関数
	CTitleLogo();
	~CTitleLogo();
	static CTitleLogo *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
	int m_nCntAnim;       // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
	int m_nCntSetAnim;    
	bool m_bAnim;
};

#endif