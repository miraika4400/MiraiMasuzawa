//=============================================================================
//
// startヘッダ [start.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _START_H_
#define _START_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene3d.h"
#include "course.h"

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CStart : public CScene3d
{
public:
	// メンバ関数
	CStart();
	~CStart();
	static CStart *Create(CCourse::COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	 // テクスチャへのポインタ
	int m_nCntAnim;       // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
	CCourse::COURSETYPE m_type; // コースのタイプ
	DWORD m_currentTime;  // 時間を計る用
	int m_nTime;          // テクスチャ切り替えタイム
};
#endif