//=============================================================================
//
// MiniMapヘッダ [minimap.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

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
//マクロ定義
//*****************************
#define MAX_MINIMAP_POLYGON 256

//*****************************
//クラス定義
//*****************************

//ミニマップクラス
class CMiniMap : public CScene
{
public:
	// メンバ関数
	CMiniMap();
	~CMiniMap();
	static CMiniMap *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
	CPolygon* m_apPolygon[MAX_MINIMAP_POLYGON];  // ミニマップ描画用のポリゴン
	static D3DXVECTOR3 m_stagePos;               // ステージのPOS
};

#endif