////////////////////////////////////////////////////
//
//    カメラクラスの処理[camera.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "title_camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "player.h"
#include "mouse.h"
#include "joypad.h"
#include "destination.h"
#include "scene.h"

//******************************
// マクロ定義
//******************************
#define SPEED 50.0f // 移動速度
#define DIRECTION_RATE 0.05f

//******************************
// 静的メンバ変数宣言
//******************************

//******************************
// コンストラクタ
//******************************
CTitleCamera::CTitleCamera()
{
	// 変数のクリア
	m_posV = VEC3_ZERO;
	m_posR = VEC3_ZERO;
	m_vecU = VEC3_ZERO;
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_nNumPoint = 0;
	memset(&m_pointPos, 0, sizeof(m_pointPos));
	m_nNum = 0;               // 座標番号
	m_nNumNext = 1;           // 次の座標番号
	m_fAngle = 0.0f;
	m_fAngleDist = 0.0f;
}

//******************************
// デストラクタ
//******************************
CTitleCamera::~CTitleCamera()
{
}

//******************************
// クリエイト
//******************************
CTitleCamera * CTitleCamera::Create(void)
{

	CTitleCamera *pTitleCamera = new CTitleCamera;

	pTitleCamera->Init();

	return pTitleCamera;
}

//******************************
// 初期化処理
//******************************
HRESULT CTitleCamera::Init(void)
{
	// 変数の初期化
	m_posV = D3DXVECTOR3(80000.0f, 30000.0f, -500.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_nNum = 0;               // 座標番号
	m_nNumNext = 1;           // 次の座標番号
	// テキストの読みこみ
	LoadText();
	m_posV = m_pointPos[m_nNum];
	m_posR = m_pointPos[m_nNumNext];

	m_fAngle = atan2f(m_pointPos[m_nNumNext].z - m_pointPos[m_nNum].z, m_pointPos[m_nNumNext].x - m_pointPos[m_nNum].x);
	m_fAngleDist = m_fAngle;
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CTitleCamera::Uninit(void)
{
}

//******************************
// 更新処理
//******************************
void CTitleCamera::Update(void)
{
	D3DXVECTOR3 move = m_pointPos[m_nNumNext] - m_pointPos[m_nNum];
	D3DXVec3Normalize(&move, &move);
	move *= SPEED;
	// 移動量を足す
	m_posV += move;

	// 注視点の管理
	m_fAngleDist = atan2f(move.z, move.x);
	Direction();
	m_posR.x = m_posV.x + cosf(m_fAngle)*SPEED;
	m_posR.y = m_posV.y;
	m_posR.z = m_posV.z + sinf(m_fAngle)*SPEED;

	float fDistance = sqrtf(powf(m_pointPos[m_nNumNext].x - m_posV.x, 2) + powf(m_pointPos[m_nNumNext].y - m_posV.y, 2) + powf(m_pointPos[m_nNumNext].z - m_posV.z, 2));
	if (fDistance <= 500)
	{
		m_nNum++;
		m_nNumNext = m_nNum + 1;

		m_nNum %= m_nNumPoint;
		m_nNumNext %= m_nNumPoint;

		//m_posR = m_pointPos[m_nNumNext];
	}
}

//******************************
// カメラのセット
//******************************
void CTitleCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクトマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	float fWidth = CManager::GetRenderer()->GetViewPort(0).Width;
	float fHeight = CManager::GetRenderer()->GetViewPort(0).Height;

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(60.0f),
		fWidth / fHeight, 10.0f, 1000000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

void CTitleCamera::Direction(void)
{
	// グルんと回転しないよう調整
	while (m_fAngleDist - m_fAngle > D3DXToRadian(180))
	{
		m_fAngleDist -= D3DXToRadian(360);
	}
	while (m_fAngleDist - m_fAngle < D3DXToRadian(-180))
	{
		m_fAngleDist += D3DXToRadian(360);
	}

	m_fAngle += (m_fAngleDist - m_fAngle)*DIRECTION_RATE;
}

//******************************
// テキストの読み込み
//******************************
void CTitleCamera::LoadText(void)
{
	// ファイルオープン
	FILE *pFile = NULL;
	pFile = fopen("data/Texts/Title_data.txt", "r");
	
	if (pFile != NULL)
	{
		// チェックポイント数の書き込み
		fscanf(pFile, "%d", &m_nNumPoint);

		// チェックポイント数分ループ
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// 座標の読み込み
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);

		}

		// ファイルクローズ
		fclose(pFile);
	}
}
