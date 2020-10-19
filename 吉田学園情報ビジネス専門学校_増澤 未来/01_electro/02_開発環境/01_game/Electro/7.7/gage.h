//=============================================================================
//
// gageヘッダ [gage.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _GAGE_H_
#define _GAGE_H_

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
//クラス定義
//*****************************

//ゲージクラス
class CGage : public CScene
{
public:
	// HPバーのパーツ数
	typedef enum
	{
		BAR_WHILTE=0,
		BAR_RED,
		BAR_FRONT,
		BAR_FRAME,
		BAR_PARTS_MAX,
	}BAR_PARTS;

	// 誰のライフか
	typedef enum
	{
		GAGE_PLAYER_HP=0,
		GAGE_PLAYER_MP,
		GAGE_BOSS_HP,
		GAGE_MAX,
	}GAGE_TYPE;

	// メンバ関数
	CGage();
	~CGage();
	static CGage *Create(GAGE_TYPE type);
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
	int m_nMaxNum;                       // ライフの最大数
	GAGE_TYPE m_gageType;                     // 誰のライフか
};

#endif