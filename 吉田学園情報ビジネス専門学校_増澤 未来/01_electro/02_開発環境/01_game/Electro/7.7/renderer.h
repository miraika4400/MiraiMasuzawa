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

//=============================
//クラス定義
//=============================

//レンダラークラス
class CRenderer
{
public:
	//============
	// メンバ関数
	//============
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);                     // 初期化
	void Uninit(void);                                         // 終了
	void Update(void);                                         // 更新
	void Draw(void);                                           // 描画
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; } // デバイスの取得

	D3DXCOLOR GetBuffColor(void) { return m_buffCol; }         // バックバッファのカラーの取得
	void      SetBuffColor(D3DXCOLOR col) { m_buffCol = col; } // バックバッファのカラーの設定
private:

#ifdef _DEBUG
	void DrawFPS(void); //FPS数の表示
#endif
	
	//============
	// メンバ変数
	//============
	PDIRECT3D9        m_pD3D;       // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice; // Deviceオブジェクト(描画に必要)
	D3DXCOLOR         m_buffCol;    // バックバッファカラー

#ifdef _DEBUG
	LPD3DXFONT        m_pFont;      // フォントへのポインタ
#endif
};

#endif