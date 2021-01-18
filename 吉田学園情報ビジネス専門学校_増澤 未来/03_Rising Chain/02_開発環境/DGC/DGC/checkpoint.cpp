////////////////////////////////////////////////////
//
//    particleクラスの処理[particle.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "checkpoint.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "player.h"


//*****************************
// マクロ定義
//*****************************
#define CHECKPOINT_RADIUS_EASY 3500     // 当たり判定の半径
#define CHECKPOINT_RADIUS_NORMAL 2700     // 当たり判定の半径

// テキストのパス
#define CHECKPOINT_TEXT_EASY   "data/Texts/EasyMapCheckpoint.txt"   // イージー
#define CHECKPOINT_TEXT_NORMAL "data/Texts/NormalMapCheckpoint.txt" // ノーマル
#define CHECKPOINT_TEXT_HARD   "data/Texts/HardMapCheckpoint.txt"   // ハード

// チェックポイント配置モード切り替えマクロ
#define SET_MODE
#undef  SET_MODE

//*****************************
// 静的メンバ変数宣言
//*****************************
CCheckPoint *CCheckPoint::m_pCheckPoint = NULL;
//******************************
// コンストラクタ
//******************************
CCheckPoint::CCheckPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// 変数のクリア
	m_type = CCourse::COURSE_EASY;
	m_nNumPoint = 0;
	memset(&m_pCollision, 0, sizeof(m_pCollision)); 
	memset(&m_pointPos, 0, sizeof(m_pointPos));
}

//******************************
// デストラクタ
//******************************
CCheckPoint::~CCheckPoint()
{
}

//******************************
// クリエイト
//******************************
CCheckPoint * CCheckPoint::Create(CCourse::COURSETYPE type)
{
	// メモリの確保
	m_pCheckPoint = new CCheckPoint;

	// 引数の代入
	m_pCheckPoint->m_type = type;// タイプ

	// 初期化
	m_pCheckPoint->Init();

	return m_pCheckPoint;
}

//******************************
// 初期化処理
//******************************
HRESULT CCheckPoint::Init(void)
{
	// オブジェクトタイプの設定
	SetPriority(OBJTYPE_CHECKPOINT);

#ifndef SET_MODE
	// チェックポイントの読み込み
	LoadCheckpoint(m_type);

#endif // !SET_MODE

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCheckPoint::Uninit(void)
{
	Release();
}

//******************************
// 更新処理
//******************************
void CCheckPoint::Update(void)
{
	
#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_pointPos[m_nNumPoint] = CGame::GetPlayer(0)->GetPos();
		if (m_type == CCourse::COURSE_EASY)
		{
			m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS_EASY);
		}
		else
		{
			m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS_NORMAL);
		}
		m_nNumPoint++;
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE))
	{
		m_nNumPoint--;
		m_pCollision[m_nNumPoint]->Uninit();
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveCheckpoint();

		CDebugLog::Init();
		CDebugLog::Print("save");
	}
#else
	// プレイヤー数分ループ
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// プレイヤー情報の取得
		CPlayer*pPlayer = CGame::GetPlayer(nCntPlayer);
		if (pPlayer != NULL)
		{
			// チェックポイント数分ループ
			for (int nCntPoint = 0; nCntPoint < m_nNumPoint; nCntPoint++)
			{
				// チャックポイント番号の比較
				if (pPlayer->GetCheckNum() == nCntPoint)
				{
					// チャックポイントとの当たり判定
					if (CCollision::CollisionSphere(pPlayer->GetCollision(), m_pCollision[nCntPoint]))
					{
						// チェックポイントを増やす
						pPlayer->AddCheckNum();

						// ゴール判定
						if (pPlayer->GetCheckNum() >= m_nNumPoint)
						{
							pPlayer->Goal();
							CDebugLog::Init();
							CDebugLog::Print("Goal");
						}
					}
				}
			}
		}
	}
	

#endif
}

//******************************
// 描画処理
//******************************
void CCheckPoint::Draw(void)
{
}

//******************************
// セーブ
//******************************
void CCheckPoint::SaveCheckpoint(void)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	switch (m_type)
	{
	case CCourse::COURSE_EASY:
		pFile = fopen(CHECKPOINT_TEXT_EASY, "w");
		break;
	case CCourse::COURSE_NORMAL:
		pFile = fopen(CHECKPOINT_TEXT_NORMAL, "w");
		break;
	case CCourse::COURSE_HARD:
		pFile = fopen(CHECKPOINT_TEXT_HARD, "w");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fprintf(pFile, "%d\n", m_nNumPoint);

		// 座標の書き込み
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_pointPos[nCnt].x, m_pointPos[nCnt].y, m_pointPos[nCnt].z);
		}
		// ファイルクローズ
		fclose(pFile);
	}
}

//******************************
// ロード
//******************************
void CCheckPoint::LoadCheckpoint(CCourse::COURSETYPE type)
{
	// ファイルオープン
	FILE *pFile = NULL;

	// コースのタイプにっよって分ける
	switch (type)
	{
	case CCourse::COURSE_EASY:
		pFile = fopen(CHECKPOINT_TEXT_EASY, "r");
		break;
	case CCourse::COURSE_NORMAL:
		pFile = fopen(CHECKPOINT_TEXT_NORMAL, "r");
		break;
	case CCourse::COURSE_HARD:
		pFile = fopen(CHECKPOINT_TEXT_HARD, "r");
		break;
	default:
		pFile = NULL;
		break;
	}
	
	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fscanf(pFile, "%d", &m_nNumPoint);

		// チェックポイント数分ループ
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// 座標の読み込み
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);

			// コリジョンの生成
			if (m_type == CCourse::COURSE_EASY)
			{
				m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS_EASY);
			}
			else
			{
				m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS_NORMAL);
			}
		}

		// ファイルクローズ
		fclose(pFile);
	}
}
