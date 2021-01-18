//=============================================================================
//
// FeverScreenヘッダ [fever_screen.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _FEVERSCREEN_H_
#define _FEVERSCREEN_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//クラス定義
//*****************************

// フィーバーロゴクラス
class CFeverScreen : public CScene2d
{
public:
	//============
	// メンバ関数
	//============
	CFeverScreen();
	~CFeverScreen();
	
	static CFeverScreen *Create(void); // クラス生成
	static HRESULT Load(void);       // テクスチャロード
	static void Unload(void);        // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ

	int m_nCntAnim;                // アニメーションカウント
	int m_nAnimX;                  // アニメーションX軸
	int m_nAnimY;                  // アニメーションY軸
	int m_nCntColor;               // 色を変えるカウント
	D3DXCOLOR m_distColor;         // カラー変更時の目標カラー
};

#endif