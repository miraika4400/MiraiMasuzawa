//=============================================================================
//
// checkpointヘッダ [checkpoint.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// マクロ定義
//*****************************
#define MAX_CHECKPOINT_NUM 256

//*****************************
// 前方宣言
//*****************************
class CCollision;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CCheckPoint : public CScene
{
public:
	
	//メンバ関数
	CCheckPoint();
	~CCheckPoint();
	static CCheckPoint *Create(CCourse::COURSETYPE type);
	static CCheckPoint *GetCheckPoint(void) { return m_pCheckPoint; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	CCollision*GetCollision(int nInex) { return m_pCollision[nInex]; }
	int GetCheckPointNum(void) { return m_nNumPoint; }

private:
	void SaveCheckpoint(void);
	void LoadCheckpoint(CCourse::COURSETYPE type);
	// メンバ変数
	static CCheckPoint * m_pCheckPoint;
	CCourse::COURSETYPE m_type;
	int m_nNumPoint; // チェックポイントの数
	CCollision*m_pCollision[MAX_CHECKPOINT_NUM];    // コリジョンクラスのポインタ
	D3DXVECTOR3 m_pointPos[MAX_CHECKPOINT_NUM];   // チェックポイントの座標
};

#endif