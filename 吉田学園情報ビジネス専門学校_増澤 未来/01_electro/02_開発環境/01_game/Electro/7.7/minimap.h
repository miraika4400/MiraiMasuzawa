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
	//============
	// メンバ関数
	//============
	CMiniMap();
	~CMiniMap();

	static CMiniMap *Create(void); // クラス生成
	static HRESULT Load(void);     // テクスチャロード
	static void Unload(void);      // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
	static D3DXVECTOR3 m_stagePos;               // ステージのPOS
	CPolygon* m_apPolygon[MAX_MINIMAP_POLYGON];  // ミニマップ描画用のポリゴン
};

#endif