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
#include "keyboard.h"
#include "game.h"
#include "mouse.h"
#include "player.h"
#include "cpu.h"

//******************************
// マクロ定義
//******************************
#define CAMERA_DISTANCE 850.0f                               // カメラと対象の距離
#define CAMERA_VIEW_MAX 100000.0f                            // カメラの描画距離(遠)
#define CAMERA_VIEW_MIN 10.0f                                // カメラの描画距離(近)
#define CAMERA_FOV D3DXToRadian(45.0f)                       // 視野角
#define CAMERA_BACK_BUFFE D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f) // カメラの背景色
#define CAMERA_MOVE_SPEED_MAX 0.04f                          // カメラ操作感度最大値

// CPUに追従替えるに切り替えるマクロ
#define CPU_CAMERA
#undef  CPU_CAMERA

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
	m_fRad   = 0.0f;              // 目標からの距離
	m_fTheta = 0.0f;              // 角度シータ
	m_fPhi   = 0.0f;              // 角度ファイ
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
	// 変数の初期化
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // 上方向ベクトル

	m_fRad = CAMERA_DISTANCE;     // 距離
	m_fTheta = D3DXToRadian(90); // 角度シータ
	m_fPhi = D3DXToRadian(70);   // 角度ファイ

	// 球面座標の設定
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

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
#ifdef CPU_CAMERA

	// CPUに追従するカメラ（デバッグ用）
	CCpu* pCpu = (CCpu*)CScene::GetTop(CScene::OBJTYPE_CPU);

	// 注視点をCPUにする
	if (pCpu != NULL)
	{
		m_posR = pCpu->GetPos();
	}

	// 角度シータの目標値
	float fThetaDist = (-pCpu->GetRot().y - D3DXToRadian(90));

	// グルんと回転しないよう調整
	while (m_fTheta - fThetaDist > D3DXToRadian(180))
	{
		fThetaDist += D3DXToRadian(360);
	}
	while (m_fTheta - fThetaDist < D3DXToRadian(-180))
	{
		fThetaDist -= D3DXToRadian(360);
	}

	// シータを目標値に近づける
	m_fTheta += (fThetaDist - m_fTheta)*0.05f;
	
	// カメラの縦移動
	m_fPhi -= CManager::GetMouse()->GetMouseMove().y / 100;

#else
	// プレイヤーに追従するカメラ
	CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

	if (pPlayer != NULL)
	{
		m_posR = pPlayer->GetPos();
	}

	m_fTheta -= min(max(CManager::GetMouse()->GetMouseMove().x / 100, -CAMERA_MOVE_SPEED_MAX), CAMERA_MOVE_SPEED_MAX);
	//m_fPhi   -= CManager::GetMouse()->GetMouseMove().y / 100;

#endif // CPU_CAMERA
	
	// 球面座標の設定
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

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

	D3DXMatrixPerspectiveFovLH(&m_pCamera->m_mtxProjection,
		CAMERA_FOV, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_VIEW_MIN, CAMERA_VIEW_MAX);
	
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CAMERA_BACK_BUFFE, 1.0f, 0);
}