////////////////////////////////////////////////////
//
//    タイトルクラスの処理[title.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "title_menu.h"
#include "title_camera.h"
#include "course.h"

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;
CTitleCamera *CTitle::m_pCamera = NULL;

//**********************************
// マクロ定義
//**********************************
#define  TITLE_TEXTURE_PATH "./data/Textures/Title.png" // テクスチャ

//=============================
// コンストラクタ
//=============================
CTitle::CTitle():CScene(OBJTYPE_UI)
{
	m_pPolygon = NULL;
}

//=============================
// デストラクタ
//=============================
CTitle::~CTitle()
{
}

//=============================
// クリエイト
//=============================
CTitle * CTitle::Create(void)
{
	// メモリの確保
	CTitle *pTitle = new CTitle;
	// 初期化
	pTitle->Init();
	return pTitle;
}

//=============================
// 初期化処理
//=============================
HRESULT CTitle::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2)-100, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->SetTexture(m_pTexture);
	
	// 人数選択メニューの生成
	CTitleMenu::Create();

	// カメラの生成
	m_pCamera = CTitleCamera::Create();
	// コースの生成
	CCourse::Create(CCourse::COURSE_EASY);

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CTitle::Uninit(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pPolygon != NULL)
	{
		// ポリゴンの終了処理
		m_pPolygon->Uninit();

		// メモリの解放
		delete m_pPolygon;
		m_pPolygon = NULL;
	}
	
	if (m_pCamera != NULL)
	{
		// カメラの終了処理
		m_pCamera->Uninit();

		// メモリの解放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// 開放処理
	Release();
}


//=============================
// 更新処理
//=============================
void CTitle::Update(void)
{
	// ポリゴンの更新処理
	m_pPolygon->Update();

	if (m_pCamera != NULL)
	{
		// カメラの終了処理
		m_pCamera->Update();
	}
}


//=============================
// 描画処理
//=============================
void CTitle::Draw(void)
{
	// カメラのセット
	m_pCamera->SetCamera();
	// ポリゴンの描画処理
	m_pPolygon->Draw();
}
