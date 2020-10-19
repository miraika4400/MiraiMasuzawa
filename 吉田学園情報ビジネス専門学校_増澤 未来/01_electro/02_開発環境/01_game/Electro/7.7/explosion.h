//=============================================================================
//
// explosionヘッダ [explosion.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// クラス定義
//*****************************

// 爆発クラス
class CExplosion : public CScene3d
{
public:
	//メンバ関数
	CExplosion();
	~CExplosion();

	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	int m_nCntAnim; // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
};

#endif