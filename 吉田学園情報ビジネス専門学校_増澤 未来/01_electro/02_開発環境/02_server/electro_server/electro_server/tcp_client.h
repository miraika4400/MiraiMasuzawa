//////////////////////////////////////////////
//
//    TCP_Clientヘッダ
//    Author:増澤 未来
//
//////////////////////////////////////////////

//二重インクルード防止
#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

//*****************
// インクルード
//*****************
#include "main.h"

//*****************
// クラス定義
//*****************
//クライアントクラス
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	//メンバ関数
	static CTcpClient*Create(const char * pHostName, int nPortNum);
	static CTcpClient* Create(SOCKET sock);
	bool Init(const char * pHostName, int nPortNum);
	bool CTcpClient::Init(SOCKET sock);
	int Send(char*pSendData, int nSendDataSize);
	int Recv(char*pRecvBuf, int nRecvBufSize);
	void Close();
	void Release(void);

private:
	//メンバ変数
	SOCKET m_sock; // ソケット
};

#endif
