//===================================================
//
//    ライフクラスの処理[life.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "life.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "boss.h"

//**********************************
// マクロ定義
//**********************************
#define  LIFE_TEXTURE_PATH "./data/Textures/HPBar.png" //テクスチャ

// プレイヤー
#define PLAYER_LIFE_WIDTH 250.0f // HPバーの横幅の最大
#define PLAYER_LIFE_HEGHT 15     // HPバーの縦幅
#define PLAYER_LIFE_POS_X 150.0f // X位置
#define PLAYER_LIFE_POS_Y 650.0f // Y位置

// ボス
#define BOSS_LIFE_WIDTH 500.0f                               // HPバーの横幅の最大
#define BOSS_LIFE_HEGHT 10                                   // HPバーの縦幅
#define BOSS_LIFE_POS_X SCREEN_WIDTH/2 - (BOSS_LIFE_WIDTH/2) // X位置
#define BOSS_LIFE_POS_Y 100.0f                               // Y位置

#define REDBAR_RATE 0.02f // 赤いバーが徐々に減ってく時の係数

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;
//int CLife::m_nLife = MAX_LIFE;

//==================================
// コンストラクタ
//==================================
CLife::CLife()
{
	// 変数のクリア
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_fRedLife = 0;
	m_fBarWidht = 0.0f;
	m_fBarHeight = 0.0f;
	m_leftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMaxLife = 0;
}

//==================================
// デストラクタ
//==================================
CLife::~CLife()
{
}

//==================================
// クリエイト
//==================================
CLife * CLife::Create(LIFE_USER user)
{
	// メモリの確保
	CLife *pLife;
	pLife = new CLife;

	// ユーザーの設定
	pLife->m_user = user;
	// 初期化
	pLife->Init();
	// オブジェクトタイプの設定
	pLife->SetObjType(OBJTYPE_UI);
	

	return pLife;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CLife::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, LIFE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CLife::Unload(void)
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
HRESULT CLife::Init(void)
{
	// ライフバーの頂点の設定
	float fWidth = 0.0f; 
	switch (m_user)
	{
	case USER_PLAYER:
		m_leftPos = D3DXVECTOR3(PLAYER_LIFE_POS_X, PLAYER_LIFE_POS_Y, 0.0f); // 左端
		m_fBarWidht = PLAYER_LIFE_WIDTH;                                     // HPバーの幅
		m_fBarHeight = PLAYER_LIFE_HEGHT;                                    // HPバーの高さ
		m_nMaxLife = PLAYER_MAX_LIFE;                                        // 最大ライフ数
		fWidth = m_fBarWidht * CGame::GetPlayer()->GetLife() / m_nMaxLife;   // 幅の計算
		m_fRedLife = CGame::GetPlayer()->GetLife();                          // 赤ゲージのライフ量
		break;
	case USER_BOSS:
		m_leftPos = D3DXVECTOR3(BOSS_LIFE_POS_X, BOSS_LIFE_POS_Y, 0.0f);    // 左端
		m_fBarWidht = BOSS_LIFE_WIDTH;									    // HPバーの幅
		m_fBarHeight = BOSS_LIFE_HEGHT;									    // HPバーの高さ
		m_nMaxLife = BOSS_MAX_LIFE;										    // 最大ライフ数
		fWidth = m_fBarWidht * CGame::GetBoss()->GetLife() / m_nMaxLife;    // 幅の計算
		m_fRedLife = CGame::GetBoss()->GetLife();						    // 赤ゲージのライフ量
		break;
	default:
		break;
	}

	// プレイヤーのゲージ
	for (int nCntLife = 0; nCntLife < BAR_PARTS_MAX; nCntLife++)
	{
		// ポリゴンの生成
		m_apPolygon[nCntLife] = CPolygon::Create(
			m_leftPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		
		
		D3DXVECTOR3 vtxPos[NUM_VERTEX];

		vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
		vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

		m_apPolygon[nCntLife]->SetVertexPos(vtxPos);
		//m_apPolygon[nCntLife]->SetTexture(m_pTexture);
	}

	
	m_apPolygon[BAR_RED]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_apPolygon[BAR_GREEN]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apPolygon[BAR_FRAME]->SetTexture(m_pTexture);


	return S_OK;
}

//==================================
// 終了処理
//==================================
void CLife::Uninit(void)
{
	// ライフの最大数分ポリゴンの終了処理
	for (int nCntLife = 0; nCntLife < BAR_PARTS_MAX; nCntLife++)
	{
		if (m_apPolygon[nCntLife] != NULL)
		{
			// 終了処理
			m_apPolygon[nCntLife]->Uninit();
			// メモリの解放
			delete m_apPolygon[nCntLife];
			m_apPolygon[nCntLife] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CLife::Update(void)
{
	int nPlayerLife = CGame::GetPlayer()->GetLife();

	// ライフバーの頂点の設定

	// 幅の計算
	float fWidth = 0.0f;
	switch (m_user)
	{
	case USER_PLAYER:
		if (CPlayer::IsAlive())
		{
			// 緑ゲージ
			fWidth = m_fBarWidht * CGame::GetPlayer()->GetLife() / m_nMaxLife;
			// 赤ゲージ
			m_fRedLife += (CGame::GetPlayer()->GetLife() - m_fRedLife)*REDBAR_RATE;
		}
		else
		{
			m_fRedLife = 0.0f;
		}
		break;

	case USER_BOSS:
		if (CBoss::IsAlive())
		{
			// 緑ゲージ
			fWidth = m_fBarWidht * CGame::GetBoss()->GetLife() / m_nMaxLife;
			// 赤ゲージ
			m_fRedLife += (CGame::GetBoss()->GetLife() - m_fRedLife)*REDBAR_RATE;
		}
		else
		{
			m_fRedLife = 0.0f;
		}
		break;

	default:
		break;
	}
	

	// 頂点座標の設定
	D3DXVECTOR3 vtxPos[NUM_VERTEX];

	vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);
	
	m_apPolygon[BAR_GREEN]->SetVertexPos(vtxPos);

	

	// 幅の計算
	fWidth = m_fBarWidht * m_fRedLife / m_nMaxLife;

	// 頂点座標の設定
	vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

	m_apPolygon[BAR_RED]->SetVertexPos(vtxPos);

	// ポリゴンの最大数分更新処理
	for (int nCntLife = 0; nCntLife < BAR_PARTS_MAX; nCntLife++)
	{
		if (m_apPolygon[nCntLife] != NULL)
		{
			m_apPolygon[nCntLife]->Update();
		}
	}


}

//==================================
// 描画処理
//==================================
void CLife::Draw(void)
{
	// 現在のライフ分描画
	for (int nCntLife = 0; nCntLife < BAR_PARTS_MAX; nCntLife++)
	{
		if (m_apPolygon[nCntLife] != NULL)
		{
			m_apPolygon[nCntLife]->Draw();
		}
	}
}
