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
#define SERVER_IP "52.185.164.185" //サーバーのIPアドレス
#define SERVER_TEX_PATH "data/Texts/serveraddress.txt"

//#define SERVER_IP "127.0.0.1" //サーバーのIPアドレス

//*****************
// クラス定義
//*****************
//クライアントクラス
class CTcpClient
{
public:
	//============
	// 状態列挙
	//============
	// API
	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// ランキング一覧を取得
		COMMAND_TYPE_SET_RANKING		// ランキングを設定
	} COMMAND_TYPE;

	//============
	// メンバ関数
	//============
	CTcpClient();
	~CTcpClient();

	static CTcpClient*Create(void);                                 // クラス生成*テキストファイル使う
	static CTcpClient*Create(const char * pHostName, int nPortNum); // クラス生成*pIP、ポートを引数に入れる
	static CTcpClient*Create(SOCKET sock);                          // クラス生成

	bool Init(const char * pHostName, int nPortNum); // 初期化
	bool CTcpClient::Init(SOCKET sock);              // 初期化
	int Send(char*pSendData, int nSendDataSize);     // 送る
	int Recv(char*pRecvBuf, int nRecvBufSize);       // もらう
	void Close();                                    // ソケットを閉じる
	void Release(void);                              // 解放

private:
	static void LoadAddress(const char * pTextPath, int* pPotnNum, char* pAdress); // テキストファイルの読み込み

    //============
    // メンバ変数
    //============
	SOCKET m_sock;// ソケット
};

#endif
