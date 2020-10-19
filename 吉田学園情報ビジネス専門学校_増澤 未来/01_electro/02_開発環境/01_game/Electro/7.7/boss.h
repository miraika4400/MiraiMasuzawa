//=============================================================================
//
// bossヘッダ [boss.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BOSS_H_
#define _BOSS_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
//マクロ定義
//*****************************
#define BOSS_MAX_LIFE 100  // ボスの体力
#define BOSS_SPAWN_SEC 60  //ボスがわく秒数

//*****************************
// クラス定義
//*****************************

//ボスクラス
class CBoss : public CScene3d
{
public:

	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX,
	}STATE;

	//メンバ関数
	CBoss();
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static bool IsAlive(void) { return m_bIsAlive; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetLife(void) { return m_nLife; }
	void HitAttack(int nDamage);

private:
	void AttackManager(void);

	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャのポインタ
	static bool m_bIsAlive;               // 生存中か

	int m_nRandAttack;     // 攻撃パターンのランダム
	int m_nLife;          // ライフ
	float m_fBulletAngle; // 弾を撃つ角度
	int m_nCntBullet;     // 弾の間隔確保用
	STATE m_state;        // ステート
	int m_nStateCount;    // ステート遷移用
	int m_nCntAnim;       // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
	bool m_bAttack;       // 攻撃しないタイミングを作る用
};

#endif