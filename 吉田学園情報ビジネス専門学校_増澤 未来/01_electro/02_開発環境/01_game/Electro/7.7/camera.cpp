////////////////////////////////////////////////////
//
//    カメラクラスの処理[camera.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "keyboard.h"
#include "game.h"
#include "boss.h"

//******************************
// 静的メンバ変数宣言
//******************************
CCamera*CCamera::m_pCamera = NULL;

//******************************
// コンストラクタ
//******************************
CCamera::CCamera()
{
	// 変数のクリア
	m_posV = { 0.0f,0.0f,0.0f };
	m_posR = { 0.0f,0.0f,0.0f };
	m_vecU = { 0.0f,0.0f,0.0f };
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_fViewAriaX = 0.0f;
	m_fViewAriaY = 0.0f;
}

//******************************
// デストラクタ
//******************************
CCamera::~CCamera()
{
}

//******************************
// クリエイト
//******************************
CCamera * CCamera::Create()
{

	m_pCamera = new CCamera;
	m_pCamera->Init();

	return m_pCamera;
}

//******************************
// 解放処理
//******************************
void CCamera::Release(void)
{
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//******************************
// 初期化処理
//******************************
HRESULT CCamera::Init(void)
{
	
	CPlayer*pPlayer = CGame::GetPlayer();
	if (pPlayer != NULL)
	{
		if (CPlayer::IsAlive())
		{
			D3DXVECTOR3 playerPos = pPlayer->GetPos();
			m_posV = D3DXVECTOR3(playerPos.x, playerPos.y, -500.0f);
			m_posR = playerPos;
			m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCamera::Uninit(void)
{
}

//******************************
// 更新処理
//******************************
void CCamera::Update(void)
{
	if (CPlayer::IsAlive())
	{
		// プレイヤーの取得
		CPlayer*pPlayer = CGame::GetPlayer();
		if (pPlayer != NULL)
		{
			// プレイヤー座標
			D3DXVECTOR3 playerPos = pPlayer->GetPos();

			// カメラがプレイヤーについていくように
			m_posV = pPlayer->GetPos() * 2;
			m_posV.z = -500;
			m_posR = pPlayer->GetPos() * 2;

		}
	}

	
	float fDistViewAreaX = 0.0f;
	float fDistViewAreaY = 0.0f;
	// ボス戦で視野を広げる
	if (CGame::GetWave()==CGame::WAVE_BOSS)
	{
		fDistViewAreaX = 1.6f * 500.0f;
		fDistViewAreaY = 0.9f * 500.0f;
	}

	m_fViewAriaX += (fDistViewAreaX - m_fViewAriaX)*0.05f;
	m_fViewAriaY += (fDistViewAreaY - m_fViewAriaY)*0.05f;
}

//******************************
// カメラのセット
//******************************
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera->m_mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_pCamera->m_mtxView, &m_pCamera->m_posV, &m_pCamera->m_posR, &m_pCamera->m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera->m_mtxView);

	//プロジェクトマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera->m_mtxProjection);

	//プロジェクションマトリックスの作成*並行投影
	D3DXMatrixOrthoLH(&m_pCamera->m_mtxProjection, ((float)SCREEN_WIDTH * 2) + m_fViewAriaX, ((float)SCREEN_HEIGHT * 2) + m_fViewAriaY, 1, 1000);

	/*D3DXMatrixPerspectiveFovLH(&m_pCamera->mtxProjection,
		D3DXToRadian(100.0f), 
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.0f, 1000.0f);*/
	
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CManager::GetRenderer()->GetBuffColor(), 1.0f, 0);

	
}
