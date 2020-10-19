//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.cpp]
// Author : 増澤　未来
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "tcp_listener.h"
#include "tcp_client.h"

//=============================================================================
// CTcpListener生成
//=============================================================================
CTcpListener* CTcpListener::Create(int nPortNum)
{
	CTcpListener*pListener;

	// TCP接続受付クラスのインスタンス生成
	pListener = new CTcpListener;

	if (pListener != NULL)
	{
		//初期化
		if (!pListener->Init(nPortNum))
		{
			pListener = NULL;
		}
	}
	else
	{
		printf("メモリの確保に失敗しました。\n");
	}


	return pListener;
}

//=============================================================================
// CTcpListenerコンストラクタ
//=============================================================================
CTcpListener::CTcpListener()
{
	m_sockServer = -1;
}

//=============================================================================
// CTcpListenerデストラクタ
//=============================================================================
CTcpListener::~CTcpListener()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
bool CTcpListener::Init(int nPortNum)
{
	struct  sockaddr_in addr;

	//接続受付用ソケットの作成
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//接続受付情報の設定
	addr.sin_family = AF_INET;               //IPv4
	addr.sin_port = htons(SERVER_PORT_NUM);  //ポート番号
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //すべての接続受け付ける

											 //ソケットにIPアドレスとポート番号を設定する
	bind(m_sockServer, (struct sockaddr*)&addr, sizeof(addr));

	//接続要求受付を開始する
	listen(m_sockServer, MAX_ACCEPT_CONN);

	return true;
}

//=============================================================================
// 接続受付処理
//=============================================================================
CTcpClient* CTcpListener::Accept()
{


	if (m_sockServer < 0) {
		return NULL;
	}

	//ソケット接続を受け付ける（受け付けるまでプログラムはブロックされる）
	int nAddrLength = sizeof(m_addrClient);

	CTcpClient*pClient = NULL;
	// 接続受付処理
	pClient = CTcpClient::Create(accept(m_sockServer, (struct sockaddr*)&m_addrClient, &nAddrLength));
	printf("connect:[%s]\n", inet_ntoa(m_addrClient.sin_addr));

	return pClient;
}

//=============================================================================
// 破棄処理
//=============================================================================
void CTcpListener::Release()
{
	// 切断処理
	Close();

	// インスタンス削除
	delete this;
}

//=============================================================================
// クローズ処理
//=============================================================================
void CTcpListener::Close()
{
	if (m_sockServer < 0) {
		return;
	}

	//切断処理
	closesocket(m_sockServer);
	m_sockServer = -1;
}


