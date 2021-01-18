//=============================================================================
//
// マネージャーヘッダ [manager.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================
// インクルード
//=============================
#include "main.h"

//=============================
// 前方宣言
//=============================1
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CGame;
class CTitle;
class CResult;
class CFade;
class CTutorial;
class CPause;

//=============================
// クラス定義
//=============================

// マネージャークラス
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE=0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX,
	}MODE;

	// メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }

	static CRenderer *GetRenderer(void) { return m_pRenderer; }            // レンダラーの取得処理
	static CInputKeyboard *GetKeyboard(void) { return m_pInputKeyboard; }  // キーボードの取得処理
	static CInputJoypad *GetJoypad(void) { return m_pJoypad; }             // ジョイパッドの取得処理
	static CInputMouse *GetMouse(void) { return m_pInputMouse; }           // マウスの取得処理
	static CSound *GetSound(void) { return m_pSound; }                     // ジョイパッドの取得処理
	static CGame *GetGame(void) { return m_pGame; }                        // ゲームの取得処理
	static CFade * GetFade(void) { return m_pFade; }                       // フェードの取得処理
	static CPause * GetPause(void) { return m_pPause; }                    // ポーズの取得処理
	static void SetActivePause(bool bPause) { m_bPause = bPause; }         // ポーズのセット
	static bool GetActivePause(void) { return m_bPause; }                  // フェードの取得処理

private:
	// メンバ変数
	static MODE m_mode;
	static CRenderer *m_pRenderer;           // レンダラーポインタ
	static CInputKeyboard *m_pInputKeyboard; // キーボードポインタ
	static CInputJoypad*m_pJoypad;           // ジョイパッドポインタ
	static CInputMouse *m_pInputMouse;       // マウスポインタ
	static CSound*m_pSound;                  // サウンドポインタ
	static CGame*m_pGame;                    // ゲームクラスのポインタ
	static CTitle*m_pTitle;                  // タイトルクラスのポインタ
	static CResult*m_pResult;                // タイトルクラスのポインタ
	static CFade*m_pFade;                    // フェードポインタ
	static CTutorial*m_pTutorial;            // チュートリアル
	static CPause*m_pPause;                    // ポーズポインタ
	static bool m_bPause;                    // ポーズフラグ
};

#endif