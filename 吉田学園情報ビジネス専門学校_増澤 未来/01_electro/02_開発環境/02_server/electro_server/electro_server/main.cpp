//=============================================================================
//
// メイン [main.cpp]
// Author : 増澤 未来
//
//=============================================================================

//*******************************
// インクルード
//*******************************
#include "main.h"
#include "protcol_ranking.h"
#include "tcp_client.h"
#include "tcp_listener.h"
#include <thread>

//*******************************
// マクロ定義
//*******************************
#define RANKINGTEXT_PATH "RankingData.txt" // ランキングのパス

//*******************************
// プロトタイプ宣言
//*******************************
void ReadFile(void);
void WriteFile(void);
int SortRank(void);
int RankUpdate(int  nRankData);
void WorkerThread(CTcpClient*pTcpClient);

//*******************************
// グローバル変数
//*******************************
int g_nRankData[MAX_RANKING + 1] = {};  // ランキングデータ

//===============================
// メイン関数
//===============================
void main(void)
{
	//変数宣言
	WSADATA wsaData;
	SOCKET sockServer;


	//WinSockの初期化
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//何らかの原因で初期化に失敗した
		printf("WSAStartup failed\n");
		return ;
	}
	memset(&g_nRankData, 0, sizeof(g_nRankData));
	ReadFile();

	//TcpListenerポインタ
	CTcpListener *pListener = NULL;

	if (pListener == NULL)
	{
		//インスタンス生成
		pListener = CTcpListener::Create(SERVER_PORT_NUM);

		if (pListener != NULL)
		{
			//サーバーをループ
			while (1)
			{
				//接続の受付
				CTcpClient*pClient = NULL;

				if (pClient == NULL)
				{
					//接続待ち
					pClient = pListener->Accept();
					if (pClient != NULL)
					{
						//処理の並列化
						std::thread th(WorkerThread, pClient);
						th.detach();
					}
					else
					{
						continue;
					}
				}
			}
		}
		else
		{
			printf("メモリの確保に失敗しました。\n");
			return ;
		}
	}

	pListener->Release();
	//待ち受けソケットをクローズ
	closesocket(sockServer);

	//winsock終了処理
	WSACleanup();

}

//===============================
// ファイル読み込み
//===============================
void ReadFile(void)
{
	FILE*pFile = NULL;

	// ファイル読み込み
	pFile = fopen(RANKINGTEXT_PATH, "r");

	if (pFile != NULL)
	{
		// 読み込み
		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			fscanf(pFile, "%d", &g_nRankData[nCntRank]);
		}
		fclose(pFile);
	}
}

//===============================
// ファイル書き込み
//===============================
void WriteFile(void)
{
	FILE*pFile = NULL;

	// ファイル読み込み
	pFile = fopen(RANKINGTEXT_PATH, "w");

	if (pFile != NULL)
	{
		// 書き込み
		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			fprintf(pFile, "%d\n", g_nRankData[nCntRank]);
		}
		fclose(pFile);
	}
}

//===============================
// ランキングのソート処理
//===============================
int SortRank(void)
{
	int nNum = 11;
	
	for (int nCntRank = 0; nCntRank < MAX_RANKING + 1; nCntRank++)
	{
		for (int nCntSort = nCntRank + 1; nCntSort < MAX_RANKING + 1; nCntSort++)
		{
			
			if (g_nRankData[nCntRank] < g_nRankData[nCntRank])
			{
				// 入れ替え処理
				int  nSaveData = g_nRankData[nCntRank];
				g_nRankData[nCntRank] = g_nRankData[nCntSort];
				g_nRankData[nCntSort] = nSaveData;

				if (nCntSort == nNum - 1)
				{
					nNum = nCntRank + 1;
				}
			}
		}
	}

	if (nNum == 11) { nNum = 0; }

	return nNum;
}

//===============================
// ランキングの更新処理
//===============================
int RankUpdate(int  nRankData)
{
	int nRank = -1;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		// ランクが埋まってなかった時
		if (g_nRankData[nCntRank] == NULL)
		{
			g_nRankData[nCntRank] = nRankData;
			return nCntRank + 1;
		}

		if (g_nRankData[nCntRank] < nRankData)
		{
			nRank = nCntRank;
			break;
		}
	}

	if (nRank >= 0)
	{
		for (int nCntRank = MAX_RANKING - 1; nCntRank > nRank; nCntRank--)
		{
			g_nRankData[nCntRank] = g_nRankData[nCntRank - 1];
		}
		g_nRankData[nRank] = nRankData;
	}

	return nRank + 1;
}

//===============================
// ランキングサーバーの処理
//===============================
void WorkerThread(CTcpClient * pTcpClient)
{
	char aRecvBuffer[BUFFER_NUM]; // データ受信用
	char aSendBuffer[BUFFER_NUM]; // データ送信用

	// データの受信
	int nRecvNum = pTcpClient->Recv(&aRecvBuffer[0], sizeof(aRecvBuffer));
	if (nRecvNum <= 0)
	{
		//ソケット接続をクローズ
		pTcpClient->Release();
		return;
	}

	int nScore;
	int nSendRank;

	switch (aRecvBuffer[0])
	{
	case COMMAND_TYPE_GET_RANKING:
		// ランキング取得API
		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			
			// エンディアン変換
			nScore = htonl(g_nRankData[nCntRank]);
			// タイムのコピー
			memcpy(&aSendBuffer[nCntRank * sizeof(int)], &nScore, sizeof(int));
		}

		// ランキングデータの送信
		pTcpClient->Send(aSendBuffer, sizeof(int) * MAX_RANKING);


		break;
	case COMMAND_TYPE_SET_RANKING:
		// ランキング設定API

		int  nRankUpdate;
		// 時間のコピー
		nScore = *(int*)&aRecvBuffer[1];
		nRankUpdate = ntohl(nScore);
		
		//ランキングデータの更新
		nSendRank = RankUpdate(nRankUpdate);
		
		// ランキングデータの書き込み
		WriteFile();

		memcpy(&aSendBuffer[0], (char*)&nSendRank, sizeof(char));
		// ランキングデータの送信
		pTcpClient->Send(aSendBuffer, sizeof(char));

		break;
	default:
		break;
	}

	//ソケット接続をクローズ
	pTcpClient->Release();
}
