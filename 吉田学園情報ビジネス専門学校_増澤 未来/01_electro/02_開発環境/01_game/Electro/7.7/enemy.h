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
	//============
	// 列挙定義
	//============
	// エネミーの種類
	typedef enum
	{
		ENEMYTYPE_YELLOW = 0,  // 黄色エネミー
		ENEMYTYPE_PURPLE,      // 紫エネミー
		ENEMYTYPE_RED,         // 赤エネミー
		ENEMYTYPE_MAX,         // エネミータイプの最大数
	}ENEMYTYPE;

	// ステート
	typedef enum
	{
		STATE_NORMAL = 0, // 通常
		STATE_DAMAGE,     // ダメージ
		STATE_MAX,        // 状態の最大数
	}STATE;

	//============
	// 構造体定義
	//============
	// エネミーの湧き管理用構造体
	typedef struct
	{
		int nMaxEnemy;   // エネミーの最大数
		int nSpawnCount; // エネミーの湧く頻度
		int nPhaseTime;  // フェーズを切り替える時間
		int nCircleNum;  // サークル数
	}ENEMY_SPAWN;

	//============
	//メンバ関数
	//============
	CEnemy();
	~CEnemy();

	// static
	static CEnemy *Create(const D3DXVECTOR3 pos,const ENEMYTYPE type);     // クラス生成
	static HRESULT Load(void);                                             // テクスチャロード
	static void Unload(void);                                              // テクスチャアンロード
	static void LoadDefficult(void);                                       // 難易度別の敵の湧くデータの読み込み
	static void UnloadDefficult(void);                                     // 難易度別の敵の湧くデータのアンロード
	static void SpawnManager(void);                                        // エネミーのわきの管理
	static int GetEnemyNum(void) { return m_nNumEnemy; }                   // エネミー数の取得処理
	static void PlusEnemy(void) { m_nNumEnemy++; }                         // エネミー数を増やす
	static void MinusEnemy(void) { m_nNumEnemy--; }                        // エネミー数を増やす
	static std::list<CEnemy*>*GetEnemylist(void) { return &m_enemyList; }  // リストの取得

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	D3DXVECTOR3 GetMove(void) { return m_move; } // 移動量の取得
	void HitAttack(int nDamage);                 // 被弾処理
private:
	void MoveManager(void);    // 移動の管理
	void ParticleCreate(void); // パーティクル生成

	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_MAX]; // テクスチャのポインタ
	static std::list<CEnemy*> m_enemyList;               // エネミーリスト
	static int m_nCntSpawn;                              // スポーンカウント
	static int m_nNumEnemy;                              // エネミーとボスの数管理用
	static ENEMY_SPAWN * m_pSpawn;                       // エネミーのわき管理用
	static int m_nMaxPhaseNum;                           // フェーズ最大数
	static int m_nPhaseNum;                              // フェーズ数

	D3DXVECTOR3 m_move;        // 移動量
	ENEMYTYPE   m_type;        // 敵の種類
	STATE       m_state;       // ステート
	int         m_nCntMove;    // 移動カウント
	int         m_nCntBullet;  // 弾の間隔確保用
	int         m_nLife;       // ライフ
	int         m_nStateCount; // ステート遷移用
	int         m_nCntEncount; // エンカウント状態解除時のカウント用
	float       m_fMoveAngle;  // 移動角度
	bool        m_bPlusAngle;  // ジグザグ動くエネミーの角度をプラスするかマイナスするか

	
};

#endif