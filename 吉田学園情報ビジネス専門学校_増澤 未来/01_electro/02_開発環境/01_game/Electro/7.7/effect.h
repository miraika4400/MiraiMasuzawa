//=============================================================================
//
// effectヘッダ [effect.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// クラス定義
//*****************************

// エフェクトクラス
class CEffect : public CScene3d
{
public:
	//============
	// メンバ関数
	//============
	CEffect();
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR col,const LPDIRECT3DTEXTURE9 pTexture); // クラス生成
	static HRESULT Load(void); // テクスチャロード
	static void Unload(void);  // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画
private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	int m_nLife;                          // 寿命
};

#endif