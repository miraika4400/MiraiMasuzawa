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
#include "light.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "player.h"
#include "course.h"
#include "checkpoint.h"
#include "character.h"
#include "cpu_point.h"
#include "cpu.h"
#include "rank.h"
#include "item_point.h"
#include "item.h"

//=============================
// 静的メンバ変数宣言
//=============================
CCamera *CGame::m_apCamera[MAX_PLAYER_NUM] = {};           // カメラクラスポインタ
CLight  *CGame::m_pLight = NULL;                           // ライトクラスポインタ
CCharacter * CGame::m_apCharacter[MAX_CHARACTER_NUM] = {}; // キャラクタークラスポインタ
int CGame::m_nNumCaracter = 0;                             // 生成したキャラクターの数
int CGame::m_nNumPlayer = 1;                               // プレイヤー数
CCpuPoint *CGame::m_pCpuPoint = NULL;                      // CPU用コースデータ
CCheckPoint *CGame::m_pCheckPoint = NULL;                  // チェックポイント
CItemPoint *CGame::m_pItemPoint = NULL;                    // アイテムポイント
CCourse    *CGame::m_pCourse = NULL;                       // コースポインタ

//=============================
// コンストラクタ
//=============================
CGame::CGame()
{
	// 変数のクリア
	m_nNumCaracter = 0;
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

	// ポーズの初期化
	CManager::SetActivePause(false);

	// ライトクラスの生成
	m_pLight = new CLight;
	// ライトクラスの初期化
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}

	// 背景の生成
	CBg::Create();

	// 順位管理クラスの生成
	CRank::Create();

	// コース生成
	m_pCourse = CCourse::Create();

	// チェックポイントの生成
	m_pCheckPoint = CCheckPoint::Create();

	// CPU用のコースデータ
	m_pCpuPoint = CCpuPoint::Create();
	
	// アイテム座標
	m_pItemPoint = CItemPoint::Create();

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		// カメラクラスの生成
		m_apCamera[nCnt] = CCamera::Create(nCnt);
		// プレイヤー生成
		m_apCharacter[m_nNumCaracter] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), 0);
		// キャラクター数加算
		m_nNumCaracter++;	
	}
	
	// CPU生成
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(-150.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_RUIN, 5);
	// キャラクター数加算
	m_nNumCaracter++;

	// CPU生成
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_STAR , 4);
	// キャラクター数加算
	m_nNumCaracter++;
	
	// CPU生成
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(-150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_RUIN, 2);
	// キャラクター数加算
	m_nNumCaracter++;

	// CPU生成
	m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(150.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), CCpu::CPU_TYPE_STAR, 10);
	// キャラクター数加算
	m_nNumCaracter++;



	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// カメラクラスの解放処理
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Uninit();
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
}


//=============================
// 更新処理
//=============================
void CGame::Update(void)
{
#ifdef _DEBUG
	// デバッグ用画面遷移コマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

#endif // _DEBUG
	CItem::RotationItem();

	// カメラクラスの更新処理
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Update();
		}
	}
}


//=============================
// 描画処理
//=============================
void CGame::Draw(void)
{
	// カメラのセット
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}