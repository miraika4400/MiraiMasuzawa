//=============================================================================
//
// courseヘッダ [course.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _COURSE_H_
#define _COURSE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CCourse : public CModel
{
public:

	// コースタイプの列挙
	typedef enum
	{
		COURSE_EASY=0,// イージー
		COURSE_NORMAL,// ノーマル
		COURSE_HARD,  // ハード
		COURSE_MAX,
	}COURSETYPE;

	//メンバ関数
	CCourse();
	~CCourse();
	static CCourse *Create(COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// メッシュの取得
	LPD3DXMESH GetMesh(void) { return m_pMeshModel[m_courseType]; }
private:
	// 当たり判定
	void CollisionPlayer(void);       // プレイヤー
	void CollisionDestination(void);  // 移動目標
	void CollisionItem(void);         // アイテム

	// メンバ変数
	static LPD3DXMESH m_pMeshModel[COURSE_MAX];	        //メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel[COURSE_MAX];    //マテリアル情報へのポインタ
	static DWORD m_nNumMatModel[COURSE_MAX];	        //マテリアル情報の数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE]; // テクスチャ
	static char *m_pTexPath[COURSE_MAX];                // テクスチャのパス
	COURSETYPE m_courseType;                            // コースタイプ
	int m_nCntInit;                                     // 初期化されてからのフレーム数
};

#endif