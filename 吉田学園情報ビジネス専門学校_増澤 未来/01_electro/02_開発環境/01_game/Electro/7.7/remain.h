//=============================================================================
//
// gageヘッダ [gage.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _REMAIN_H_
#define _REMAIN_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//前方宣言
//*****************************
class CPolygon;
class CNumber;

//*****************************
//クラス定義
//*****************************

//残機クラス
class CRemain : public CScene
{
public:

	// メンバ関数
	CRemain();
	~CRemain();
	static CRemain *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
	CPolygon* m_pPolygon; // ライフ描画用のポリゴン
	CNumber *m_pNumber;
	int m_nRemain;
};

#endif