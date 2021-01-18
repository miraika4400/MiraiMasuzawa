////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "fever_logo.h"
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
#define TEXTURE_PATH "./data/Textures/fever.png" // テクスチャのパス
#define ANIM_SPEED 3           // アニメーション速度
#define MAX_ANIMATION_X 12     // アニメーション数 横
#define MAX_ANIMATION_Y 1      // アニメーション数 縦
#define COLOR_COUNT 20         // 目標カラー変更のインターバル
#define COLOR_RATE  0.05f      // 色を変えるとき係数
#define LOGO_SIZE D3DXVECTOR3(600.0f/2.0f, 200.0f / 2.0f, 0.0f)   // サイズ画像の解像度に合わせる
#define LOGO_POS  D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f,0.0f)  // 座標
//D3DXCOLOR(((float)(rand() % 50) / 100.0f)+ 0.5f, ((float)(rand() % 50) / 100.0f)+0.5f, ((float)(rand() % 50) / 100.0f) + 0.5f, 1.0f);
//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CFeverLogo::m_pTexture = NULL;

//=============================
// コンストラクタ
//=============================
CFeverLogo::CFeverLogo() :CScene2d(OBJTYPE_UI)
{
	// 変数のクリア
	m_nCntAnim  = 0;                                 // アニメーションカウント
	m_nAnimX    = 0;                                 // アニメーションX軸
	m_nAnimY    = 0;                                 // アニメーションY軸
	m_nCntColor = 0;                                 // 色を変えるカウント
	m_distColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // カラー変更時の目標カラー
}

//=============================
// デストラクタ
//=============================
CFeverLogo::~CFeverLogo()
{
}

//=============================
// クリエイト
//=============================
CFeverLogo * CFeverLogo::Create(void)
{
	// メモリの確保
	CFeverLogo *pFeverLogo = new CFeverLogo;
	// 初期化
	pFeverLogo->Init();
	return pFeverLogo;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CFeverLogo::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CFeverLogo::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// 初期化処理
//=============================
HRESULT CFeverLogo::Init(void)
{
	CScene2d::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	// 座標の設定
	SetPos(LOGO_POS);
	// サイズの設定
	SetSize(LOGO_SIZE);
	// テクスチャの設定
	BindTexture(m_pTexture);
	//SetAddMode(true);
	// ランダムで初期化
	m_distColor = D3DXCOLOR((float)(rand() % 80) / 100.0f, (float)(rand() % 80) / 100.0f, (float)(rand() % 80) / 100.0f, 1.0f);
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CFeverLogo::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// 更新処理
//=============================
void CFeverLogo::Update(void)
{
	// アニメーションカウントを進める
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_SPEED == 0)
	{
		// アニメーションX軸の加算
		m_nAnimX++;

		if (m_nAnimX > MAX_ANIMATION_X)
		{
			// アニメーションX軸の初期化
			m_nAnimX = 0;
			m_nAnimY++;
			if (m_nAnimY > MAX_ANIMATION_Y)
			{
				m_nAnimY = 0;
			}
		}

		// UV座標の設定
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV座標セット
		SetTextureUV(uv);
	}

	// カウントを進める
	m_nCntColor++;
	// 一定のカウントで
	if (m_nCntColor % COLOR_COUNT == 0)
	{
		m_distColor = D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f); // カラー変更時の目標カラー
	}
	else
	{
		// 現在カラーの取得
		D3DXCOLOR col = GetColor();
		
		// 徐々に色を変える
		col.r += (m_distColor.r - col.r)*COLOR_RATE;
		col.g += (m_distColor.g - col.g)*COLOR_RATE;
		col.b += (m_distColor.b - col.b)*COLOR_RATE;

		// 色のセット
		SetColor(col);
	}
}


//=============================
// 描画処理
//=============================
void CFeverLogo::Draw(void)
{
	CScene2d::Draw();
}
