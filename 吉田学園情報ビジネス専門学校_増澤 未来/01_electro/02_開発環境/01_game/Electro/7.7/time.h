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
	// メンバ関数
	CTime();
	~CTime();
	static CTime *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int GetTime(void) { return m_nTime; }
	static void SetPauseTime(void) { m_nPauseCurrentTime = timeGetTime(); }
private:
	// メンバ変数
	CNumber* m_apNumber[MAX_TIME_NUM]; // タイム描画用のポリゴン
	static int m_nTime;                // 経過時間
	static int m_nPauseCurrentTime;           // ポーズに入った時間
	int m_nPauseTime;                  // ポーズ画面中のタイム
	int m_nCurrentTime;                // 開始時間
};

#endif