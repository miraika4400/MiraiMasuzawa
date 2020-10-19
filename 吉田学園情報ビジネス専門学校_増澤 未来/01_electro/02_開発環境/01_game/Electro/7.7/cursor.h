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
	//メンバ関数
	CCursor();
	~CCursor();
	static CCursor *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos3d(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャのポインタ
<<<<<<< HEAD
=======
	float m_fRotAngle;                       // 回転角度
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
	int m_nCntAnim;     // アニメーションカウント
	int m_nAnimX;       // アニメーションX軸
	int m_nAnimY;       // アニメーションY軸
};

#endif