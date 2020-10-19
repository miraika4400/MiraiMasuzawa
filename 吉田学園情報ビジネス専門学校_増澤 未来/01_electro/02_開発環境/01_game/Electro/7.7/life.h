//=============================================================================
//
// lifeヘッダ [life.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _LIFE_H_
#define _LIFE_H_

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
#define PLAYER_MAX_LIFE 6
#define BOSS_MAX_LIFE 50

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CLife : public CScene
{
public:
	// HPバーのパーツ数
	typedef enum
	{
		BAR_WHILTE=0,
		BAR_RED,
		BAR_GREEN,
		BAR_FRAME,
		BAR_PARTS_MAX,
	}BAR_PARTS;

	// 誰のライフか
	typedef enum
	{
		USER_PLAYER=0,
		USER_BOSS,
		USER_MAX,
	}LIFE_USER;

	// メンバ関数
	CLife();
	~CLife();
	static CLife *Create(LIFE_USER user);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
	CPolygon* m_apPolygon[BAR_PARTS_MAX]; // ライフ描画用のポリゴン
	float m_fRedLife;                     // 赤いゲージがだんだん減ってく時用
	D3DXVECTOR3 m_leftPos;                // ライフの左端
	float m_fBarWidht;                    // 幅
	float m_fBarHeight;                   // 高さ
	int m_nMaxLife;                       // ライフの最大数
	LIFE_USER m_user;                     // 誰のライフか
};

#endif