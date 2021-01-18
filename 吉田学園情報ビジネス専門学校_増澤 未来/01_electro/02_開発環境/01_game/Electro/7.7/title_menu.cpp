//=======================================================================================
//
//タイトルメニュー処理 [titlemenu.cpp]
// Author : Masuzawa Mirai
//
//=======================================================================================

//=======================================================================================
// インクルード
//=======================================================================================
#include "title_menu.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "tcp_client.h"
#include "fade.h"
#include "joypad.h"
#include "game.h"
#include "mouse.h"
//=======================================================================================
// マクロ定義
//=======================================================================================
#define BACK_SIZE D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // 背面サイズ
#define BACK_POS  D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // 背面座標

#define STRING_SIZE D3DXVECTOR3(150.0f,50.0f,0.0f)             // 文字列
#define NORMAL_POS  D3DXVECTOR3(SCREEN_WIDTH/2-150, 600,0.0f)  // ノーマル
#define HARD_POS    D3DXVECTOR3(SCREEN_WIDTH/2+150, 600,0.0f)  // ハード

#define MENU_ENTER_COL     D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)     // 選んでるメニューの色
#define MENU_NOT_ENTER_COL D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f)     // 選んでないメニューの色

//=======================================================================================
// 前方宣言
//=======================================================================================
LPDIRECT3DTEXTURE9 CTitleMenu::m_apTexture[CTitleMenu::PARTS_MAX] = {};

//=======================================================================================
// コンストラクタ
//=======================================================================================
CTitleMenu::CTitleMenu():CScene(OBJTYPE_UI)
{
	memset(&m_pPolygon, 0, sizeof(m_pPolygon));
	m_nMenu = BACK;
	m_bMove = true;
}

//=======================================================================================
// デストラクタ
//=======================================================================================
CTitleMenu::~CTitleMenu()
{

}

//=======================================================================================
// クリエイト
//=======================================================================================
CTitleMenu* CTitleMenu::Create(void)
{
	// メモリの生成
	CTitleMenu* pTitleMenu = new CTitleMenu();
	// 初期化処理
	pTitleMenu->Init();

	return pTitleMenu;
}

//=======================================================================================
// テクスチャのロード
//=======================================================================================
HRESULT CTitleMenu::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Normal.png", &m_apTexture[NORMAL]); // ノーマル
	D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Hard.png"  , &m_apTexture[HARD]);   // ハード

	return S_OK;
}
//=======================================================================================
// テクスチャのアンロード
//=======================================================================================
void CTitleMenu::Unload(void)
{

	for (int nCntTex = 0; nCntTex < PARTS_MAX; nCntTex++)
	{
		// テクスチャの開放
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=======================================================================================
// 初期化
//=======================================================================================
HRESULT CTitleMenu::Init(void)
{
	// ポリゴン生成
	m_pPolygon[BACK] = CPolygon::Create(BACK_POS, BACK_SIZE);
	m_pPolygon[BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	// ノーマル
	m_pPolygon[NORMAL] = CPolygon::Create(NORMAL_POS, STRING_SIZE);
	m_pPolygon[NORMAL]->BindTexture(m_apTexture[NORMAL]);
	// ハード
	m_pPolygon[HARD] = CPolygon::Create(HARD_POS, STRING_SIZE);
	m_pPolygon[HARD]->BindTexture(m_apTexture[HARD]);

	return S_OK;
}

//=======================================================================================
// 終了
//=======================================================================================
void CTitleMenu::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < PARTS_MAX; nCntTex++)
	{
		if (m_pPolygon[nCntTex] != NULL)
		{
			//終了処理
			m_pPolygon[nCntTex]->Uninit();
			// メモリの解放
			delete m_pPolygon[nCntTex];
			m_pPolygon[nCntTex] = NULL;
		}
	}
}

//=======================================================================================
// 更新
//=======================================================================================
void CTitleMenu::Update(void)
{

	// メニューコマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		switch (m_nMenu)
		{
		case NORMAL:
			// ノーマル
			CGame::SetDefficult(CGame::DEFFICULT_NORMAL);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			break;

		case HARD:
			// ハード
			CGame::SetDefficult(CGame::DEFFICULT_HARD);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			break;

		default:
			break;
		}
	}

	// 選んでるメニューで色分け
	for (int nCntMenu = NORMAL; nCntMenu < PARTS_MAX; nCntMenu++)
	{
		if (m_nMenu == nCntMenu)
		{// 選んでるとき
			m_pPolygon[nCntMenu]->SetColor(MENU_ENTER_COL);
		}
		else
		{// 選んでないとき
			m_pPolygon[nCntMenu]->SetColor(MENU_NOT_ENTER_COL);
		}
	}
	DIJOYSTATE jy = CManager::GetJoypad()->GetStick(0);

	// メニュー操作
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_A) || CManager::GetKeyboard()->GetKeyTrigger(DIK_LEFT) || m_bMove && jy.lX <= -600)
	{// 左
		m_nMenu--;
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyTrigger(DIK_D) || CManager::GetKeyboard()->GetKeyTrigger(DIK_RIGHT) || m_bMove && jy.lX >= 600)
	{// 右
		m_nMenu++;
		m_bMove = false;
	}

	// スティック用フラグの初期化
	if (jy.lY <= 500 && jy.lY >= -500)
	{
		m_bMove = true;
	}

	// 範囲外に行かないように
	if (m_nMenu < NORMAL)
	{
		m_nMenu = NORMAL;
	}
	else if (m_nMenu > HARD)
	{
		m_nMenu = HARD;
	}
}

//=======================================================================================
// 描画
//=======================================================================================
void CTitleMenu::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < PARTS_MAX; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Draw();
		}
	}
}