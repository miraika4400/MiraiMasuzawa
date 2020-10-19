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
	//メンバ関数
	CEffect();
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR col,const LPDIRECT3DTEXTURE9 pTexture);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	int m_nLife;                          // 寿命
};

#endif