////////////////////////////////////////////////////
//
//    TitleLogoの処理[TitleLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "debug_log.h"
#include "manager.h"
#include "renderer.h"
char CDebugLog::m_aStr[DEBUG_LOG_MAX_STRING] = {};
LPD3DXFONT CDebugLog::m_pFont = NULL;

//=============================
// コンストラクタ
//=============================
CDebugLog::CDebugLog()
{
	
}

//=============================
// デストラクタ
//=============================
CDebugLog::~CDebugLog()
{
}

//=============================
// 初期化処理
//=============================
HRESULT CDebugLog::Init(void)
{
	// 文字列の初期化
	memset(&m_aStr, 0, sizeof(m_aStr));
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	return E_NOTIMPL;
}

//=============================
// 終了処理
//=============================
void CDebugLog::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================
// 書きこみ
//=============================
void CDebugLog::Print(const char * format, ...)
{
	va_list arg;
	va_start(arg, format);

	bool bVariable = false;// 変数かどうかのフラグ

	// 可変引数の判定
	for (const char* chC = format; *chC != '\0'; chC++)
	{
		// %が読み込まれたら次は変数が来る
		if (chC[0] == '%')
		{
			// 変数フラグを立てる
			bVariable = true;
		}

		if (bVariable)
		{
			if (chC[0] == 'd')
			{//%d（int）
				sprintf(m_aStr, "%s%d", m_aStr, va_arg(arg, int));
			}
			else if (chC[0] == 'f')
			{//%f（float）
				sprintf(m_aStr, "%s%f", m_aStr, va_arg(arg, float));
			}
			else if (chC[0] == 'c')
			{//%c（char）
				sprintf(m_aStr, "%s%c", m_aStr, va_arg(arg, char));

			}
			else if (chC[0] == 's')
			{//%s（*char）
				sprintf(m_aStr, "%s%s", m_aStr, va_arg(arg, const char*));
			}
			else
			{// 変数じゃないとき
				sprintf(m_aStr, "%s%c", m_aStr, chC[0]);
			}
			// 変数フラグの初期化
			bVariable = false;
		}
		else
		{// 変数じゃないとき
			sprintf(m_aStr, "%s%c", m_aStr, chC[0]);
		}
	}
	va_end(arg);
}

//=============================
// 描画処理
//=============================
void CDebugLog::Draw(void)
{
	// FPS表示と被んないようにサイズを決める
	RECT rect = { 20, 70, SCREEN_WIDTH, SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}
