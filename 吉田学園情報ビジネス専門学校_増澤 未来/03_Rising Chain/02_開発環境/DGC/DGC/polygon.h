//=============================================================================
//
// polygonヘッダ [polygon.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _POLYGON_H_
#define _POLYGON_H_

//*****************************
//インクルード
//*****************************
#include "main.h"

//*****************************
//クラス定義
//*****************************

//ナンバークラス
class CPolygon
{
public:
	// メンバ関数
	CPolygon();
	~CPolygon();
	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetVertexPos(D3DXVECTOR3 pos[NUM_VERTEX]);
	void SetColor(const D3DXCOLOR col);
	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
private:

	// メンバ変数
	LPDIRECT3DTEXTURE9        m_pTexture;	 // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;	 // 頂点バッファへのポイン
	D3DXVECTOR3 m_pos;  // 座標
	D3DXVECTOR3 m_size;  // サイズ
};

#endif