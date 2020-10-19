////////////////////////////////////////////////////
//
//    TitleLogoの処理[TitleLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "press_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

//**********************************
// マクロ定義
//**********************************
#define LOGO_TEXTURE_PATH "./data/Textures/press_button.png" // テクスチャ
#define LOGO_SIZE D3DXVECTOR3(685.0f*0.4f, 221.0f*0.4f, 0.0f)  // サイズ画像の解像度に合わせる
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH/2,600.0f,0.0f) // 座標

//=============================
// コンストラクタ
//=============================
CPressLogo::CPressLogo()
{
	m_pTexture = NULL;
}

//=============================
// デストラクタ
//=============================
CPressLogo::~CPressLogo()
{
}

//=============================
// クリエイト
//=============================
CPressLogo * CPressLogo::Create(void)
{
	// メモリの確保
	CPressLogo *pPressLogo = new CPressLogo;
	// 初期化
	pPressLogo->Init();
	return pPressLogo;
}

//=============================
// 初期化処理
//=============================
HRESULT CPressLogo::Init(void)
{
	CTitleLogo::Init();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, LOGO_TEXTURE_PATH, &m_pTexture);

	// 座標の設定
	SetPos(LOGO_POS);
	// サイズの設定
	SetSize(LOGO_SIZE);
	// テクスチャの設定
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CPressLogo::Uninit(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	CTitleLogo::Uninit();
}


//=============================
// 更新処理
//=============================
void CPressLogo::Update(void)
{
	CTitleLogo::Update();
}


//=============================
// 描画処理
//=============================
void CPressLogo::Draw(void)
{
	CTitleLogo::Draw();
}
