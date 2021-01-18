//=============================================================================
//
// evolution_effectヘッダ [evolution_effect.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _EVOLUTION_EFFECT_H_
#define _EVOLUTION_EFFECT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// マクロ定義
//*****************************

//*****************************
// 前方宣言
//*****************************


//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CEvoEffect : public CModel
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	//メンバ関数
	//==========================
	CEvoEffect();
	~CEvoEffect();
	static CEvoEffect *Create(int nPlayerNum);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static Model m_model;   // モデル構造体
	int m_nPlayerNum;       // プレイヤー番号
	int m_nLife;            // 寿命
	float m_fAlfa;          // アルファ値
	D3DXCOLOR m_col;        // カラー
	D3DXCOLOR m_distCol;    // カラー目標値
};
#endif