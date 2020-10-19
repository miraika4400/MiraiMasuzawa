////////////////////////////////////////////////////
//
//    ゲームクラスの処理[game.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//=============================
// インクルード
//=============================
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "number.h"
#include "score.h" 
#include "gage.h"
#include "effect.h"
#include "mouse.h"
#include "cursor.h"
#include "stage.h"
#include "camera.h"
#include "minimap.h"
#include "boss.h"
#include "fade.h"
#include "item.h"
#include "bomb.h"
#include "bomb_ui.h"
#include "manacircle.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"

//=============================
// 静的メンバ変数宣言
//=============================
CPlayer *CGame::m_pPlayer = NULL;   // プレイヤー
CCursor *CGame::m_pCursor = NULL;   // カーソル
CCamera *CGame::m_pCamera = NULL;   // カメラ
CStage  *CGame::m_pStage  = NULL;   // ステージ
CBoss   *CGame::m_pBoss   = NULL;   // ボス
CGame::GAME_WAVES CGame::m_wave = CGame::WAVE_NORMAL; // Wave数

//=============================
// コンストラクタ
//=============================
CGame::CGame()
{
}

//=============================
// デストラクタ
//=============================
CGame::~CGame()
{
}

//=============================
// クリエイト
//=============================
CGame * CGame::Create(void)
{
	// メモリの確保
	CGame *pGame = new CGame;
	// 初期化
	pGame->Init();
	return pGame;
}

//=============================
// 初期化処理
//=============================
HRESULT CGame::Init(void)
{
	// タイムクラスの生成
	CTime::Create();
	// 背景生成
	CBg::Create();
	// ステージの生成
	m_pStage = CStage::Create();
	
	// プレイヤー生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0, -400.0f, 0.0f));
	//// カーソル生成
	m_pCursor = CCursor::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

	// Waveの初期化
	m_wave = WAVE_NORMAL;
	SetWave(m_wave);

	// カメラの生成
	m_pCamera = CCamera::Create();

	// スコア生成
	CScore::Create();
	// ゲージ生成
	CGage::Create(CGage::GAGE_PLAYER_HP);
	CGage::Create(CGage::GAGE_PLAYER_MP);
	// ミニマップの生成
	CMiniMap::Create();
	// ボムUIの生成
	CBombUi::Create();

	// サークルの数の初期化
	CManaCircle::SetCircleNum(0);

	// ポーズの初期化
	CManager::SetActivePause(false);
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// 開放処理
	CCamera::Release();

	// 開放処理
	Release();
}


//=============================
// 更新処理
//=============================
void CGame::Update(void)
{
	// サークル数の管理
	CManaCircle::SpawnManager();

	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}
	
	if (m_wave == WAVE_NORMAL)
	{
		CEnemy::SpawnManager();
	}

#ifdef _DEBUG
	// デバックコマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SetWave(WAVE_BOSS);
	}

#endif // _DEBUG

}


//=============================
// 描画処理
//=============================
void CGame::Draw(void)
{
	CGame::GetCamera()->SetCamera();
}

//=============================
// WAVEセット
//=============================
void CGame::SetWave(GAME_WAVES wave)
{
	m_wave = wave;

	if (m_wave == WAVE_BOSS)
	{
		// ボスの生成
		m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		CGage::Create(CGage::GAGE_BOSS_HP);
	}
}
