//=============================================================================
//
// manacircreヘッダ [manacircre.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _MANACIRCRE_H_
#define _MANACIRCRE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// マクロ定義
//*****************************
#define CIRCLE_MAX 6  // サークルの最大数 

//*****************************
// クラス定義
//*****************************

// サークルクラス
class CManaCircle : public CScene3d
{
public:

	//メンバ関数
	CManaCircle();
	~CManaCircle();
	static CManaCircle *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static void SpawnManager(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//static std::list<CManaCircle*>*GetCirclelist(void) { return &m_circleList; }
	static void SetCircleNum(int nNum) { m_nNumCircle = nNum; }
	static void SetCircleMax(int nNum) { m_nMaxCircle = nNum; }
	static CManaCircle *GetCircle(int nIndex) { return m_apCircle[nIndex]; }
private:
	static D3DXVECTOR3 GetRandCirclePos(void);
	void AnimationManager(void);
	void CollisionCircle(void);
	void LostCircle(void);

	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	static CManaCircle *m_apCircle[CIRCLE_MAX];
	//static std::list<CManaCircle*> m_circleList;  // サークルリスト
	static int m_nNumCircle;     // 出現しているサークルの数
	static int m_nMaxCircle;     // 出現するサークルの最大数
	int nCntRecovery;   // サークルに入っている間のカウント
	int m_nIndex;
	bool bInCircle;     // サークルに入っているか

	int m_nCntAnim;     // アニメーションカウント
	int m_nAnimX;       // アニメーションX軸
	int m_nAnimY;       // アニメーションY軸

};

#endif