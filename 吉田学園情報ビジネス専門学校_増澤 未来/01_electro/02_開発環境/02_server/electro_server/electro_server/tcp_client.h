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
	//============
	// メンバ関数
	//============
	CTcpClient();
	~CTcpClient();

	static CTcpClient*Create(const char * pHostName, int nPortNum); // クラス生成
	static CTcpClient* Create(SOCKET sock);                         // クラス生成

	bool Init(const char * pHostName, int nPortNum); // 初期化
	bool CTcpClient::Init(SOCKET sock);              // 終了
	int Send(char*pSendData, int nSendDataSize);     // 送る
	int Recv(char*pRecvBuf, int nRecvBufSize);       // 受け取る
	void Close();                                    // ソケットを閉じる
	void Release(void);                              // 解放

private:
	//メンバ変数
	SOCKET m_sock; // ソケット
};

#endif
