//=============================================================================
//
// courseヘッダ [course.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _WALL_H_
#define _WALL_H_

//*****************************
// インクルード
//*****************************
#include "model.h"
#include "course.h"

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CWall : public CModel
{
public:


	//メンバ関数
	CWall();
	~CWall();
	static CWall *Create(CCourse::COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void CollisionPlayer(void);

	// メンバ変数
	static LPD3DXMESH m_pMeshModel[CCourse::COURSE_MAX];	        //メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel[CCourse::COURSE_MAX];    //マテリアル情報へのポインタ
	static DWORD m_nNumMatModel[CCourse::COURSE_MAX];	        //マテリアル情報の数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE]; // テクスチャ
	static char *m_pTexPath[CCourse::COURSE_MAX];                // テクスチャのパス
	CCourse::COURSETYPE m_courseType;                            // コースタイプ
};

#endif