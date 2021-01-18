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
#include "fade.h"
#include "joypad.h"
#include "game.h"
#include "mouse.h"
#include "sound.h"

//=======================================================================================
// マクロ定義
//=======================================================================================
#define BACK_SIZE D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // 背面サイズ
#define BACK_POS  D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // 背面座標

#define STRING_SIZE D3DXVECTOR3(140.0f*1.5f,11.0f*1.5f,0.0f)              // 文字列サイズ
#define TWO_POS     D3DXVECTOR3(SCREEN_WIDTH/2, 530.0f-10,0.0f)  // 二人
#define THREE_POS   D3DXVECTOR3(SCREEN_WIDTH/2, 600.0f-10,0.0f)  // 三人
#define FOUR_POS    D3DXVECTOR3(SCREEN_WIDTH/2, 670.0f-10,0.0f)  // 四人

#define MENU_ENTER_COL     D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)   // 選んでるメニューの色
#define MENU_NOT_ENTER_COL D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)   // 選んでないメニューの色

//=======================================================================================
// 前方宣言
//=======================================================================================
LPDIRECT3DTEXTURE9 CTitleMenu::m_apTexture[CTitleMenu::PARTS_MAX] = {};

//=======================================================================================
// コンストラクタ
//=======================================================================================
CTitleMenu::CTitleMenu() :CScene(OBJTYPE_UI)
{
	memset(&m_pPolygon, 0, sizeof(m_pPolygon));
	m_nMenu = 0;
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
	D3DXCreateTextureFromFile(pDevice, "data/Textures/2_PLAYER_GAME.png", &m_apTexture[TWO]);   // 二人
	D3DXCreateTextureFromFile(pDevice, "data/Textures/3_PLAYER_GAME.png", &m_apTexture[THREE]); // 三人
	D3DXCreateTextureFromFile(pDevice, "data/Textures/4_PLAYER_GAME.png", &m_apTexture[FOUR]);  // 四人

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
	//m_pPolygon[BACK] = CPolygon::Create(BACK_POS, BACK_SIZE);
	//m_pPolygon[BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	// 二人
	m_pPolygon[TWO] = CPolygon::Create(TWO_POS, STRING_SIZE);
	m_pPolygon[TWO]->SetTexture(m_apTexture[TWO]);
	// 三人
	m_pPolygon[THREE] = CPolygon::Create(THREE_POS, STRING_SIZE);
	m_pPolygon[THREE]->SetTexture(m_apTexture[THREE]);
	// 四人
	m_pPolygon[FOUR] = CPolygon::Create(FOUR_POS, STRING_SIZE);
	m_pPolygon[FOUR]->SetTexture(m_apTexture[FOUR]);

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

	// デバッグ用
	if(CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		// 一人
		CGame::SetPlayerNum(1);
		// チュートリアル画面に遷移
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		

	}
#ifdef _DEBUG
#endif // _DEBUG

	// メニューコマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		switch (m_nMenu)
		{
		case TWO:
			// 二人
			CGame::SetPlayerNum(2);
			break;

		case THREE:
			// 三人
			CGame::SetPlayerNum(3);
			break;

		case FOUR:
			// 四人
			CGame::SetPlayerNum(4);
			break;
		default:
			break;
		}
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
		// チュートリアル画面に遷移
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		ShowCursor(FALSE);
	}

	// 選んでるメニューで色分け
	for (int nCntMenu = TWO; nCntMenu < PARTS_MAX; nCntMenu++)
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
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_W) || CManager::GetKeyboard()->GetKeyTrigger(DIK_UP) || m_bMove && jy.lY <= -100)
	{// 左
		m_nMenu--;
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyTrigger(DIK_S) || CManager::GetKeyboard()->GetKeyTrigger(DIK_DOWN) || m_bMove && jy.lY >= 100)
	{// 右
		m_nMenu++;
		m_bMove = false;
	}

	// スティック用フラグの初期化
	if (jy.lY <= 30 && jy.lY >= -30)
	{
		m_bMove = true;
	}

	// 範囲外に行かないように
	if (m_nMenu < TWO)
	{
		m_nMenu = TWO;
	}
	else if (m_nMenu > FOUR)
	{
		m_nMenu = FOUR;
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