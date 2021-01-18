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
#include "gauge.h"
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
#include "manager.h"
#include "renderer.h"

//=============================
// マクロ定義
//=============================
#define DEFFICULT_UI_POS  D3DXVECTOR3(150.0f,50.0f,0.0f)             // 難易度UI位置
#define DEFFICULT_UI_SIZE D3DXVECTOR3(120.0f,40.0f ,0.0f)             // 難易度UIサイズ
#define DEFFICULT_UI_COL_NORMAL D3DXCOLOR(0.8f,0.8f ,0.0f,1.0f)      // 難易度UIカラー*ノーマル
#define DEFFICULT_UI_COL_HARD   D3DXCOLOR(0.8f,0.0f ,0.0f,1.0f)      // 難易度UIカラー*ハード

//=============================
// 静的メンバ変数宣言
//=============================
CPlayer *CGame::m_pPlayer = NULL;   // プレイヤー
CCursor *CGame::m_pCursor = NULL;   // カーソル
CCamera *CGame::m_pCamera = NULL;   // カメラ
CStage  *CGame::m_pStage  = NULL;   // ステージ
CBoss   *CGame::m_pBoss   = NULL;   // ボス

CGame::GAME_WAVES CGame::m_wave = CGame::WAVE_NORMAL;                // Wave数
CGame::GAME_DEFFICULT CGame::m_defficult = CGame::DEFFICULT_NORMAL;  // 難易度

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
	
	// カーソル生成
	m_pCursor = CCursor::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

	// Waveの初期化
	m_wave = WAVE_NORMAL;
	SetWave(m_wave);

	// カメラの生成
	m_pCamera = CCamera::Create();

	// スコア生成
	CScore::Create();
	
	// ミニマップの生成
	CMiniMap::Create();
	
	// ボムUIの生成
	CBombUi::Create();

	// サークルの数の初期化
	CManaCircle::SetCircleNum(0);

	// ポーズの初期化
	CManager::SetActivePause(false);

	// 難易度の読み込み
	CEnemy::LoadDefficult();

	// 難易度UIの生成
	
	// ポリゴンのポインタ
	CScene2d*pScene2d = NULL;
	// ポリゴンの生成
	pScene2d = CScene2d::Create();
	// NULLチェック
	if (pScene2d != NULL)
	{
		// サイズの設定
		pScene2d->SetSize(DEFFICULT_UI_SIZE);
		// 座標の設定
		pScene2d->SetPos(DEFFICULT_UI_POS);
		// プライオリティの設定
		pScene2d->SetPriority(OBJTYPE_UI);
		// テクスチャ
		LPDIRECT3DTEXTURE9 pTex = NULL;
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		// 難易度によって読み込むテクスチャ・色を変える
		if (m_defficult == DEFFICULT_NORMAL)
		{// ノーマル
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Normal.png", &pTex);
			// 色の設定
			pScene2d->SetColor(DEFFICULT_UI_COL_NORMAL);
		}
		else
		{// ハード
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Hard.png", &pTex);
			// 色の設定
			pScene2d->SetColor(DEFFICULT_UI_COL_HARD);
		}
		// テクスチャの割り当て
		pScene2d->BindTexture(pTex);
	}
	
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// 開放処理
	CCamera::Release();
	// 難易度の読み込み
	CEnemy::UnloadDefficult();
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
		//CGauge::Create(CGauge::GAUGE_BOSS_HP);
	}
}
