//=============================================================================
//
// timeヘッダ [time.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TIME_H_
#define _TIME_H_

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
#define MAX_TIME_NUM 4 // タイムに必要なナンバーの数

//*****************************
//クラス定義
//*****************************

//タイムクラス
class CTime : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CTime();
	~CTime();

	//static
	static CTime *Create(void);                                             // クラス生成
	static int GetTime(void) { return m_nTime; }                            // タイムの取得
	static void SetPauseTime(void) { m_nPauseCurrentTime = timeGetTime(); } // ポーズ画面に入った時間のセット

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	
private:
	//============
	// メンバ変数
	//============
	static int m_nTime;                // 経過時間
	static int m_nPauseCurrentTime;    // ポーズに入った時間
	CNumber* m_apNumber[MAX_TIME_NUM]; // タイム描画用のポリゴン
	int m_nPauseTime;                  // ポーズ画面中のタイム
	int m_nCurrentTime;                // 開始時間
};

#endif