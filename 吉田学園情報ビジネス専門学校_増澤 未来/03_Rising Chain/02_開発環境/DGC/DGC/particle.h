//=============================================================================
//
// particleヘッダ [particle.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CParticle : public CScene2d
{
public:
	typedef enum
	{
		PARTICLE_BOX = 0,
		PARTICLE_MAX,
	}PARTICLE_TYPE;

	//メンバ関数
	CParticle();
	~CParticle();
	static CParticle *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const D3DCOLOR col, const PARTICLE_TYPE type = PARTICLE_BOX);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTICLE_MAX]; // テクスチャ
	D3DXVECTOR3 m_move; // 移動量
	int m_nLife;        // 寿命
	PARTICLE_TYPE m_type; // パーティクルの種類
};

#endif