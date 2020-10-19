//===================================================
//
//    ライフクラスの処理[life.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
// インクルード
//**********************************
#include "bomb_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "boss.h"

//**********************************
// マクロ定義
//**********************************
#define BOMB_UI_POS D3DXVECTOR3(100.0f,632.0f,0.0f)
#define BOMB_UI_SIZE D3DXVECTOR3(25.0f,25.0f,0.0f)
#define BOMB_UI_TEXTURE_PATH "./data/Textures/Bomb.png" //テクスチャ

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CBombUi::m_pTexture = NULL;
//int CBombUi::m_nBombUi = MAX_LIFE;

//==================================
// コンストラクタ
//==================================
CBombUi::CBombUi() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	m_pPolygon = NULL;
}

//==================================
// デストラクタ
//==================================
CBombUi::~CBombUi()
{
}

//==================================
// クリエイト
//==================================
CBombUi * CBombUi::Create(void)
{
	// メモリの確保
	CBombUi *pBombUi;
	pBombUi = new CBombUi;
	// 初期化
	pBombUi->Init();
	// オブジェクトタイプの設定
	pBombUi->SetObjType(OBJTYPE_UI);

	return pBombUi;
}

//==================================
// 初期化処理
//==================================
HRESULT CBombUi::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BOMB_UI_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(BOMB_UI_POS, BOMB_UI_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPolygon->SetTexture(m_pTexture);
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CBombUi::Uninit(void)
{

	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	if (m_pPolygon != NULL)
	{
		// 終了処理
		m_pPolygon->Uninit();
		// メモリの解放
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CBombUi::Update(void)
{

}

//==================================
// 描画処理
//==================================
void CBombUi::Draw(void)
{
	if (CGame::GetPlayer()->HaveBomb())
	{
		m_pPolygon->Draw();
	}
}
