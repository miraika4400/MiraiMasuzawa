//===============================
//
// �}�E�X�w�b�_ [mouse.h]
// Author:���V ����
//
//===============================

// ��d�C���N���[�h�h�~
#ifndef _MOUSE_H_
#define _MOUSE_H_

//===============================
// �C���N���[�h
//===============================
#include "main.h"
#include "input.h"

//===============================
// �}�N����`
//===============================
#define MAX_MOUSE_BOTTON 3
#define MOUSE_SENSI_RATE 700.0f     // �J�����̃}�E�X���x

//===============================
// �N���X��`
//===============================

//�L�[�{�[�h�N���X
class CInputMouse : public CInput
{
public:
	//�����o�֐�
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetMousePress(int nKey);
	bool GetMouseTrigger(int nKey);
	bool GetMouseRelease(int nKey);
	D3DXVECTOR3 GetMousePos(void);
	void SetMousePos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetMouseMove(void);

private:
	// �����o�ϐ�
	BYTE m_aMouseState[MAX_MOUSE_BOTTON];          // �L�[�{�[�h�̓��͏�񃏁[�N
	BYTE m_aMouseStateTrigger[MAX_MOUSE_BOTTON];   // �L�[�{�[�h�̃g���K�[���
	BYTE m_aMouseStateRelease[MAX_MOUSE_BOTTON];   // �L�[�{�[�h�̃����[�X���
	DIMOUSESTATE m_mousState;
};

#endif