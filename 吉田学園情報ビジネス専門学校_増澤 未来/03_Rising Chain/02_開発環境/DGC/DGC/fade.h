//=============================================================================
//
// fadeヘッダ [fade.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _FADE_H_
#define _FADE_H_
//***********************************
//インクルード
//***********************************
#include "main.h"
#include "manager.h"

//***********************************
// 前方宣言
//***********************************
class CPolygon;

//=============================
//クラス定義
//=============================

//シーンクラス
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX,
	}FADE;

	CFade();
	~CFade();
	static CFade*Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CManager::MODE mode);
private:
	// メンバ変数
	CPolygon*m_pPolygon; // フェードポリゴン
	D3DXCOLOR m_col;     // カラー
	CManager::MODE m_modeNext;
	FADE m_fade;
};

#endif