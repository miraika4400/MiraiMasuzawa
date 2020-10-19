//=============================================================================
//
// TitleLogoヘッダ [TitleLogo.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _PRESSLOGO_H_
#define _PRESSLOGO_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "title_logo.h"

//*****************************
//クラス定義
//*****************************

//プレスロゴクラス
class CPressLogo : public CTitleLogo
{
public:
	// メンバ関数
	CPressLogo();
	~CPressLogo();
	static CPressLogo *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ

};

#endif