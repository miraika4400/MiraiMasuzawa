//=============================================================================
//
// tutorialヘッダ [tutorial.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// 前方宣言
//*****************************
class CPolygon;

//*****************************
// マクロ定義
//*****************************
#define TUTORIAL_NUM 1

//*****************************
//クラス定義
//*****************************

// チュートリアルクラス
class CTutorial : public CScene
{
public:
	// メンバ関数
	CTutorial();
	~CTutorial();
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[TUTORIAL_NUM];        // テクスチャへのポインタ
	CPolygon *m_pPolygon;
	int m_nNumTutorial;

};

#endif