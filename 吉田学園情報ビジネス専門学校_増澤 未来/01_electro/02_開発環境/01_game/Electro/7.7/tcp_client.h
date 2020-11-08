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
// マクロ定義
//*****************
#define SERVER_PORT_NUM 12345      //接続先サーバーの接続先ポート
#define BUFFER_NUM 256             //送受信バッファーサイズ
#define SERVER_IP "52.155.109.73"  //サーバーのIPアドレス
//#define SERVER_IP "127.0.0.1"    //サーバーのIPアドレス

//*****************
// クラス定義
//*****************
//クライアントクラス
class CTcpClient
{
public:
	// 列挙型宣言
	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// ランキング一覧を取得
		COMMAND_TYPE_SET_RANKING		// ランキングを設定
	} COMMAND_TYPE;

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
	SOCKET m_sock;
};

#endif
