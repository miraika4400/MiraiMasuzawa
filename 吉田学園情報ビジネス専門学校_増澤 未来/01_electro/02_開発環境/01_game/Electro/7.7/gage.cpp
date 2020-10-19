//===================================================
//
//    ゲージクラスの処理[gage.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "gage.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "boss.h"

//**********************************
// マクロ定義
//**********************************
#define  GAGE_TEXTURE_PATH "./data/Textures/HPBar.png" //テクスチャ

// プレイヤー(HP)
#define PLAYER_LIFE_WIDTH 300.0f // HPバーの横幅の最大
#define PLAYER_LIFE_HEGHT 15      // HPバーの縦幅
#define PLAYER_LIFE_POS_X 150.0f // X位置
#define PLAYER_LIFE_POS_Y 618.0f // Y位置
// プレイヤー(MP)
#define PLAYER_MP_WIDTH 300.0f // HPバーの横幅の最大
#define PLAYER_MP_HEGHT 12      // HPバーの縦幅
#define PLAYER_MP_POS_X 150.0f // X位置
#define PLAYER_MP_POS_Y 645.0f // Y位置

// ボス
#define BOSS_HP_WIDTH 500.0f                               // HPバーの横幅の最大
#define BOSS_HP_HEGHT 10                                   // HPバーの縦幅
#define BOSS_HP_POS_X SCREEN_WIDTH/2 - (BOSS_HP_WIDTH/2) // X位置
#define BOSS_HP_POS_Y 100.0f                               // Y位置

#define REDBAR_RATE 0.02f // 赤いバーが徐々に減ってく時の係数

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CGage::m_pTexture = NULL; // テクスチャ

//==================================
// コンストラクタ
//==================================
CGage::CGage() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_fRedLife = 0;
	m_fBarWidht = 0.0f;
	m_fBarHeight = 0.0f;
	m_leftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMaxNum = 0;
}

//==================================
// デストラクタ
//==================================
CGage::~CGage()
{
}

//==================================
// クリエイト
//==================================
CGage * CGage::Create(GAGE_TYPE type)
{
	// メモリの確保
	CGage *pGage;
	pGage = new CGage;

	// ユーザーの設定
	pGage->m_gageType = type;
	// 初期化
	pGage->Init();
	// オブジェクトタイプの設定
	pGage->SetObjType(OBJTYPE_UI);
	

	return pGage;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CGage::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GAGE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CGage::Unload(void)
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
HRESULT CGage::Init(void)
{
	// ライフバーの頂点の設定
	float fWidth = 0.0f;
	D3DXCOLOR frontCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	switch (m_gageType)
	{
	case GAGE_PLAYER_HP:
		m_leftPos = D3DXVECTOR3(PLAYER_LIFE_POS_X, PLAYER_LIFE_POS_Y, 0.0f); // 左端
		m_fBarWidht = PLAYER_LIFE_WIDTH;                                     // HPバーの幅
		m_fBarHeight = PLAYER_LIFE_HEGHT;                                    // HPバーの高さ
		m_nMaxNum = PLAYER_MAX_HP;                                        // 最大ライフ数
		fWidth = m_fBarWidht * CGame::GetPlayer()->GetLife() / m_nMaxNum;   // 幅の計算
		m_fRedLife = CGame::GetPlayer()->GetLife();                          // 赤ゲージのライフ量
		break;
	case GAGE_PLAYER_MP:
		m_leftPos = D3DXVECTOR3(PLAYER_MP_POS_X, PLAYER_MP_POS_Y, 0.0f); // 左端
		m_fBarWidht = PLAYER_MP_WIDTH;                                     // HPバーの幅
		m_fBarHeight = PLAYER_MP_HEGHT;                                    // HPバーの高さ
		m_nMaxNum = PLAYER_MAX_MP;                                           // 最大
		fWidth = m_fBarWidht * CGame::GetPlayer()->GetMp() / m_nMaxNum;      // 幅の計算
		m_fRedLife = 0;                                                      // 赤ゲージの量

		frontCol = D3DXCOLOR(0.0f, 0.75f, 1.0f, 1.0f);
		break;
	case GAGE_BOSS_HP:
		m_leftPos = D3DXVECTOR3(BOSS_HP_POS_X, BOSS_HP_POS_Y, 0.0f);    // 左端
		m_fBarWidht = BOSS_HP_WIDTH;									    // HPバーの幅
		m_fBarHeight = BOSS_HP_HEGHT;									    // HPバーの高さ
		m_nMaxNum = BOSS_MAX_LIFE;										    // 最大
		fWidth = m_fBarWidht * CGame::GetBoss()->GetLife() / m_nMaxNum;     // 幅の計算
		m_fRedLife = CGame::GetBoss()->GetLife();						    // 赤ゲージの量
		break;
	default:
		break;
	}

	// プレイヤーのゲージ
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		// ポリゴンの生成
		m_apPolygon[nCntGage] = CPolygon::Create(
			m_leftPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		
		// 頂点座標の設定
		D3DXVECTOR3 vtxPos[NUM_VERTEX];

		vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
		vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

		m_apPolygon[nCntGage]->SetVertexPos(vtxPos);
	}

	// 赤ゲージの色
	m_apPolygon[BAR_RED]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	// 緑ゲージの色
	m_apPolygon[BAR_FRONT]->SetColor(frontCol);
	// ゲージのフレームのテクスチャ割り当て
	m_apPolygon[BAR_FRAME]->SetTexture(m_pTexture);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CGage::Uninit(void)
{
	// ライフの最大数分ポリゴンの終了処理
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			// 終了処理
			m_apPolygon[nCntGage]->Uninit();
			// メモリの解放
			delete m_apPolygon[nCntGage];
			m_apPolygon[nCntGage] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CGage::Update(void)
{
	// ライフバーの頂点の設定

	// 幅の計算

	float fWidth = 0.0f;
	switch (m_gageType)
	{
	case GAGE_PLAYER_HP:
		if (CPlayer::IsAlive())
		{
			// 緑ゲージ
			fWidth = m_fBarWidht * CGame::GetPlayer()->GetLife() / m_nMaxNum;
			// 赤ゲージ
			m_fRedLife += (CGame::GetPlayer()->GetLife() - m_fRedLife)*REDBAR_RATE;
		}
		else
		{
			m_fRedLife = 0.0f;
		}
		break;
	case GAGE_PLAYER_MP:
		if (CPlayer::IsAlive())
		{
			// 緑ゲージ
			fWidth = m_fBarWidht * CGame::GetPlayer()->GetMp() / m_nMaxNum;
			// 赤ゲージ
			m_fRedLife = 0.0f;
		}

		break;

	case GAGE_BOSS_HP:
		if (CBoss::IsAlive())
		{
			// 緑ゲージ
			fWidth = m_fBarWidht * CGame::GetBoss()->GetLife() / m_nMaxNum;
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
	
	m_apPolygon[BAR_FRONT]->SetVertexPos(vtxPos);

	// 幅の計算
	fWidth = m_fBarWidht * m_fRedLife / m_nMaxNum;

	// 頂点座標の設定
	vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
	vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
	vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

	m_apPolygon[BAR_RED]->SetVertexPos(vtxPos);

	// ポリゴンの最大数分更新処理
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			m_apPolygon[nCntGage]->Update();
		}
	}
}

//==================================
// 描画処理
//==================================
void CGage::Draw(void)
{
	// 前パーツ分描画
	for (int nCntGage = 0; nCntGage < BAR_PARTS_MAX; nCntGage++)
	{
		if (m_apPolygon[nCntGage] != NULL)
		{
			m_apPolygon[nCntGage]->Draw();
		}
	}
}
