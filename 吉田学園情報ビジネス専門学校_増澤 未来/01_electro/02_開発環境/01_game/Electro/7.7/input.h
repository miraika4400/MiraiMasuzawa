//===============================
//
// インプットヘッダ [input.h]
// Author:増澤 未来
//
//===============================

//二重インクルード防止
#ifndef _INPUT_H_
#define _INPUT_H_

//===============================
//マクロ定義
//===============================
#define	NUM_KEY_MAX (256)

//===============================
// インクルードファイル
//===============================
#include "main.h"

//===============================
//前方宣言
//===============================
class CInputKeyboard;

//===============================
//クラス定義
//===============================

// インプットクラス
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput; // DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice; // 入力デバイス(キーボード)へのポインタ
private:
	CInputKeyboard *m_pKeybord;     // キーボードクラス
};



#endif