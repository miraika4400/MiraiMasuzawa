//=============================================================================
//
// debug_log�w�b�_ [debug_log.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"

//*****************************
//�}�N����`
//*****************************
#define DEBUG_LOG_MAX_STRING 1024

//*****************************
//�N���X��`
//*****************************

//�f�o�b�O���O�N���X
class CDebugLog
{
public:
	// �����o�֐�
	CDebugLog();
	~CDebugLog();
	static HRESULT Init(void);
	static void Uninit(void);
	static void Print(const char *format, ...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont; // �t�@����
	static char m_aStr[DEBUG_LOG_MAX_STRING];  // ������
};

#endif