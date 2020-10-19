////////////////////////////////////////////////////
//
//    TitleLogoの処理[TitleLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "title_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CTitleLogo::m_pTexture = NULL;

//**********************************
// マクロ定義
//**********************************
#define TITLE_LOGO_TEXTURE_PATH "./data/Textures/title_logo2.png" // テクスチャ
#define ANIM_SPEED 5          // アニメーション速度
#define MAX_ANIMATION_X 4      // アニメーション数 横
#define MAX_ANIMATION_Y 6      // アニメーション数 縦
#define LOGO_SIZE D3DXVECTOR3(685.0f/2, 221.0f/2, 0.0f)  // サイズ画像の解像度に合わせる
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH/2,300.0f,0.0f) // 座標
#define NEON_MAX_X 4                                     // ネオン点灯時のU座標
#define NEON_MAX_Y 5                                     // ネオン点灯時のV座標
#define NEON_INTERVAL 60                                 // アニメーション頻度

//=============================
// コンストラクタ
//=============================
CTitleLogo::CTitleLogo():CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0;       // アニメーションカウント
	m_nAnimX = 0;         // アニメーションX軸
	m_nAnimY = 0;         // アニメーションY軸
	m_nCntSetAnim = 0;
	m_bAnim = false;
}

//=============================
// デストラクタ
//=============================
CTitleLogo::~CTitleLogo()
{
}

//=============================
// クリエイト
//=============================
CTitleLogo * CTitleLogo::Create(void)
{
	// メモリの確保
	CTitleLogo *pTitleLogo = new CTitleLogo;
	// 初期化
	pTitleLogo->Init();
	return pTitleLogo;
}

//=============================
// 初期化処理
//=============================
HRESULT CTitleLogo::Init(void)
{
	CScene2d::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURE_PATH, &m_pTexture);
	
	m_bAnim = false;
	m_nAnimX = NEON_MAX_X;
	m_nAnimY = NEON_MAX_Y;

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

	// ランダムで初期化
	m_nCntSetAnim = rand() % 20;
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CTitleLogo::Uninit(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	CScene2d::Uninit();
}


//=============================
// 更新処理
//=============================
void CTitleLogo::Update(void)
{
	
	if (m_bAnim)
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

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV座標セット
			SetTextureUV(uv);
		}

		if (m_nAnimX == NEON_MAX_X && m_nAnimY == NEON_MAX_Y)
		{
			m_nCntSetAnim = rand() % 20;
			m_bAnim = false;
		}
	}
	else
	{
		m_nCntSetAnim++;
		if (m_nCntSetAnim >= NEON_INTERVAL && rand() % 20 == 0)
		{
			m_nCntAnim = ANIM_SPEED - 1;
			CManager::GetSound()->Play(CSound::LABEL_SE_NEON);
			m_bAnim = true;
		}

	}
	
}


//=============================
// 描画処理
//=============================
void CTitleLogo::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2d::Draw();

	// 通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
