//=============================================================================
//
// ランキングヘッダ [ranking.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"
#include "score.h"

//*****************************
//前方宣言
//*****************************
class CNumber;

//*****************************
//マクロ定義
//*****************************
#define MAX_RANK 8 // ランキングの順位数

//*****************************
//クラス定義
//*****************************

//ランキングクラス
class CRanking : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CRanking();
	~CRanking();

	static CRanking *Create(void);      // クラス生成
	static void GetRanking(void);       // ランキング取得
	static int SetRanking(void);        // ランキングセット(サーバー通信)
	static void SetRanking(int nScore); // ランキングセット(ゲーム内)

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画
	
private:
	//============
	// メンバ変数
	//============
	static int m_nNowScore;                              // 今のプレイのスコア
	static CNumber* m_pNowScore[MAX_SCORE_DIGIT];        // スコアポリゴン
	static int m_nRanking[MAX_RANK];                     // スコアデータ
	CNumber* m_apRankNumber[MAX_RANK];                   // ランキング
	CNumber* m_apScoreNumber[MAX_RANK][MAX_SCORE_DIGIT]; // スコアポリゴン
	int m_nCntFlash;                                     // 点滅カウント

};

#endif