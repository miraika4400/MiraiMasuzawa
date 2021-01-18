//////////////////////////////////////////////
//
//    TCP_Client�w�b�_
//    Author:���V ����
//
//////////////////////////////////////////////

//��d�C���N���[�h�h�~
#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

//*****************
// �C���N���[�h
//*****************
#include "main.h"

//*****************
// �N���X��`
//*****************
//�N���C�A���g�N���X
class CTcpClient
{
public:
	//============
	// �����o�֐�
	//============
	CTcpClient();
	~CTcpClient();

	static CTcpClient*Create(const char * pHostName, int nPortNum); // �N���X����
	static CTcpClient* Create(SOCKET sock);                         // �N���X����

	bool Init(const char * pHostName, int nPortNum); // ������
	bool CTcpClient::Init(SOCKET sock);              // �I��
	int Send(char*pSendData, int nSendDataSize);     // ����
	int Recv(char*pRecvBuf, int nRecvBufSize);       // �󂯎��
	void Close();                                    // �\�P�b�g�����
	void Release(void);                              // ���

private:
	//�����o�ϐ�
	SOCKET m_sock; // �\�P�b�g
};

#endif
