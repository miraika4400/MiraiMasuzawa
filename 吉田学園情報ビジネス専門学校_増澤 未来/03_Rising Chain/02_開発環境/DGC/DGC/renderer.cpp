//===================================================
//
//    レンダリングクラスの処理[renderer.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include"debug_log.h"

//==================================
//    コンストラクタ
//==================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
#ifdef _DEBUG
	m_pFont = NULL;
#endif
}

//==================================
//    デストラクタ
//==================================
CRenderer::~CRenderer()
{
}

//==================================
//    初期化処理
//==================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	//変数宣言
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		    // 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						    // ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;    	// インターバル

																	// デバイスの生成
																	// ディスプレイアダプタを表すためのデバイスを作成
																	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// 環境光の設定
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_pD3DDevice->SetMaterial(&material);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // カリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                   // Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          // aブレンド
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // aソースカラーの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // aデスティネーションカラーの設定

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); // テクスチャU値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); // テクスチャV値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  // テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  // テクスチャ縮小時の補間設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// 選択した人数分回す
		for (int nCount = 0; nCount < CGame::GetPlayerNum(); nCount++)
		{
			// 射影行列/ビュー/ワールド
			D3DXMATRIX matProj, matView, matWorld;
			D3DXMATRIX trans;

			if (CGame::GetCamera(nCount) != NULL)
			{
				CCamera *pCamera = CGame::GetCamera(nCount);
				pCamera->SetCamera();

				D3DXVECTOR3 posV = pCamera->GetposV();
				D3DXVECTOR3 posR = pCamera->GetposR();

				D3DXMatrixLookAtLH(&matView,
					&posV,								// カメラ座標
					&posR,								// 注視点座標
					&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// カメラの上の向きのベクトル

				// ビューポートの設定
				SetUpViewport(nCount);

				// バックバッファ＆Ｚバッファのクリア
				m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 255, 255, 0), 1.0f, 0);
			}
			else
			{
				D3DVIEWPORT9 ViewPortClear;

				ViewPortClear.X = 0;
				ViewPortClear.Y = 0;
				ViewPortClear.Width = SCREEN_WIDTH;
				ViewPortClear.Height = SCREEN_HEIGHT;
				ViewPortClear.MinZ = 0;
				ViewPortClear.MaxZ = 1;

				m_pD3DDevice->SetViewport(&ViewPortClear);
			}

			CScene::DrawAll();
		
#ifdef _DEBUG
			// FPS表示
			DrawFPS();
			CDebugLog::Draw();
#endif

			

			// バックバッファとフロントバッファの入れ替え
			//		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		}

		if (CGame::GetPlayerNum() == 3)
		{
			// ビューポートの設定
			SetUpViewport(3);
			CScene::DrawAll();
		}

		D3DVIEWPORT9 ViewPortClear;

		ViewPortClear.X = 0;
		ViewPortClear.Y = 0;
		ViewPortClear.Width = SCREEN_WIDTH;
		ViewPortClear.Height = SCREEN_HEIGHT;
		ViewPortClear.MinZ = 0;
		ViewPortClear.MaxZ = 1;

		m_pD3DDevice->SetViewport(&ViewPortClear);
		if (CManager::GetMode() == CManager::MODE_GAME&&CManager::GetActivePause())
		{// ポーズ状態の時
		 // ポーズメニューの表示
			CManager::GetPause()->Draw();
		}
		//フェードの描画
		CManager::GetFade()->Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// ビューポートの設定
//=============================================================================
bool CRenderer::SetUpViewport(int nNumber)
{
	int nPlayerNum = CGame::GetPlayerNum();

	// プレイヤーの人数
	switch (nPlayerNum)
	{
	case 1:
		// プレイヤーの番号
		switch (nNumber)
		{
		case 0:
			// ビューポートの左上座標
			m_view_port[nNumber].X = 0;
			m_view_port[nNumber].Y = 0;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT ;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;
		}
		break;

	case 2:
		// プレイヤーの番号
		switch (nNumber)
		{
		case 0:
			// ビューポートの左上座標
			m_view_port[nNumber].X = 0;
			m_view_port[nNumber].Y = 0;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH ;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;

		case 1:
			// ビューポートの左上座標
			m_view_port[nNumber].X = 0;
			m_view_port[nNumber].Y = SCREEN_HEIGHT / 2;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}

			break;
		default:
			break;
		}
		break;
	case 3:
	case 4:
		// プレイヤーの番号
		switch (nNumber)
		{
		case 0:
			// ビューポートの左上座標
			m_view_port[nNumber].X = 0;
			m_view_port[nNumber].Y = 0;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH / 2;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;

		case 1:
			// ビューポートの左上座標
			m_view_port[nNumber].X = SCREEN_WIDTH / 2;
			m_view_port[nNumber].Y = 0;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH / 2;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;
		case 2:
			// ビューポートの左上座標
			m_view_port[nNumber].X = 0.0f;
			m_view_port[nNumber].Y = SCREEN_HEIGHT / 2;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH / 2;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;

		case 3:
			// ビューポートの左上座標
			m_view_port[nNumber].X = SCREEN_WIDTH / 2;
			m_view_port[nNumber].Y = SCREEN_HEIGHT / 2;

			// ビューポートの幅
			m_view_port[nNumber].Width = SCREEN_WIDTH / 2;

			// ビューポートの高さ
			m_view_port[nNumber].Height = SCREEN_HEIGHT / 2;

			// ビューポート深度設定
			m_view_port[nNumber].MinZ = 0.0f;
			m_view_port[nNumber].MaxZ = 1.0f;

			// ビューポート設定
			if (FAILED(m_pD3DDevice->SetViewport(&m_view_port[nNumber])))
			{
				return false;
			}
			break;
		default:
			break;
		}							// プレイヤーの人数の終了

		return true;
	}

	return false;

}


#ifdef _DEBUG
void CRenderer::DrawFPS(void)
{

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", GetFps());

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif