//===============================
//
// マウスヘッダ [mouse.h]
// Author:増澤 未来
//
//===============================

// 二重インクルード防止
#ifndef _MOUSE_H_
#define _MOUSE_H_

//===============================
// インクルード
//===============================
#include "main.h"
#include "input.h"

//===============================
// マクロ定義
//===============================
#define MAX_MOUSE_BOTTON 3

//===============================
// クラス定義
//===============================

//マウスクラス
class CInputMouse : public CInput
{
public:
	//メンバ関数
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
private:
	// メンバ変数
	BYTE m_aMouseState[MAX_MOUSE_BOTTON];          // キーボードの入力情報ワーク
	BYTE m_aMouseStateTrigger[MAX_MOUSE_BOTTON];   // キーボードのトリガー情報
	BYTE m_aMouseStateRelease[MAX_MOUSE_BOTTON];   // キーボードのリリース情報
};

#endif