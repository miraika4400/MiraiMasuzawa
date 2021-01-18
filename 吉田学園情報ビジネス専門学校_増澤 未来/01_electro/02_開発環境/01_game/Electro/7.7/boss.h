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
// 前方宣言
//*****************************
class CGauge;

//*****************************
//マクロ定義
//*****************************
#define BOSS_MAX_LIFE 200.0f  // ボスの体力
#define BOSS_SPAWN_SEC 60  //ボスがわく秒数
#define BOSS_HP_WIDTH 500.0f                               // HPバーの横幅の最大
#define BOSS_HP_HEGHT 10                                   // HPバーの縦幅
#define BOSS_HP_POS_X SCREEN_WIDTH/2 - (BOSS_HP_WIDTH/2)   // X位置
#define BOSS_HP_POS_Y 100.0f                               // Y位置

//*****************************
// クラス定義
//*****************************

//ボスクラス
class CBoss : public CScene3d
{
public:
	//============
	// 列挙定義
	//============
	// 状態
	typedef enum
	{
		STATE_NORMAL = 0, // 通常
		STATE_DAMAGE,     // ダメージ
		STATE_DEATH,      // 死亡
		STATE_MAX,        // 状態の最大数
	}STATE;

	//============
	// メンバ関数
	//============
	CBoss();
	~CBoss();

	// static
	static CBoss *Create(const D3DXVECTOR3 pos);     // クラス生成
	static HRESULT Load(void);                       // テクスチャロード
	static void Unload(void);                        // テクスチャアンロード
	static bool IsAlive(void) { return m_bIsAlive; } // 生きているか取得

	HRESULT Init(void);// 初期化
	void Uninit(void); // 終了
	void Update(void); // 更新
	void Draw(void);   // 描画

	float GetLife(void) { return m_fLife; } // 体力の取得 
	void HitAttack(int nDamage);            // 被弾処理

private:
	void AttackManager(void); // 攻撃の処理

	//============
	// メンバ関数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャのポインタ
	static bool m_bIsAlive;               // 生存中か

	CGauge * m_pGauge;         // ゲージクラスのポインタ
	int      m_nRandAttack;    // 攻撃パターンのランダム
	float    m_fLife;          // ライフ
	float    m_fBulletAngle;   // 弾を撃つ角度
	int      m_nCntBullet;     // 弾の間隔確保用
	STATE    m_state;          // ステート
	int      m_nStateCount;    // ステート遷移用
	int      m_nCntAnim;       // アニメーションカウント
	int      m_nAnimX;         // アニメーションX軸
	int      m_nAnimY;         // アニメーションY軸
	bool     m_bAttack;        // 攻撃しないタイミングを作る用
};

#endif