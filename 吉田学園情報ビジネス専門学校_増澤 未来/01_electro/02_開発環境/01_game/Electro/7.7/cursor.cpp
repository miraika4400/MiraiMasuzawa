//===================================================
//
//    カーソルクラスの処理[cursor.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "cursor.h"
#include "manager.h"
#include "renderer.h"
#include "joypad.h"
#include "mouse.h"
#include "player.h"
#include "game.h"
#include "camera.h"

//*****************************
// マクロ定義
//*****************************
#define CURSOR_TEXTURE_PATH "./data/Textures/cursor001.png" // テクスチャのパス
#define CURSOR_RATE_SPEED 60.0f   // ジョイパッドの感度調整用
#define CURSOR_SIZE 30            // カーソルサイズ
<<<<<<< HEAD
=======
//#define CURSOR_DISTANCE 100       // カーソルサイズ
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
#define CURSOR_ANIM_SPEED 6       // アニメーション速度
#define CURSOR_MAX_ANIMATION_X 11 // アニメーション数 横
#define CURSOR_MAX_ANIMATION_Y 1  // アニメーション数 縦

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CCursor::m_pTexture = NULL;

//==================================
// コンストラクタ
//==================================
CCursor::CCursor():CScene2d(OBJTYPE_UI)
{
<<<<<<< HEAD
	// 変数のクリア
=======
	m_fRotAngle = 0.0f;
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//==================================
// デストラクタ
//==================================
CCursor::~CCursor()
{
}

//==================================
// クリエイト
//==================================
CCursor * CCursor::Create(const D3DXVECTOR3 pos)
{
	//メ モリの確保
	CCursor *pCursor;
	pCursor = new CCursor;
	// 初期化
	pCursor->Init();

<<<<<<< HEAD
=======
	pCursor->m_fRotAngle = 0.0f;

>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

	// 座標の設定
	CManager::GetMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f));
	pCursor->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f));

	// サイズの設定
	pCursor->SetSize(D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f));
	// オブジェクトタイプの設定
	pCursor->SetObjType(OBJTYPE_UI);

	return pCursor;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CCursor::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, CURSOR_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CCursor::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CCursor::Init(void)
{
	ShowCursor(false);

	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}
	// テクスチャ割り当て
	BindTexture(m_pTexture);

<<<<<<< HEAD
=======
	// 回転角度の初期化
	m_fRotAngle = 0.0f;

>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / CURSOR_MAX_ANIMATION_X;
	float fv = 1.0f / CURSOR_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CCursor::Uninit(void)
{
	CScene2d::Uninit();
	ShowCursor(true);
}

//==================================
// 更新処理
//==================================
void CCursor::Update(void)
{
	// マウスの取得
	CInputMouse*pMouse = CManager::GetMouse();
	// マウスカーソル座標の取得
	D3DXVECTOR3 pos = pMouse->GetMousePos();
	// 移動する前の座標の保存
	D3DXVECTOR3 posOld = pos;
	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

<<<<<<< HEAD
=======
	//// ジョイパッドでカーソルの操作
	//pos.x += cosf(45) * js.lZ / CURSOR_RATE_SPEED;
	//pos.y += sinf(45) * js.lRz / CURSOR_RATE_SPEED;

	//// 画面端の判定
	//if (pos.y < 0.0f + CURSOR_SIZE / 2)
	//{// ↑
	//	pos.y = 0.0f + CURSOR_SIZE / 2;
	//}
	//if (pos.y > SCREEN_HEIGHT - CURSOR_SIZE / 2)
	//{ // ↓
	//	pos.y = SCREEN_HEIGHT - CURSOR_SIZE / 2;
	//}
	//if (pos.x < 0.0f + CURSOR_SIZE / 2)
	//{// ←
	//	pos.x = 0.0f + CURSOR_SIZE / 2;
	//}
	//if(pos.x > SCREEN_WIDTH - CURSOR_SIZE / 2)
	//{ // →
	//	pos.x = SCREEN_WIDTH - CURSOR_SIZE / 2;
	//}

>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	// ジョイスティックの角度でカーソルの位置を変える
	if (js.lRz >= 600 || js.lRz <= -600 || js.lZ >= 600 || js.lZ <= -600)
	{
		// スティックのベクトルの計算
		D3DXVECTOR3 stickVector = D3DXVECTOR3(cosf(45)*js.lZ, sinf(45)*js.lRz, 0.0f);
		// 正規化
		D3DXVec3Normalize(&stickVector, &stickVector);
		// 座標の設定
		pos.x = SCREEN_WIDTH / 2 + stickVector.x * 200;
		pos.y = SCREEN_HEIGHT / 2 + stickVector.y * 200;
	}

	// 座標のセット
	SetPos(pos);
	pMouse->SetMousePos(pos);

<<<<<<< HEAD
=======
	/*m_fRotAngle += 2.0f;
	SetAngle(m_fRotAngle);*/

>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	// アニメーションカウントを進める
	m_nCntAnim++;

	if (m_nCntAnim % CURSOR_ANIM_SPEED == 0)
	{
		// アニメーションX軸の加算
		m_nAnimX++;

		if (m_nAnimX >= CURSOR_MAX_ANIMATION_X)
		{
			// アニメーションX軸の初期化
			m_nAnimX = 0;
		}

		// UV座標の設定
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / CURSOR_MAX_ANIMATION_X;
		float fv = 1.0f / CURSOR_MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV座標セット
		SetTextureUV(uv);
	}

}

//==================================
// 描画処理
//==================================
void CCursor::Draw(void)
{
	CScene2d::Draw();
}

<<<<<<< HEAD
//==================================
// スクリーン座標からワールド座標に変換
//==================================
=======
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
D3DXVECTOR3 CCursor::GetPos3d(void)
{
	D3DXVECTOR3 pos;

	CCamera*pCamera = CGame::GetCamera();

	// 各行列の逆行列
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &pCamera->GetViewMtx());
	D3DXMatrixInverse(&InvPrj, NULL, &pCamera->GetProjectionMtx());
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(GetPos().x, GetPos().y, 0), &tmp);

<<<<<<< HEAD
	return pos / 2.0f;
=======
	return pos/2;
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
}
