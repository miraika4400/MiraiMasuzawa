//=============================================================================
//
// レンダラーヘッダ [renderer.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _RENDERER_H_
#define _RENDERER_H_

//インクルード
#include "main.h"
#include "game.h"

//=============================
//クラス定義
//=============================

//レンダラークラス
class CRenderer
{
public:
	//メンバ関数
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	bool SetUpViewport(int nNumber);
	D3DVIEWPORT9 GetViewPort(int nCount) { return m_view_port[nCount]; };

private:
#ifdef _DEBUG
	//メンバ関数
	void DrawFPS(void);
#endif
	//メンバ変数
	PDIRECT3D9        m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;	    // Deviceオブジェクト(描画に必要)
	D3DVIEWPORT9 m_view_port[MAX_PLAYER_NUM];			// ビューポート
#ifdef _DEBUG
	LPD3DXFONT				m_pFont;	// フォントへのポインタ
#endif
};

#endif