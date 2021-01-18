//===============================
//
// �W���C�p�b�h�w�b�_ [joypad.h]
// Author:���V ����
//
//===============================

// ��d�C���N���[�h�h�~
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

//===============================
// �C���N���[�h�t�@�C��
//===============================
#include "main.h"
#include "input.h"

//===============================
//�}�N����`
//===============================
#define MAX_JOYSTICK_NUM 4// JOYPAD�ڑ����
#define	NUM_JOY_MAX 256   // �{�^���̍ő吔
#define JOYPAD_SENSI_RATE 45000.0f // �R���g���[���[�̃X�e�B�b�N�̊��x

//===============================
//�N���X��`
//===============================
//�W���C�p�b�h�N���X
class CInputJoypad : public CInput
{
public:
	//�����o�֐�
	CInputJoypad();
	virtual ~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetJoystickPress(int nKey, int nId);
	bool GetJoystickTrigger(int nKey, int nId);
	bool GetJoystickRelease(int nKey, int nId);
	
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);

	// �����o�ϐ�
	BYTE m_aJoyState[NUM_JOY_MAX][MAX_JOYSTICK_NUM] = {};          // �W���C�p�b�h�̓��͏�񃏁[�N
	BYTE m_aJoyStateTrigger[NUM_JOY_MAX][MAX_JOYSTICK_NUM] = {};   // �W���C�p�b�h�̃g���K�[���
	BYTE m_aJoyStateRelease[NUM_JOY_MAX][MAX_JOYSTICK_NUM] = {};   // �W���C�p�b�h�̃����[�X���
	static DIJOYSTATE CInputJoypad::GetStick(const int nID);

private:
	static LPDIRECTINPUTDEVICE8 m_apDevice[MAX_JOYSTICK_NUM];
	DIDEVCAPS   m_diDevCaps;
	static int m_nJoyStickCont;

};

#endif