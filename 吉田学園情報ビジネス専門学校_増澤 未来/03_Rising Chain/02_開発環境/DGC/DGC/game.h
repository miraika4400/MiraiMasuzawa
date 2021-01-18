//=============================================================================
//
// gameヘッダ [game.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"


//=============================
// 前方宣言
//=============================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CCamera;
class CPlayer;
class CLight;
class CCourse;
//*****************************
//マクロ定義
//*****************************
#define MAX_PLAYER_NUM 4

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CGame : public CScene
{
public:
	// メンバ関数
	CGame();
	~CGame();
	static CGame *Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// リザルトフラグの取得・セット
	static bool GetResultFlag(void) { return m_bResult; }
	static void SetResultFlag(bool bResult) { m_bResult = bResult; }

	static CCamera *GetCamera(int nIndex) { return m_pCamera[nIndex]; }  // カメラの取得処理
	static CPlayer *GetPlayer(int nIndex) { return m_pPlayer[nIndex]; }  // プレイヤーの取得処理
	static CCourse *GetCourse(void) { return m_pCourse; }                // コースの取得
	static int GetPlayerNum(void) { return m_nNumPlayer; }                 // プレイヤー人数の取得
	static void SetPlayerNum(int nNumPlayer){ m_nNumPlayer = nNumPlayer; } // プレイヤー人数のセット
private:
	// メンバ変数
	static CCamera*m_pCamera[MAX_PLAYER_NUM]; // カメラ
	static CPlayer*m_pPlayer[MAX_PLAYER_NUM]; // プレイヤー
	static CLight *m_pLight;                  // ライト
	static CCourse*m_pCourse;                 // コース
	static int m_nNumPlayer;                  // プレイヤー人数
	static bool m_bResult;                           // リザルト
	int m_nCntResult;                         // リザルト遷移時のカウント
};

#endif