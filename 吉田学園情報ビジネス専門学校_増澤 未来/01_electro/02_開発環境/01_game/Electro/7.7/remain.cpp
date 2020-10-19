//===================================================
//
//    ゲージクラスの処理[gage.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "remain.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "number.h"
#include "game.h"
#include "player.h"

//**********************************
// マクロ定義
//**********************************
#define REMAIN_SIZE_X 30.0f
#define REMAIN_SIZE_Y 20.0f
#define REMAIN_SIZE D3DXVECTOR3(REMAIN_SIZE_X, REMAIN_SIZE_Y, 0.0f)
#define REMAIN_TEXTURE_PATH "./data/Textures/remain000.png" //テクスチャ

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CRemain::m_pTexture = NULL;
//int CRemain::m_nRemain = MAX_GAGE;

//==================================
// コンストラクタ
//==================================
CRemain::CRemain() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	//memset(m_apPolygon, 0, sizeof(m_apPolygon));
	m_pPolygon = NULL;
	m_pNumber = NULL;
	m_nRemain = 1;
}

//==================================
// デストラクタ
//==================================
CRemain::~CRemain()
{
}

//==================================
// クリエイト
//==================================
CRemain * CRemain::Create(void)
{
	// メモリの確保
	CRemain *pRemain;
	pRemain = new CRemain;

	// 初期化
	pRemain->Init();
	// オブジェクトタイプの設定
	pRemain->SetObjType(OBJTYPE_UI);

	return pRemain;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CRemain::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, REMAIN_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CRemain::Unload(void)
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
HRESULT CRemain::Init(void)
{
	// ポリゴン生成
	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(50, 100, 0.0f), REMAIN_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pPolygon->SetTexture(m_pTexture);
	}
	// ナンバー生成
	if (m_pNumber == NULL)
	{
		m_pNumber = CNumber::Create(m_nRemain, D3DXVECTOR3(50 + REMAIN_SIZE_X +10, 105, 0.0f), D3DXVECTOR3(15, 15, 0.0f), D3DXCOLOR(0.8f, 0.01f, 0.05f, 1.0f));
	}

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CRemain::Uninit(void)
{
	// ポリゴンの終了処理
	if (m_pPolygon != NULL)
	{
		// 終了処理
		m_pPolygon->Uninit();
		// メモリの解放
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	// ナンバーの終了処理
	if (m_pNumber != NULL)
	{
		// 終了処理
		m_pNumber->Uninit();
		// メモリの解放
		delete m_pNumber;
		m_pNumber = NULL;
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CRemain::Update(void)
{
	//m_pNumber->SetNumber(CGame::GetPlayer()->GetRemain());
}

//==================================
// 描画処理
//==================================
void CRemain::Draw(void)
{
	// 現在のライフ分描画

	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
	if (m_pNumber != NULL)
	{
		m_pNumber->Draw();
	}

}