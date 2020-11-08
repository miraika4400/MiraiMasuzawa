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
// �}�N����`
//*****************
#define SERVER_PORT_NUM 12345      //�ڑ���T�[�o�[�̐ڑ���|�[�g
#define BUFFER_NUM 256             //����M�o�b�t�@�[�T�C�Y
#define SERVER_IP "52.155.109.73"  //�T�[�o�[��IP�A�h���X
//#define SERVER_IP "127.0.0.1"    //�T�[�o�[��IP�A�h���X

//*****************
// �N���X��`
//*****************
//�N���C�A���g�N���X
class CTcpClient
{
public:
	// �񋓌^�錾
	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// �����L���O�ꗗ���擾
		COMMAND_TYPE_SET_RANKING		// �����L���O��ݒ�
	} COMMAND_TYPE;

	CTcpClient();
	~CTcpClient();

	//�����o�֐�
	static CTcpClient*Create(const char * pHostName, int nPortNum);
	static CTcpClient* Create(SOCKET sock);
	bool Init(const char * pHostName, int nPortNum);
	bool CTcpClient::Init(SOCKET sock);
	int Send(char*pSendData, int nSendDataSize);
	int Recv(char*pRecvBuf, int nRecvBufSize);
	void Close();
	void Release(void);

private:
	//�����o�ϐ�
	SOCKET m_sock;
};

#endif
