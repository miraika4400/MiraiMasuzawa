//=============================================================================
//
// bgヘッダ [bg.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BG_H_
#define _BG_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//  前方宣言
//*****************************
class CScene3d;

//*****************************
// マクロ定義
//*****************************
#define BG_TEXTURE_NUM 3 // BGで使うテクスチャ数
#define BG_PARTS_NUM 3   // BGで使うポリゴン数

//*****************************
// クラス定義
//*****************************

// 背景クラス
class CBg : public CScene
{
public:
	//メンバ関数
	CBg();
	~CBg();

	static CBg *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[BG_TEXTURE_NUM];
	CScene3d *m_apScene3d[BG_PARTS_NUM];

	float m_afTexMoveU[BG_PARTS_NUM]; // テクスチャをずらす用(U/横)
	float m_afTexMoveV[BG_PARTS_NUM]; // テクスチャをずらす用(V/縦)
};

#endif