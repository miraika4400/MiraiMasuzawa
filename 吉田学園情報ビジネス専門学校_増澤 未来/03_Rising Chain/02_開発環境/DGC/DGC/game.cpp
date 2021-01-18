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
#include "bg.h"
#include "number.h"
#include "score.h" 
#include "mouse.h"
#include "camera.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "course.h"
#include "player.h"
#include "light.h"
#include "item.h"
#include "rank.h"
#include "file.h"
#include "accelfloor.h"
#include "sound.h"
#include "renderer.h"

//=============================
// マクロ定義
//=============================
#define FILE_NAME "data/Texts/Item.txt"

//=============================
// マクロ定義
//=============================
#define RESULT_COUNT 200

//=============================
// 静的メンバ変数宣言
//=============================
CCamera *CGame::m_pCamera[MAX_PLAYER_NUM] = {};   // カメラ
CPlayer *CGame::m_pPlayer[MAX_PLAYER_NUM] = {};   // プレイヤー
CLight *CGame::m_pLight = NULL;		// ライトクラスのポインタ変数
CCourse *CGame::m_pCourse = NULL;   // コースクラスのポインタ変数
int CGame::m_nNumPlayer = 1;        // プレイヤー人数
bool CGame::m_bResult = false;      // リザルト

//=============================
// コンストラクタ
//=============================
CGame::CGame()
{
	// 変数のクリア
	m_nCntResult = 0;
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
CGame * CGame::Create()
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
	// ポーズの初期化
	CManager::SetActivePause(false);
	
	// マップの生成
	m_pCourse = CCourse::Create(CCourse::COURSE_EASY);

	float fPosX = (((300 * 2)*m_nNumPlayer) / 2) - 300;// X軸位置
	float fPosZ = 300.0f*m_nNumPlayer;
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		// プレイヤーの生成
		m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), nCntPlayer);
		// カメラの生成
		m_pCamera[nCntPlayer] = CCamera::Create(nCntPlayer);
		// X軸をずらす
		fPosX -= 300.0f * 2.0f;
		// Y軸をずらす
		fPosZ -= 200.0f;
	}
	//ライトクラスの生成
	m_pLight = new CLight;
	// 初期化
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}
	
	// 変数の初期化
	m_bResult = false;
	m_nCntResult = 0;
    // アイテム配置
    CFile::Create()->Read(FILE_NAME);
	if (m_nNumPlayer == 3)
	{
		LPDIRECT3DTEXTURE9 pTexture;        // テクスチャへのポインタ
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/Textures/NoEntry.png", &pTexture);   // 二人
		CScene2d * pScene2d = CScene2d::Create();
		// 座標の設定
		pScene2d->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 2, 0.0f));
		// サイズの設定
		pScene2d->SetSize(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f));
		// オブジェクトタイプの設定
		pScene2d->SetPriority(OBJTYPE_UI);
		// テクスチャのセット
		pScene2d->BindTexture(pTexture);
	}
	
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// カメラ
	for (int nCntCamera = 0; nCntCamera < m_nNumPlayer; nCntCamera++)
	{
		if (m_pCamera[nCntCamera] != NULL)
		{
			m_pCamera[nCntCamera]->Uninit();
			delete m_pCamera[nCntCamera];
			m_pCamera[nCntCamera] = NULL;
		}
	}
	// ライト
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// 開放処理
	Release();

    CFile::GetInstance()->Uninit();
}


//=============================
// 更新処理
//=============================
void CGame::Update(void)
{
	// アイテム回転
	CItem::ItemRotasion();

	// カメラクラスのアップデート
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		if (m_pCamera[nCntPlayer] != NULL)
		{
			m_pCamera[nCntPlayer]->Update();
		}
	}
	// 順位の管理
	CRank::RankManage();

	if (!m_bResult)
	{
		bool bGoal = true;
		// プレイヤー全員がゴール状態化の確認
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (m_pPlayer[nCnt] != NULL)
			{
				if (!m_pPlayer[nCnt]->GetGoalFlag())
				{
					bGoal = false;
					break;
				}
			}
		}

		if (bGoal)
		{
			// カウントを進める
			m_nCntResult++;
			// 言ってのカウントに達したら
			if (m_nCntResult >= RESULT_COUNT)
			{
				// カウントの初期化
				m_nCntResult = 0;
				// リザルト状態にする
				m_bResult = true;
				// ゲームBGM停止
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM_GAME);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);
			}
		}
	}
	else
	{// リザルト状態の時
		if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
		{
			// タイトルに遷移
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
}


//=============================
// 描画処理
//=============================
void CGame::Draw(void)
{
}