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
	//============
	// メンバ関数
	//============
	CExplosion();
	~CExplosion();
	// static
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col); // クラス生成
	static HRESULT Load(void);													 // テクスチャロード
	static void Unload(void);													 // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	int m_nCntAnim; // アニメーションカウント
	int m_nAnimX;   // アニメーションX軸
	int m_nAnimY;   // アニメーションY軸
};

#endif