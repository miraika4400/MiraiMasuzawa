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
class CPlayer;
class CCursor;
class CCamera;
class CStage;
class CBoss;

//*****************************
//クラス定義
//*****************************

//ゲームクラス
class CGame : public CScene
{
public:

	//*****************************
	// 列挙定義
	//*****************************

	// 難易度
	typedef enum
	{
		DEFFICULT_NORMAL = 0,
		DEFFICULT_HARD,
		DEFFICULT_MAX,
	}GAME_DEFFICULT;
	// ウェーブ
	typedef enum
	{
		WAVE_NORMAL = 0,
		WAVE_BOSS,
		WAVE_MAX,
	}GAME_WAVES;

	//*****************************
	// メンバ関数
	//*****************************
	CGame();
	~CGame();

	static CGame *Create(void); // クラス生成

	HRESULT Init(void);// 初期化
	void Uninit(void); // 終了
	void Update(void); // 更新
	void Draw(void);   // 描画
	
	static CPlayer *GetPlayer(void) { return m_pPlayer; }                           // プレイヤーの取得処理
	static CCursor *GetCursor(void) { return m_pCursor; }                           // カーソルの取得処理
	static CCamera *GetCamera(void) { return m_pCamera; }                           // カメラの取得処理
	static CStage *GetStage(void) { return m_pStage; }                              // ステージ取得
	static CBoss *GetBoss(void) { return m_pBoss; }                                 // ボス取得

	static GAME_WAVES GetWave(void) { return m_wave; }                              // ウェーブ取得
	static void SetWave(GAME_WAVES wave);                                           // ウェーブセット
	static GAME_DEFFICULT GetDefficult(void) { return m_defficult; }                // 難易度取得
	static void SetDefficult(GAME_DEFFICULT defficult) { m_defficult= defficult; }  // 難易度セット

private:

	//*****************************
	// メンバ変数
	//*****************************
	static CPlayer*       m_pPlayer;   // プレイヤーポインタ
	static CCursor*       m_pCursor;   // カーソルポインタ
	static CCamera*       m_pCamera;   // カメラ
	static CStage*        m_pStage;    // ステージ
	static CBoss*         m_pBoss;     // ボス
	static GAME_WAVES     m_wave;      // WAVE数管理用
	static GAME_DEFFICULT m_defficult; // 難易度
};

#endif