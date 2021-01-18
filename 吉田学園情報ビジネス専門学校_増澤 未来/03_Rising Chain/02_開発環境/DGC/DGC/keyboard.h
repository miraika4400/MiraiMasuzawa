//===============================
//
// キーボードヘッダ [keyborad.h]
// Author:増澤 未来
//
//===============================

// 二重インクルード防止
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//===============================
// インクルードファイル
//===============================
#include "main.h"
#include "input.h"

//キーボードクラス
class CInputKeyboard : public CInput
{
public:
	//メンバ関数
	CInputKeyboard();
	virtual ~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyPress(int nKey);
	bool GetKeyTrigger(int nKey);
	bool GetKeyRelease(int nKey);

private:
	// メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];          // キーボードの入力情報ワーク
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];   // キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];   // キーボードのリリース情報
};

#endif