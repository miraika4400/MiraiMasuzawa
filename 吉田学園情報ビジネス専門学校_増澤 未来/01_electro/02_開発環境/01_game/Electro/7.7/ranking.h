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
#define MAX_RANK 8

//*****************************
//クラス定義
//*****************************

//ランキングクラス
class CRanking : public CScene
{
public:
	// メンバ関数
	CRanking();
	~CRanking();
	static CRanking *Create(void);
	static void GetRanking(void);
	static int SetRanking(void);
	static void SetRanking(int nScore);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
private:
	// メンバ変数
	CNumber* m_apRankNumber[MAX_RANK];                   // ランキング
	CNumber* m_apScoreNumber[MAX_RANK][MAX_SCORE_DIGIT]; // スコアポリゴン
	static int m_nNowScore;                              // 今のプレイのスコア
	static CNumber* m_pNowScore[MAX_SCORE_DIGIT];        // スコアポリゴン
	static int m_nRanking[MAX_RANK];                     // スコアデータ
	int m_nCntFlash;

};

#endif