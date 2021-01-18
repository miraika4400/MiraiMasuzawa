//=============================================================================
//
// rankヘッダ [rank.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _RANK_H_
#define _RANK_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//クラス定義
//*****************************

// ランクUIクラス
class CRank : public CScene2d
{
public:
	//============
	// メンバ関数
	//============
	CRank();
	~CRank();

	static CRank *Create(int nPlayerNum); // クラス生成
	static HRESULT Load(void);            // テクスチャロード
	static void Unload(void);             // テクスチャアンロード
	static void RankManage(void);         // 順位の管理

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ

	D3DXVECTOR3 m_resultPos;       // リザルト時の座標
	D3DXVECTOR3 m_resultSize;      // リザルト時のサイズ

	int m_nCntAnim;                // アニメーションカウント
	int m_nAnimX;                  // アニメーションX軸
	int m_nAnimY;                  // アニメーションY軸
	int m_nPlayerNum;              // プレイヤー番号
};

#endif