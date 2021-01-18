//=============================================================================
//
// debug_logヘッダ [debug_log.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

//*****************************
//インクルード
//*****************************
#include "main.h"

//*****************************
//マクロ定義
//*****************************
#define DEBUG_LOG_MAX_STRING 1024

//*****************************
//クラス定義
//*****************************

//デバッグログクラス
class CDebugLog
{
public:
	// メンバ関数
	CDebugLog();
	~CDebugLog();
	static HRESULT Init(void);
	static void Uninit(void);
	static void Print(const char *format, ...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont; // ファンと
	static char m_aStr[DEBUG_LOG_MAX_STRING];  // 文字列
};

#endif