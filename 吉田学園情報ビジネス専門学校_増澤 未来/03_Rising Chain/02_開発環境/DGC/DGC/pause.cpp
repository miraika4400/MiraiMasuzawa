//=======================================================================================
//
//ポーズメニュー処理 [pause.cpp]
// Author : Masuzawa Mirai
//
//=======================================================================================

//=======================================================================================
// インクルード
//=======================================================================================
#include "pause.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "joypad.h"

//=======================================================================================
// マクロ定義
//=======================================================================================
#define BACK_SIZE D3DXVECTOR3(200.0f,180.0f,0.0f)                  // 背面サイズ
#define BACK_POS D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f) // 背面座標

#define STRING_SIZE D3DXVECTOR3(966.0f/5.0f,180.0f/5.0f,0.0f)                                  // 文字列
#define RESUME_POS  D3DXVECTOR3(SCREEN_WIDTH/2, 35 + BACK_POS.y - (STRING_SIZE.y + 50),0.0f) // 続ける
#define RESTART_POS D3DXVECTOR3(SCREEN_WIDTH/2, 35 + BACK_POS.y -10,0.0f)                        // リスタート
#define EXIT_POS   	D3DXVECTOR3(SCREEN_WIDTH/2, 35 + BACK_POS.y + (STRING_SIZE.y + 30),0.0f) // 終了

#define MENU_ENTER_COL D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)     // 選んでるメニューの色
#define MENU_NOT_ENTER_COL D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f) // 選んでないメニューの色
//=======================================================================================
// 前方宣言
//=======================================================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[CPause::PARTS_MAX] = {};

//=======================================================================================
// 
//=======================================================================================
CPause::CPause()
{
	memset(&m_pPolygon, 0, sizeof(m_pPolygon));
	m_nMenu = RESUME;
}

//=======================================================================================
// 
//=======================================================================================
CPause::~CPause()
{

}

//=======================================================================================
// 
//=======================================================================================
CPause* CPause::Create(void)
{
	CPause* pPause = new CPause();

	pPause->Init();

	return pPause;
}

//=======================================================================================
// 
//=======================================================================================
HRESULT CPause::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Pose_Bg.png"   , &m_apTexture[BACK]);
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Pose_Play.png" , &m_apTexture[RESUME]);
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Pose_Restart.png", &m_apTexture[RESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Pose_Title.png" , &m_apTexture[EXIT]);

	return S_OK;
}
//=======================================================================================
// テクスチャのアンロード
//=======================================================================================
void CPause::UnLoad(void)
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
// 
//=======================================================================================
HRESULT CPause::Init(void)
{
	// ポリゴン生成

	// 後ろのやつ
	m_pPolygon[BACK] = CPolygon::Create(BACK_POS, BACK_SIZE);
	m_pPolygon[BACK]->SetTexture(m_apTexture[BACK]);
	// 再開のやつ
	m_pPolygon[RESUME] = CPolygon::Create(RESUME_POS, STRING_SIZE);
	m_pPolygon[RESUME]->SetTexture(m_apTexture[RESUME]);
	// リスタートのやつ
	m_pPolygon[RESTART] = CPolygon::Create(RESTART_POS, STRING_SIZE);
	m_pPolygon[RESTART]->SetTexture(m_apTexture[RESTART]);
	// EXITのやつ
	m_pPolygon[EXIT] = CPolygon::Create(EXIT_POS, STRING_SIZE);
	m_pPolygon[EXIT]->SetTexture(m_apTexture[EXIT]);
	

	return S_OK;
}

//=======================================================================================
// 
//=======================================================================================
void CPause::Uninit(void)
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
// 
//=======================================================================================
void CPause::Update(void)
{
	// 選んでるメニューで色分け
	for (int nCntMenu = RESUME; nCntMenu < PARTS_MAX; nCntMenu++)
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

	// メニュー操作
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_W) || CManager::GetKeyboard()->GetKeyTrigger(DIK_UP))
	{// ↑
		m_nMenu--;
	}
	else if (CManager::GetKeyboard()->GetKeyTrigger(DIK_S) || CManager::GetKeyboard()->GetKeyTrigger(DIK_DOWN))
	{// ↓
		m_nMenu++;
	}

	// 範囲外に行かないように
	if (m_nMenu > EXIT)
	{
		m_nMenu = RESUME;
	}
	else if (m_nMenu < RESUME)
	{
		m_nMenu = EXIT;
	}

	// メニューコマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		switch (m_nMenu)
		{
		case RESUME:
			// 続ける
			CManager::SetActivePause(false);
			break;
		case RESTART:
			// リスタート
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			break;
		case EXIT:
			// 終了処理
			// チュートリアル画面に遷移
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			break;
		default:
			break;
		}
	}
}

//=======================================================================================
// 
//=======================================================================================
void CPause::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < PARTS_MAX; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Draw();
		}
	}
}