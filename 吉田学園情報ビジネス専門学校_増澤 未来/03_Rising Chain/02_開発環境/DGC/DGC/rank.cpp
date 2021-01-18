////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "rank.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// マクロ定義
//**********************************
#define TEXTURE_PATH "./data/Textures/Ranking　number.png"    // テクスチャのパス
#define MAX_ANIMATION_X 4                                     // アニメーション数 横
#define MAX_ANIMATION_Y 1                                     // アニメーション数 縦
#define SIZE D3DXVECTOR3( 192.25f / 2.5f,110.0f/2.5f, 0.0f)   // サイズ*画像の解像度に合わせる

// 位置
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 70 ,  SCREEN_HEIGHT    - 70 , 0.0f)  // 画面分割してないとき
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT/2) - 70 , 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 70 , (SCREEN_HEIGHT/2) - 50 , 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 70 ,  SCREEN_HEIGHT    - 70 , 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT/2) - 50 , 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 70 , (SCREEN_HEIGHT)   - 50 , 0.0f)  // プレイヤー3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT)   - 50 , 0.0f)  // プレイヤー4

// リザルト時のサイズ
#define RESULT_SIZE D3DXVECTOR3( 192.25f / 1.5f,110.0f / 1.5f, 0.0f)   // 画像の解像度に合わせる
// リザルト時の位置
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   - 60.0f, 0.0f)  // 画面分割してないとき
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // プレイヤー3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // プレイヤー4

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CRank::m_pTexture = NULL;

//=============================
// コンストラクタ
//=============================
CRank::CRank() :CScene2d(OBJTYPE_UI)
{
	// 変数のクリア
	m_nCntAnim = 0;           // アニメーションカウント
	m_nAnimX = 0;             // アニメーションX軸
	m_nAnimY = 0;             // アニメーションY軸
	m_nPlayerNum = 0;         // プレイヤー番号
	m_resultPos = VEC3_ZERO;  // リザルト時のポス
	m_resultSize = VEC3_ZERO; // リザルト時のサイズ
}

//=============================
// デストラクタ
//=============================
CRank::~CRank()
{
}

//=============================
// クリエイト
//=============================
CRank * CRank::Create(int nPlayerNum)
{
	// メモリの確保
	CRank *pRank = new CRank;
	// プレイヤー番号の代入
	pRank->m_nPlayerNum = nPlayerNum;
	// 初期化
	pRank->Init();
	return pRank;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CRank::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CRank::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// 順位の管理
//==================================
void CRank::RankManage(void)
{
	// プレイヤー配列
	CPlayer*pPlayer[MAX_PLAYER_NUM] = {};

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();

	// 配列にプレイヤーを格納
	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (CGame::GetPlayer(nCnt) != NULL)
		{
			pPlayer[nCnt] = CGame::GetPlayer(nCnt);
		}
	}

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (pPlayer[nCnt] != NULL)
		{
			for (int nCnt2 = 0; nCnt2 < nNumPlayer; nCnt2++)
			{
				if (pPlayer[nCnt2] != NULL)
				{
					if (!pPlayer[nCnt]->GetGoalFlag() && !pPlayer[nCnt2]->GetGoalFlag())
					{
						if (pPlayer[nCnt]->GetCheckNum() > pPlayer[nCnt2]->GetCheckNum())
						{// チェックポイントの数でソート
							CPlayer*pSave = pPlayer[nCnt];
							pPlayer[nCnt] = pPlayer[nCnt2];
							pPlayer[nCnt2] = pSave;
							continue;
						}
						else if (pPlayer[nCnt]->GetCheckNum() == pPlayer[nCnt2]->GetCheckNum())
						{// チェックポイント数が一緒だった時
							D3DXVECTOR3 checkPos = CCheckPoint::GetCheckPoint()->GetCollision(pPlayer[nCnt]->GetCheckNum())->GetPos();

							// チェックポイントとの距離でソート
							float fDistance = sqrtf(powf(checkPos.x - pPlayer[nCnt]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt]->GetPos().z, 2));
							if (fDistance < sqrtf(powf(checkPos.x - pPlayer[nCnt2]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt2]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt2]->GetPos().z, 2)))
							{
								CPlayer*pSave = pPlayer[nCnt];
								pPlayer[nCnt] = pPlayer[nCnt2];
								pPlayer[nCnt2] = pSave;
								continue;
							}
						}
					}
				}
			}
		}
	}

	// 配列の順番がそのまま順位
	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (pPlayer[nCnt] != NULL)
		{
			pPlayer[nCnt]->SetRank(nCnt + 1);
		}
	}
}

//=============================
// 初期化処理
//=============================
HRESULT CRank::Init(void)
{
	CScene2d::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nAnimX = 0;     // アニメーションX軸
	m_nAnimY = 0;     // アニメーションY軸

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

	// サイズの設定
	SetSize(SIZE);
	
	m_resultSize = RESULT_SIZE;      // リザルト時のサイズ
	// テクスチャの設定
	BindTexture(m_pTexture);

	// 座標の設定

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();
	// プレイヤー番号によって分岐
	switch (m_nPlayerNum)
	{
	case 0:
		// プレイヤー1

		if (nNumPlayer == 1)
		{// 分割なし
			SetPos(POS_PLAYER1_1);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if(nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER1_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// 四分割
			SetPos(POS_PLAYER1_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// プレイヤー2

		if (nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER2_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// 四分割
			SetPos(POS_PLAYER2_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// プレイヤー3
		SetPos(POS_PLAYER3);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// プレイヤー4
		SetPos(POS_PLAYER4);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}
	

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CRank::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// 更新処理
//=============================
void CRank::Update(void)
{
	if (!CGame::GetResultFlag())
	{
		m_nAnimX = CGame::GetPlayer(m_nPlayerNum)->GetRank() - 1;     // アニメーションX軸

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
	else
	{// リザルト状態の時
		SetPos(m_resultPos);
		SetSize(m_resultSize);
	}
}


//=============================
// 描画処理
//=============================
void CRank::Draw(void)
{
	CScene2d::Draw();
}