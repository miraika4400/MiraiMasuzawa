////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "chain_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "polygon.h"

//**********************************
// マクロ定義
//**********************************
#define TEXTURE_PATH "./data/Textures/Chain.png"     // テクスチャのパス
#define NUBER_SIZE   D3DXVECTOR3( 15.0f*1.5f, 30.0f*1.5f, 0.0f)     // 数字サイズ*画像の解像度に合わせる
#define STRING_SIZE  D3DXVECTOR3( 500.0f/10.0f, 220.0f/10.0f, 0.0f) // "chain"文字サイズ*画像の解像度に合わせる

// 位置
#define POS_PLAYER1_1 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // 画面分割してないとき
#define POS_PLAYER1_2 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define POS_PLAYER1_4 D3DXVECTOR3( 0                   + 90.0f , 0                 + 85.0f , 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define POS_PLAYER2_2 D3DXVECTOR3( 0                   + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define POS_PLAYER2_4 D3DXVECTOR3((SCREEN_WIDTH/2)     + 90.0f , 0                 + 85.0f , 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define POS_PLAYER3   D3DXVECTOR3( 0                   + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // プレイヤー3
#define POS_PLAYER4   D3DXVECTOR3((SCREEN_WIDTH/2)     + 90.0f , (SCREEN_HEIGHT/2) + 85.0f , 0.0f)  // プレイヤー4

// リザルト時のサイズ
#define RESULT_NUMBER_SIZE D3DXVECTOR3( 15.0f*2.5f, 30.0f*2.5f, 0.0f)     // 数字サイズ*画像の解像度に合わせる

// リザルト時の位置
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/2                   +60, 0.0f)  // 画面分割してないとき
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  -40, SCREEN_HEIGHT/4                   +60, 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // プレイヤー3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  -40, SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 +60, 0.0f)  // プレイヤー4

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CChainUi::m_pTexture = NULL;

//=============================
// コンストラクタ
//=============================
CChainUi::CChainUi() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	memset(&m_apNumber, 0, sizeof(m_apNumber)); // チェイン数字
	m_nPlayerNum = 0;                         // プレイヤー番号
	m_nDegit = 1;                             // 表示桁数
	m_pos = VEC3_ZERO;                        // 中心座標
	m_nChain = 0;                             // チェイン数
	m_pPolygon = NULL;                        // 文字画像
	m_resultPos = VEC3_ZERO;                  // リザルト時のポス
	m_resultSize = VEC3_ZERO;                 // リザルト時のサイズ
}

//=============================
// デストラクタ
//=============================
CChainUi::~CChainUi()
{
}

//=============================
// クリエイト
//=============================
CChainUi * CChainUi::Create(int nPlayerNum)
{
	// メモリの確保
	CChainUi *pChainUi = new CChainUi;
	// プレイヤー番号の代入
	pChainUi->m_nPlayerNum = nPlayerNum;
	// 初期化
	pChainUi->Init();
	return pChainUi;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CChainUi::Load(void)
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
void CChainUi::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// 初期化処理
//=============================
HRESULT CChainUi::Init(void)
{
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
			m_pos = (POS_PLAYER1_1);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if(nNumPlayer == 2)
		{// 二分割
			m_pos = (POS_PLAYER1_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// 四分割
			m_pos = (POS_PLAYER1_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// プレイヤー2

		if (nNumPlayer == 2)
		{// 二分割
			m_pos = (POS_PLAYER2_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// 四分割
			m_pos = (POS_PLAYER2_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// プレイヤー3
		m_pos = (POS_PLAYER3);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// プレイヤー4
		m_pos = (POS_PLAYER4);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}



	float fPosX = m_pos.x + (((NUBER_SIZE.x * 2)*m_nDegit) / 2) - NUBER_SIZE.x;// X軸位置
																			   // ナンバークラスの生成
	for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(0, D3DXVECTOR3(fPosX, m_pos.y, 0.0f), NUBER_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// X軸をずらす
		fPosX -= NUBER_SIZE.x * 2;
	}
	
	m_pPolygon = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y + NUBER_SIZE.y+5, 0.0f), STRING_SIZE);
	m_pPolygon->SetTexture(m_pTexture);
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CChainUi::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
			delete m_apNumber[nCnt];
			m_apNumber[nCnt] = NULL;
		}
	}

	if (m_pPolygon != NULL)
	{
		m_pPolygon->Uninit();
		delete m_pPolygon;
		m_pPolygon = NULL;
	}
	Release();
}


//=============================
// 更新処理
//=============================
void CChainUi::Update(void)
{
	if (!CGame::GetResultFlag())
	{// リザルト状態じゃないとき
		m_nChain = CGame::GetPlayer(m_nPlayerNum)->GetChainNum();
		int nIndex = MAX_CHAIN_DIGIT - 1;
		for (int nCntDigit = 0; nCntDigit < MAX_CHAIN_DIGIT; nCntDigit++)
		{

			m_apNumber[nIndex]->Update();

			m_apNumber[nIndex]->SetNumber((m_nChain % (int)(powf(10.0f, (MAX_CHAIN_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_CHAIN_DIGIT - nCntDigit - 1))));
			nIndex--;
		}

		// 桁数
		if (m_nChain >= 100)
		{
			m_nDegit = MAX_CHAIN_DIGIT;
		}
		else if (m_nChain >= 10)
		{
			m_nDegit = 2;
		}
		else if (m_nChain >= 1)
		{
			m_nDegit = 1;
		}
		else
		{
			m_nDegit = 0;
		}

		float fPosX = m_pos.x + (((NUBER_SIZE.x * 2)*m_nDegit) / 2) - NUBER_SIZE.x;// X軸位置
																						  // ナンバークラスの生成
		for (int nCnt = 0; nCnt < m_nDegit; nCnt++)
		{
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3(fPosX, m_pos.y, 0.0f));
			// X軸をずらす
			fPosX -= NUBER_SIZE.x * 2;
		}
	}
	else
	{// リザルト時

		// 最大チェイン数を表示
		m_nChain = CGame::GetPlayer(m_nPlayerNum)->GetMaxChain();
		int nIndex = MAX_CHAIN_DIGIT - 1;
		for (int nCntDigit = 0; nCntDigit < MAX_CHAIN_DIGIT; nCntDigit++)
		{

			m_apNumber[nIndex]->Update();

			m_apNumber[nIndex]->SetNumber((m_nChain % (int)(powf(10.0f, (MAX_CHAIN_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_CHAIN_DIGIT - nCntDigit - 1))));
			nIndex--;
		}
		m_pos = m_resultPos;

		float fPosX = m_pos.x + (((RESULT_NUMBER_SIZE.x * 2)*MAX_CHAIN_DIGIT) / 2) - RESULT_NUMBER_SIZE.x;// X軸位置
																										  // ナンバークラスの生成
		for (int nCnt = 0; nCnt < MAX_CHAIN_DIGIT; nCnt++)
		{
			m_apNumber[nCnt] ->SetPos(D3DXVECTOR3(fPosX, m_pos.y, 0.0f));
			m_apNumber[nCnt]->SetSize(RESULT_NUMBER_SIZE);
			// X軸をずらす
			fPosX -= RESULT_NUMBER_SIZE.x * 2;
		}

		m_pPolygon->SetPos(D3DXVECTOR3(m_resultPos.x+ RESULT_NUMBER_SIZE.x * 4, m_resultPos.y + 30, 0.0f));
	}
}


//=============================
// 描画処理
//=============================
void CChainUi::Draw(void)
{
	int nLoop = 0;
	if (!CGame::GetResultFlag())
	{
		nLoop = m_nDegit;
	}
	else
	{
		nLoop = MAX_CHAIN_DIGIT;
	}
	// ループ数と桁数を合わせる
	for (int nCnt = 0; nCnt < nLoop; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			if (m_pPolygon != NULL&&nCnt == 0)
			{
				// chain文字列の描画
				m_pPolygon->Draw();
			}
			// chain数字の描画
			m_apNumber[nCnt]->Draw();
		}
	}
}