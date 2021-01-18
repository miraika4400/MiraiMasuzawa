//=============================================================================
//
// cursorヘッダ [cursor.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CURSOR_H_
#define _CURSOR_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// クラス定義
//*****************************

// カーソルクラス
class CCursor : public CScene2d
{
public:
	//============
	// メンバ関数
	//============
	CCursor();
	~CCursor();
	static CCursor *Create(const D3DXVECTOR3 pos); // クラス生成
	static HRESULT Load(void);                     // テクスチャロード
	static void Unload(void);                      // テクスチャアンロード

	HRESULT Init(void);         // 初期化
	void Uninit(void);          // 終了
	void Update(void);          // 更新
	void Draw(void);            // 描画
	D3DXVECTOR3 GetPos3d(void); // カーソルのワールド座標の取得

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャのポインタ
	float m_fRotAngle;                    // 回転角度
	int   m_nCntAnim;                     // アニメーションカウント
	int   m_nAnimX;                       // アニメーションX軸
	int   m_nAnimY;                       // アニメーションY軸
};

#endif