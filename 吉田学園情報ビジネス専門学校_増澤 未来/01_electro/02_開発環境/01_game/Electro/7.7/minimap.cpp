//===================================================
//
//    ライフクラスの処理[life.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "minimap.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "enemy.h"
#include "stage.h"
#include "game.h"
#include "boss.h"
#include "item.h"
#include "manacircle.h"

//**********************************
// マクロ定義
//**********************************
#define  MINIMAP_TEXTURE_PATH "./data/Textures/Circle.png" // テクスチャ
#define MINIMAP_POS_X SCREEN_WIDTH  - 150.0f  // ミニマップのX位置
#define MINIMAP_POS_Y SCREEN_HEIGHT - 150.0f // ミニマップのY位置
#define MINIMAP_SCAIL 0.1f

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CMiniMap::m_pTexture = NULL;
D3DXVECTOR3 CMiniMap::m_stagePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//==================================
// コンストラクタ
//==================================
CMiniMap::CMiniMap() :CScene(OBJTYPE_UI)
{
	// 0クリア
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
}

//==================================
// デストラクタ
//==================================
CMiniMap::~CMiniMap()
{
}

//==================================
// クリエイト
//==================================
CMiniMap * CMiniMap::Create(void)
{
	// メモリの確保
	CMiniMap *pMiniMap;
	pMiniMap = new CMiniMap;
	// 初期化
	pMiniMap->Init();
	// オブジェクトタイプの設定
	pMiniMap->SetObjType(OBJTYPE_UI);

	return pMiniMap;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CMiniMap::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MINIMAP_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CMiniMap::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CMiniMap::Init(void)
{

	// ステージ
	D3DXVECTOR3 size; // サイズの取得用
	size = (STAGE_SIZE / 2)*MINIMAP_SCAIL; // サイズの取得
	m_stagePos = STAGE_POS;                // 座標の取得

	// マップ内のステージ生成
	m_apPolygon[0] = CPolygon::Create(
		D3DXVECTOR3(MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f),
		size,
		D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.8f));
	m_apPolygon[0]->SetTexture(m_pTexture);

	
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CMiniMap::Uninit(void)
{
	// ライフの最大数分ポリゴンの終了処理
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			// 終了処理
			m_apPolygon[nCntMiniMap]->Uninit();
			// メモリの解放
			delete m_apPolygon[nCntMiniMap];
			m_apPolygon[nCntMiniMap] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CMiniMap::Update(void)
{
	D3DXVECTOR3 size; // サイズの取得用
	D3DXVECTOR3 pos;  // 座標計算用
	float fAngle;     // ステージ中央からの角度取得用
	float fDistance;  // ステージ中央からの距離

	// マップの初期化 ＊0はステージなので初期化しない
	for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Uninit();
			m_apPolygon[nCntMiniMap] = NULL;
		}
	}

	// マナサークルをマップに表示

	// リストの取得
	
	for (int nCntCirle = 0; nCntCirle < CIRCLE_MAX; nCntCirle++)
	{
		CManaCircle*pCircle = CManaCircle::GetCircle(nCntCirle);
		if (pCircle != NULL)
		{
			// サイズの取得
			size = pCircle->GetSize() / 2;
			// 座標の取得x
			pos = pCircle->GetPos();
			// ステージ中央からの角度の取得
			fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
			// ステージ中央からの距離の取得
			fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

			// 縮尺
			size *= MINIMAP_SCAIL;
			fDistance *= MINIMAP_SCAIL;

			// 座標の計算
			pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
			pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
			for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
			{
				if (m_apPolygon[nCntMiniMap] == NULL)
				{
					// ポリゴンの生成
					m_apPolygon[nCntMiniMap] = CPolygon::Create(
						pos,
						size,
						D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					m_apPolygon[nCntMiniMap]->SetTexture(m_pTexture);

					break;
				}
			}
		}
	}

	// アイテムをマップに表示

	// リストの取得
	std::list<CItem*>::iterator itemIteretor;
	for (itemIteretor = CItem::GetItemlist()->begin(); itemIteretor != CItem::GetItemlist()->end(); itemIteretor++)
	{
		// サイズの取得
		size = (*itemIteretor)->GetSize();
		// 座標の取得
		pos = (*itemIteretor)->GetPos();
		// ステージ中央からの角度の取得
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// ステージ中央からの距離の取得
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// 縮尺
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// 座標の計算
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// ポリゴンの生成
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_apPolygon[nCntMiniMap]->SetTexture(m_pTexture);

				break;
			}
		}
	}

	// エネミーをマップに表示

	// リストの取得
	std::list<CEnemy*>::iterator enemyIteretor;
	for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
	{
		// サイズの取得
		size = (*enemyIteretor)->GetSize();
		// 座標の取得
		pos = (*enemyIteretor)->GetPos();
		// ステージ中央からの角度の取得
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// ステージ中央からの距離の取得
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// 縮尺
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// 座標の計算
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;
		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// ポリゴンの生成
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apPolygon[nCntMiniMap]->SetTexture(m_pTexture);

				break;
			}
		}
	}

	// プレイヤーをマップに表示

	if (CPlayer::IsAlive())
	{

		// サイズの取得
		size = CGame::GetPlayer()->GetSize();
		// 座標の取得
		pos = CGame::GetPlayer()->GetPos();
		// ステージ中央からの角度の取得
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// ステージ中央からの距離の取得
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// 縮尺
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// 座標の計算
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;

		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// ポリゴンの生成
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

				m_apPolygon[nCntMiniMap]->SetTexture(m_pTexture);

				break;
			}
		}
	}

	// ボスをマップに表示
	if (CBoss::IsAlive())
	{

		// サイズの取得
		size = CGame::GetBoss()->GetSize()/2;
		// 座標の取得
		pos = CGame::GetBoss()->GetPos();
		// ステージ中央からの角度の取得
		fAngle = atan2f(m_stagePos.y - pos.y, m_stagePos.x - pos.x);
		// ステージ中央からの距離の取得
		fDistance = sqrtf(powf(m_stagePos.x - pos.x, 2) + powf(m_stagePos.y - pos.y, 2));

		// 縮尺
		size *= MINIMAP_SCAIL;
		fDistance *= MINIMAP_SCAIL;

		// 座標の計算
		pos.x = MINIMAP_POS_X + cosf(fAngle)*-fDistance;
		pos.y = MINIMAP_POS_Y + sinf(fAngle)*fDistance;

		for (int nCntMiniMap = 1; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
		{
			if (m_apPolygon[nCntMiniMap] == NULL)
			{
				// ポリゴンの生成
				m_apPolygon[nCntMiniMap] = CPolygon::Create(
					pos,
					size,
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				m_apPolygon[nCntMiniMap]->SetTexture(m_pTexture);

				break;
			}
		}
	}

	// ポリゴンのアップデートの呼び出し
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Update();
		}
	}

	
}

//==================================
// 描画処理
//==================================
void CMiniMap::Draw(void)
{
	// 現在のライフ分描画
	for (int nCntMiniMap = 0; nCntMiniMap < MAX_MINIMAP_POLYGON; nCntMiniMap++)
	{
		if (m_apPolygon[nCntMiniMap] != NULL)
		{
			m_apPolygon[nCntMiniMap]->Draw();
		}
	}
}
