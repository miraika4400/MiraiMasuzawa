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
#define SERVER_IP "52.185.164.185" //�T�[�o�[��IP�A�h���X
#define SERVER_TEX_PATH "data/Texts/serveraddress.txt"

//#define SERVER_IP "127.0.0.1" //�T�[�o�[��IP�A�h���X

//*****************
// �N���X��`
//*****************
//�N���C�A���g�N���X
class CTcpClient
{
public:
	//============
	// ��ԗ�
	//============
	// API
	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// �����L���O�ꗗ���擾
		COMMAND_TYPE_SET_RANKING		// �����L���O��ݒ�
	} COMMAND_TYPE;

	//============
	// �����o�֐�
	//============
	CTcpClient();
	~CTcpClient();

	static CTcpClient*Create(void);                                 // �N���X����*�e�L�X�g�t�@�C���g��
	static CTcpClient*Create(const char * pHostName, int nPortNum); // �N���X����*pIP�A�|�[�g�������ɓ����
	static CTcpClient*Create(SOCKET sock);                          // �N���X����

	bool Init(const char * pHostName, int nPortNum); // ������
	bool CTcpClient::Init(SOCKET sock);              // ������
	int Send(char*pSendData, int nSendDataSize);     // ����
	int Recv(char*pRecvBuf, int nRecvBufSize);       // ���炤
	void Close();                                    // �\�P�b�g�����
	void Release(void);                              // ���

private:
	static void LoadAddress(const char * pTextPath, int* pPotnNum, char* pAdress); // �e�L�X�g�t�@�C���̓ǂݍ���

    //============
    // �����o�ϐ�
    //============
	SOCKET m_sock;// �\�P�b�g
};

#endif
