////////////////////////////////////////////////////
//
//    mauseクラスの処理[mause.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "mouse.h"

//******************************
// コンストラクタ
//******************************
CInputMouse::CInputMouse()
{
	// 0クリア
	memset(m_aMouseState, 0, sizeof(m_aMouseState));
	memset(m_aMouseStateTrigger, 0, sizeof(m_aMouseStateTrigger));
	memset(m_aMouseStateRelease, 0, sizeof(m_aMouseStateRelease));
}

//******************************
// デストラクタ
//******************************
CInputMouse::~CInputMouse()
{
}


//******************************
// 初期化処理
//******************************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス（マウス）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//******************************
// 更新処理
//******************************
void CInputMouse::Update(void)
{
	int nCntKey;
	DIMOUSESTATE  ms;
	HRESULT hr;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &ms);

		//デバイスからデータを取得
		if (SUCCEEDED(hr))
		{
			for (nCntKey = 0; nCntKey < MAX_MOUSE_BOTTON; nCntKey++)
			{
				//キートリガー
				m_aMouseStateTrigger[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) &  ms.rgbButtons[nCntKey];

				//キーリリース
				m_aMouseStateRelease[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) & ~ms.rgbButtons[nCntKey];

				//キープレス情報を保存
				m_aMouseState[nCntKey] = ms.rgbButtons[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}

//******************************
// プレス情報の取得
//******************************
bool CInputMouse::GetMousePress(int nKey)
{
	return m_aMouseState[nKey] & 0x80 ? true : false;
}

//******************************
// トリガー情報の取得
//******************************
bool CInputMouse::GetMouseTrigger(int nKey)
{
	return m_aMouseStateTrigger[nKey] & 0x80 ? true : false;
}

//******************************
// リリース情報の取得
//******************************
bool CInputMouse::GetMouseRelease(int nKey)
{
	return m_aMouseStateRelease[nKey] & 0x80 ? true : false;
}

//******************************
// 座標の取得
//******************************
D3DXVECTOR3 CInputMouse::GetMousePos(void)
{
	// マウス座標を取得する
	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	return D3DXVECTOR3(p.x, p.y, 0);
}

//******************************
// 座標のセット
//******************************
void CInputMouse::SetMousePos(D3DXVECTOR3 pos)
{
	POINT p;
	p.x = pos.x;
	p.y = pos.y;
	// クライアント座標をスクリーン座標に変換する
	ClientToScreen(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	SetCursorPos(p.x, p.y);
}
