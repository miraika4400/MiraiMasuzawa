//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.h]
// Author : 増澤 未来
//
//=============================================================================
#ifndef _TCPLISTENER_H_
#define _TCPLISTENER_H_

#include <winsock2.h>
#include "main.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CTcpClient;

//*********************************************************
// TCP接続受付クラス
//*********************************************************
class CTcpListener
{
public:
	static CTcpListener* Create(int nPortNum); //クリエイト
	CTcpListener();                            //コンストラクタ
	~CTcpListener();                           //デストラクタ
	bool Init(int nPortNum);                   //初期化
	CTcpClient* Accept();                      //接続
	void Release();                            //リリース
	void Close();                              //切断

	char* GetIP(void) { return inet_ntoa(m_addrClient.sin_addr); }//クライアントのIPアドレスの取得

private:
	SOCKET m_sockServer;//サーバー用ソケット
	struct  sockaddr_in m_addrClient;//クライアント情報
};

#endif
