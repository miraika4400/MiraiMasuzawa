//===================================================
//
//    ゲージクラスの処理[gage.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "boss.h"

//**********************************
// マクロ定義
//**********************************
#define  GAUGE_TEXTURE_PATH "./data/Textures/HPBar.png" //テクスチャ

#define REDBAR_RATE 0.02f // 赤いバーが徐々に減ってく時の係数

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CGauge::m_pTexture = NULL;

//==================================
// コンストラクタ
//==================================
CGauge::CGauge() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_fRedLife = 0;
	m_fBarWidht = 0.0f;
	m_fBarHeight = 0.0f;
	m_leftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMaxNum = 0;
	m_bRedGauge = false;
	m_pData = NULL;
	m_frontCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
}

//==================================
// デストラクタ
//==================================
CGauge::~CGauge()
{
}

//==================================
// クリエイト
//==================================
CGauge * CGauge::Create(float * pData, D3DXVECTOR3 leftPos, float fBarWidht, float m_fBarHeight, int m_nMaxNum, D3DXCOLOR frontCol, bool bRedGaouge)
{
	// メモリの確保
	CGauge *pGage;
	pGage = new CGauge;

	// 引数の代入
	pGage->m_leftPos = leftPos;         // 左端
	pGage->m_fBarWidht = fBarWidht;     // HPバーの幅
	pGage->m_fBarHeight = m_fBarHeight; // HPバーの高さ
	pGage->m_nMaxNum = m_nMaxNum;       // 最大ライフ数
	pGage->m_bRedGauge = bRedGaouge;    // 赤ゲージを使うかのフラグ     
	pGage->m_pData = pData;             // ゲージと紐づける数値のポインタ
	pGage->m_frontCol = frontCol;       // フロントカラー

	// 初期化
	pGage->Init();
	// オブジェクトタイプの設定
	pGage->SetPriority(OBJTYPE_UI);

	return pGage;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CGauge::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GAUGE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CGauge::Unload(void)
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
HRESULT CGauge::Init(void)
{
	// ライフバーの頂点の設定
	float fWidth = 0.0f;
	fWidth = m_fBarWidht * m_nMaxNum / m_nMaxNum;    // 幅の計算
	if (m_bRedGauge)
	{// 赤ゲージを使う時
		m_fRedLife = (*m_pData); // 赤ゲージのライフ量
	}
	else
	{// 使わないとき
		m_fRedLife = 0.0f;
	}

	// パーツ数分ループ
	for (int nCntGauge = 0; nCntGauge < BAR_PARTS_MAX; nCntGauge++)
	{
		// ポリゴンの生成
		m_apPolygon[nCntGauge] = CPolygon::Create(
			m_leftPos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		// 頂点座標の設定
		D3DXVECTOR3 vtxPos[NUM_VERTEX];

		vtxPos[0] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[1] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y - m_fBarHeight, 0.0f);
		vtxPos[2] = D3DXVECTOR3(m_leftPos.x         , m_leftPos.y + m_fBarHeight, 0.0f);
		vtxPos[3] = D3DXVECTOR3(m_leftPos.x + fWidth, m_leftPos.y + m_fBarHeight, 0.0f);

		m_apPolygon[nCntGauge]->SetVertexPos(vtxPos);
	}

	m_apPolygon[BAR_RED]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_apPolygon[BAR_FRONT]->SetColor(m_frontCol);
	m_apPolygon[BAR_FRAME]->BindTexture(m_pTexture);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CGauge::Uninit(void)
{
	// ライフの最大数分ポリゴンの終了処理
	for (int nCntGauge = 0; nCntGauge < BAR_PARTS_MAX; nCntGauge++)
	{
		if (m_apPolygon[nCntGauge] != NULL)
		{
			// 終了処理
			m_apPolygon[nCntGauge]->Uninit();
			// メモリの解放
			delete m_apPolygon[nCntGauge];
			m_apPolygon[nCntGauge] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CGauge::Update(void)
{
	// ライフバーの頂点の設定

	// 幅の計算
	float fWidth = 0.0f;
	fWidth = m_fBarWidht * (*m_pData) / m_nMaxNum;

	if (m_bRedGauge)
	{// 赤ゲージを使う時
		m_fRedLife += ((*m_pData) - m_fRedLife)*REDBAR_RATE;
	}
	else
	{// 使わないとき
		m_fRedLife = 0.0f;
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
void CGauge::Draw(void)
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
