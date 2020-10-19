//=============================================================================
//
// enemyヘッダ [enemy.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// クラス定義
//*****************************


// エネミークラス
class CEnemy : public CScene3d
{
public:
	// エネミーの種類
	typedef enum
	{
		ENEMYTYPE_YELLOW = 0,
		ENEMYTYPE_PURPLE,
		ENEMYTYPE_RED,
		ENEMYTYPE_MAX,
	}ENEMYTYPE;

	// ステート
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_MAX,
	}STATE;

	//メンバ関数
	CEnemy();
	~CEnemy();

	static CEnemy *Create(const D3DXVECTOR3 pos,const ENEMYTYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	static void SpawnManager(void);
	static int GetEnemyNum(void) { return m_nNumEnemy; }                   // エネミー数の取得処理
	static void PlusEnemy(void) { m_nNumEnemy++; }                         // エネミー数を増やす
	static void MinusEnemy(void) { m_nNumEnemy--; }                        // エネミー数を増やす

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	static std::list<CEnemy*>*GetEnemylist(void) { return &m_enemyList; }

	void HitAttack(int nDamage);
private:
	void MoveManager(void);
	void ParticleCreate(void);

	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_MAX]; // テクスチャのポインタ
	static std::list<CEnemy*> m_enemyList;  // エネミーリスト
	static int m_nCntSpawn;      // スポーンカウント
	static int m_nNumEnemy;                  // エネミーとボスの数管理用

	D3DXVECTOR3 m_move;   // 移動量
	ENEMYTYPE m_type;     // 敵の種類
	STATE m_state;        // ステート
	int m_nCntMove;       // 移動カウント
	int m_nCntBullet;     // 弾の間隔確保用
	int m_nLife;          // ライフ
	int m_nStateCount;    // ステート遷移用
	int m_nCntEncount;    // エンカウント状態解除時のカウント用
	float m_fMoveAngle;   // 移動角度
	bool m_bPlusAngle;    // ジグザグ動くエネミーの角度をプラスするかマイナスするか
	
	
};

#endif