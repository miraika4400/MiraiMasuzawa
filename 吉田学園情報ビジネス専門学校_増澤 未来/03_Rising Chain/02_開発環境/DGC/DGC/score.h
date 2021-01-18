//=============================================================================
//
// scoreヘッダ [score.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//前方宣言
//*****************************
class CNumber;

//*****************************
//マクロ定義
//*****************************
#define MAX_SCORE_DIGIT 6  // スコアの最大桁数

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CScore : public CScene
{
public:
	// メンバ関数
	CScore();
	~CScore();
	static CScore *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddScore(int nPoint) { m_nScore += nPoint; }
	static int GetScore(void) { return m_nScore; }
private:
	// メンバ変数
	CNumber* m_apNumber[MAX_SCORE_DIGIT];
	static int m_nScore;

};

#endif