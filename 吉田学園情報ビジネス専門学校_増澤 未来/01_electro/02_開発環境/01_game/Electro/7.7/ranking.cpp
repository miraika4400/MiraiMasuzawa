//===================================================
//
//    ランキングクラスの処理[ranking.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "tcp_client.h"
#include "ranking.h"
#include "number.h"

//**********************************
// マクロ定義
//**********************************
#define RANK_NUMBER_SIZE 30.0f       // ランクナンバーのサイズ
#define NOW_SCORE_SIZE 25.0f         // 現在スコアナンバーのサイズ
#define RANK_INIT_POS_X 440.0f       // ランクナンバー配置のXの初期位置
#define RANK_INIT_POS_Y 60.0f        // ランクナンバー配置のYの初期位置　　　　
#define NOW_SCORE_INIT_POS_X 510.0f  // 現在スコア配置のX初期位置
#define NOW_SCORE_POS_Y 665.0f       // 現在スコアのY位置
#define FLASH_COUNT 30               // 点滅のカウント
//**********************************
// 静的メンバ変数宣言
//**********************************
int      CRanking::m_nRanking[MAX_RANK] = {};    // ランキングのスコアの格納用
CNumber* CRanking::m_pNowScore[MAX_SCORE_DIGIT] = {}; // スコアポリゴン
int CRanking::m_nNowScore = 0;

//==================================
// コンストラクタ
//==================================
CRanking::CRanking()
{
	// ナンバーのクリア
	memset(m_apRankNumber, 0, sizeof(m_apRankNumber));
	memset(m_apScoreNumber, 0, sizeof(m_apScoreNumber));
	memset(m_pNowScore, 0, sizeof(m_pNowScore));
	m_nCntFlash = 0;
}

//==================================
// デストラクタ
//==================================
CRanking::~CRanking()
{
}


//==============================
// ランキング取得
//==============================
void CRanking::GetRanking(void)
{
	WSADATA wsaData;
	CTcpClient *pTcpClient = NULL;

	//WinSockの初期化
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//何らかの原因で初期化に失敗した
		printf("WSAStartup failed\n");
		return;
	}

	if (pTcpClient == NULL)
	{
		//メモリの確保
		pTcpClient = CTcpClient::Create(SERVER_IP, SERVER_PORT_NUM);

		if (pTcpClient == NULL)
		{
			//winsock終了処理
			WSACleanup();
			return;
		}
	}

	///////////////////////////////////////////////////

	// Send
	char aSendBuf[BUFFER_NUM];
	aSendBuf[0] = CTcpClient::COMMAND_TYPE_GET_RANKING;

	pTcpClient->Send(aSendBuf, strlen(aSendBuf));

	// Recv
	char aRecvBuf[BUFFER_NUM];
	memset(&aRecvBuf[0], 0, sizeof(aRecvBuf));

	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// スコアのコピー
		int nScore = *(int*)&aRecvBuf[nIndex];
		nScore = ntohl(nScore);
		nIndex += sizeof(int);
		m_nRanking[nCntRank] = nScore;
	}

	///////////////////////////////////////////////////

	//winsock終了処理
	WSACleanup();

	//メモリの解放
	pTcpClient->Release();
}

//==============================
// ランキング設定
//==============================
int CRanking::SetRanking(void)
{
	WSADATA wsaData;
	CTcpClient *pTcpClient = NULL;

	//WinSockの初期化
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//何らかの原因で初期化に失敗した
		printf("WSAStartup failed\n");
		return -1;
	}

	if (pTcpClient == NULL)
	{
		//メモリの確保
		pTcpClient = CTcpClient::Create(SERVER_IP, SERVER_PORT_NUM);

		if (pTcpClient == NULL)
		{
			//winsock終了処理
			WSACleanup();
			return -1;
		}
	}

	///////////////////////////////////////////////////

	char aSendBuf[BUFFER_NUM];

	// リクエスト
	aSendBuf[0] = CTcpClient::COMMAND_TYPE_SET_RANKING;
	// エンディアン変換
	int nTime = htonl(m_nNowScore);

	// タイムのコピー
	memcpy(&aSendBuf[1], &nTime, sizeof(int));

	int nLen = strlen(aSendBuf);
	pTcpClient->Send(aSendBuf, 13);

	// レスポンス

	char aRecvBuf[BUFFER_NUM];

	memset(&aRecvBuf[0], 0, sizeof(aRecvBuf));

	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int nRecvRank = (int)aRecvBuf[0];

	//nRecvRank = ntohl(nRecvRank);

	///////////////////////////////////////////////////

	//winsock終了処理
	WSACleanup();

	//メモリの解放
	pTcpClient->Release();

	return nRecvRank;
}

//==================================
// クリエイト
//==================================
CRanking * CRanking::Create(void)
{
	// メモリの確保
	CRanking *pRanking = new CRanking;

	// 初期化処理
	pRanking->Init();

	// オブジェクトタイプの設定
	pRanking->SetObjType(OBJTYPE_UI);

	return pRanking;
}

//==================================
// セットランキング
//==================================
void CRanking::SetRanking(int nScore)
{
	m_nNowScore = nScore;
	SetRanking();
	GetRanking();
	//// ランキング圏内か判定
	//if (m_nRanking[MAX_RANK - 1] < nScore)
	//{
	//	// ランキング最下位の上書き
	//	m_nRanking[MAX_RANK - 1] = nScore;

	//	// ソート
	//	for (int nCntRank = MAX_RANK-1; nCntRank > 0; nCntRank--)
	//	{
	//		if (m_nRanking[nCntRank] > m_nRanking[nCntRank - 1])
	//		{
	//			// 入れかえ
	//			int nSort = m_nRanking[nCntRank];
	//			m_nRanking[nCntRank] = m_nRanking[nCntRank - 1];
	//			m_nRanking[nCntRank - 1] = nSort;
	//		}
	//	}
	//}

	// 今のスコアの表示
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		// ナンバーのセット
		m_pNowScore[nCntDigit]->SetNumber((nScore % (int)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit)))) / (float)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit - 1))));
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CRanking::Init(void)
{
	// 色
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 最大桁数分ループ
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		switch (nCntRank)
		{
		case 0:
			// 一位の時のカラー
			col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;

		case 1:
			// 二位の時のカラー
			col = D3DXCOLOR(0.45f, 0.8f, 1.0f, 1.0f);
			break;

		case 2:
			// 三位の時のカラー
			col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
			break;

		default:
			// デフォルトのカラー
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		// ポリゴン生成
		m_apRankNumber[nCntRank] = CNumber::Create(nCntRank + 1,
			D3DXVECTOR3(RANK_INIT_POS_X, RANK_INIT_POS_Y + (RANK_NUMBER_SIZE* MAX_RANK/2) + (nCntRank * RANK_NUMBER_SIZE*2), 0.0f),
			D3DXVECTOR3(RANK_NUMBER_SIZE, RANK_NUMBER_SIZE, 0),
			col);

		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// ポリゴン生成
			m_apScoreNumber[nCntRank][nCntDigit] = CNumber::Create(0,
				D3DXVECTOR3((float)(RANK_INIT_POS_X+100 + nCntDigit * RANK_NUMBER_SIZE * 2), RANK_INIT_POS_Y + (float)((RANK_NUMBER_SIZE * MAX_RANK / 2) + (nCntRank * RANK_NUMBER_SIZE*2)), 0.0f),
				D3DXVECTOR3(RANK_NUMBER_SIZE, RANK_NUMBER_SIZE, 0),
				col);
		}
	}

	// 現在のスコア
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit] = CNumber::Create(0,
			D3DXVECTOR3((float)(NOW_SCORE_INIT_POS_X + 100 + nCntDigit * RANK_NUMBER_SIZE * 2), NOW_SCORE_POS_Y, 0.0f),
			D3DXVECTOR3(NOW_SCORE_SIZE, NOW_SCORE_SIZE, 0),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	m_nCntFlash = 0;
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CRanking::Uninit(void)
{
	// 最大ランク数
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (m_apRankNumber[nCntRank] != NULL)
		{
			m_apRankNumber[nCntRank]->Uninit();
		}
		// 最大桁数
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			if (m_apScoreNumber[nCntRank][nCntDigit] != NULL)
			{
				m_apScoreNumber[nCntRank][nCntDigit]->Uninit();
				delete m_apScoreNumber[nCntRank][nCntDigit];
				m_apScoreNumber[nCntRank][nCntDigit] = NULL;
			}
		}
	}

	// 今のスコアの描画
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit]->Uninit();
		delete m_pNowScore[nCntDigit];
		m_pNowScore[nCntDigit] = NULL;
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CRanking::Update(void)
{
	// カウントを進める
	m_nCntFlash++;
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// スコアとランキングの比較
			if (m_nNowScore != 0 && m_nNowScore == m_nRanking[nCntRank])
			{


				// 点滅させる
				if (m_nCntFlash % (FLASH_COUNT * 2) == 0)
				{// ON
					D3DXCOLOR col = m_apScoreNumber[nCntRank][nCntDigit]->GetColor();
					col.a = 0.4f;
					m_apScoreNumber[nCntRank][nCntDigit]->SetColor(col);
				}
				else if (m_nCntFlash % FLASH_COUNT == 0)
				{// OFF
					D3DXCOLOR col = m_apScoreNumber[nCntRank][nCntDigit]->GetColor();
					col.a = 1.0f;
					m_apScoreNumber[nCntRank][nCntDigit]->SetColor(col);
				}

			}

			// 更新処理
			m_apScoreNumber[nCntRank][nCntDigit]->Update();
			// 各桁数の計算
			m_apScoreNumber[nCntRank][nCntDigit]->SetNumber((m_nRanking[nCntRank] % (int)(powf(10.0f, (MAX_SCORE_DIGIT - nCntDigit)))) / (float)(powf(10.0, (MAX_SCORE_DIGIT - nCntDigit - 1))));
		}

	}
}

//==================================
// 描画処理
//==================================
void CRanking::Draw(void)
{
	// ランキング描画
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// 順位
		m_apRankNumber[nCntRank]->Draw();

		// スコア
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			m_apScoreNumber[nCntRank][nCntDigit]->Draw();
		}
	}

	// 今のスコアの描画
	for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
	{
		m_pNowScore[nCntDigit]->Draw();
	}
}
