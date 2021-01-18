//=============================================================================
//
// TCP送受信の処理 [tcp_client.cpp]
// Author : 増澤　未来
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "tcp_client.h"

CTcpClient * CTcpClient::Create(void)
{
	//メモリの確保
	CTcpClient*pTcpClient = new CTcpClient;

	if (pTcpClient != NULL)
	{
		int nPort = 0;
		char chAdress[64] = {};

		LoadAddress(SERVER_TEX_PATH, &nPort, &chAdress[0]);
		//初期化
		if (!pTcpClient->Init(chAdress, nPort))
		{
			pTcpClient->Close();
			pTcpClient = NULL;
		}
	}
	else
	{
		printf("メモリの確保に失敗しました。\n");
		return NULL;
	}
	return pTcpClient;
}

//=============================================================================
// CTcpClient生成 (connectによる接続)
//=============================================================================
CTcpClient* CTcpClient::Create(const char* pHostName, int nPortNum)
{
	//メモリの確保
	CTcpClient*pTcpClient = new CTcpClient;

	if (pTcpClient != NULL)
	{
		//初期化
		if (!pTcpClient->Init(pHostName, nPortNum))
		{
			pTcpClient->Close();
			pTcpClient = NULL;
		}
	}
	else
	{
		printf("メモリの確保に失敗しました。\n");
		return NULL;
	}
	return pTcpClient;

}

//=============================================================================
// CTcpClient生成 (acceptによる接続)
//=============================================================================
CTcpClient* CTcpClient::Create(SOCKET sock)
{
	// TCP送受信クラスのインスタンス生成
	CTcpClient*pTcpClient = new CTcpClient;

	if (pTcpClient != NULL)
	{
		//初期化処理
		if (!pTcpClient->Init(sock))
		{
			pTcpClient->Release();
			pTcpClient = NULL;
		}
	}
	else
	{
		printf("メモリの確保に失敗しました。\n");
	}
	return pTcpClient;

}

//=============================================================================
// CTcpClientコンストラクタ
//=============================================================================
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//=============================================================================
// CTcpClientデストラクタ
//=============================================================================
CTcpClient::~CTcpClient()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
bool CTcpClient::Init(const char* pHostName, int nPortNum)
{
	//変数宣言
	struct sockaddr_in addr;

	//接続受付情報の設定
	addr.sin_family = AF_INET;                           //IPv4
	addr.sin_port = htons(nPortNum);                     //ポート番号
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);    //サーバーのIP

	//接続用ソケットの生成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET)
	{
		//printf("socket:%d\n", WSAGetLastError());
		return 1;
	}

	//コネクト
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		//printf("connect:%dan", WSAGetLastError());

		//キー入力待ち
		rewind(stdin);
		getchar();

		closesocket(m_sock);
		return false;
	}

	return true;

}

//=============================================================================
// 初期化処理
//=============================================================================
bool CTcpClient::Init(SOCKET sock)
{
	m_sock = sock;
	return true;
}

//=============================================================================
// 送信処理
//=============================================================================
int CTcpClient::Send(char* pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		return 0;
	}

	// 入力した文字列の送信
	return send(m_sock, &pSendData[0], nSendDataSize, 0);
}

//=============================================================================
// 受信処理
//=============================================================================
int CTcpClient::Recv(char* pRecvData, int nRecvBufferSize)
{
	if (m_sock < 0) {
		return 0;
	}
	// メモリーの0クリア
	memset(pRecvData, 0, nRecvBufferSize);

	// データの受信
	return recv(m_sock, pRecvData, nRecvBufferSize, 0);
}

//=============================================================================
// 破棄処理
//=============================================================================
void CTcpClient::Release()
{
	// 切断処理
	Close();
	// メモリの解放
	delete this;
}

//=============================================================================
// テキストファイルの読み込み
//=============================================================================
void CTcpClient::LoadAddress(const char * pTextPath, int* pPotnNum, char* pAdress)
{
	// ファイルオープン
	FILE*pFile = NULL;
	pFile = fopen(pTextPath, "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// ポート番号とIPアドレスの読み込み
		fscanf(pFile,"%*s %*s %d", pPotnNum);
		fscanf(pFile,"%*s %*s %s", pAdress);

		// ファイルクローズ
		fclose(pFile);
	}
}

//=============================================================================
// クローズ処理
//=============================================================================
void CTcpClient::Close()
{
	if (m_sock < 0)
	{
		return;
	}
	closesocket(m_sock);
	m_sock = -1;
}