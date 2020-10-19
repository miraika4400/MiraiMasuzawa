//=============================================================================
//
// enemyヘッダ [enemy.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ENEMYSPAWN_H_
#define _ENEMYSPAWN_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"
#include "enemy.h"

//*****************************
// 前方宣言
//*****************************
class CParticle;

//*****************************
// マクロ定義
//*****************************
#define ENEMYSPAWN_PARTICLE_NUM 6

//*****************************
// クラス定義
//*****************************

// エネミースポーン演出のクラス
class CEnemySpawn : public CScene
{
public:

	//メンバ関数
	CEnemySpawn();
	~CEnemySpawn();

	static CEnemySpawn *Create(const D3DXVECTOR3 pos, const CEnemy::ENEMYTYPE type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_pos;                              // 座標
	CParticle*m_pParticle[ENEMYSPAWN_PARTICLE_NUM]; // パーティクルのポインタ
	CEnemy::ENEMYTYPE m_enemyType;                  // エネミータイプ
	int m_nCntFrame;                                // フレームカウント用
	D3DXCOLOR m_col;                                // 色
};

#endif