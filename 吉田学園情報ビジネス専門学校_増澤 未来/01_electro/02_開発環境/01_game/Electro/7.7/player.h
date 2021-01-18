//=============================================================================
//
// playerヘッダ [player.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// 前方宣言
//*****************************
class CGauge;
class CFeverLogo;
class CFeverScreen;

//*****************************
// マクロ定義
//*****************************
#define PLAYER_MAX_HP    8.0f   // 最大体力
#define PLAYER_MAX_MP    50.0f  // 最大MP
#define IN_FEVER_NUM    50.0f   // フィーバーの入る値
#define PLAYER_GAUGE_NUM 3      // 使うゲージ数
// ゲージ情報
// (HP)
#define PLAYER_LIFE_WIDTH 300.0f // HPバーの横幅の最大
#define PLAYER_LIFE_HEGHT 15     // HPバーの縦幅
#define PLAYER_LIFE_POS_X 150.0f // X位置
#define PLAYER_LIFE_POS_Y 618.0f // Y位置
// (MP)
#define PLAYER_MP_WIDTH 300.0f // HPバーの横幅の最大
#define PLAYER_MP_HEGHT 12     // HPバーの縦幅
#define PLAYER_MP_POS_X 150.0f // X位置
#define PLAYER_MP_POS_Y 645.0f // Y位置
// (フィーバー)
#define FEVER_WIDTH 600.0f // HPバーの横幅の最大
#define FEVER_HEGHT 10     // HPバーの縦幅
#define FEVER_POS_X SCREEN_WIDTH/2-600.0f/2
#define FEVER_POS_Y 680.0f // Y位置

//*****************************
// クラス定義
//*****************************

//プレイヤークラス
class CPlayer : public CScene3d
{
public:

	//============
	// 状態列挙
	//============
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,     
		STATE_SPAWN,    
		STATE_GAMEOVER,
		STATE_MAX,      
	}STATE;

	//============
	// メンバ関数
	//============
	CPlayer();
	~CPlayer();

	// static
	static CPlayer *Create(const D3DXVECTOR3 pos); // クラス生成
	static HRESULT Load(void);                     // テクスチャロード
	static void Unload(void);                      // テクスチャアンロード
	static bool IsAlive(void) { return m_bAlive;}  // 生きているかの取得

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画
	void HitAttack(int nDamage);                 // 攻撃ヒット時の処理
	void RecoveryLife(const int nRecovery);      // 体力の回復
	void RecoveryMp(float fRecoveryMp);          // MPの回復
	void ProgressFever(float fAdd);              // フィーバー値を増やす

    // 移動量の取得・セット
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetState(const D3DXVECTOR3 move) { m_move = move; }
	// ステートの取得・セット
	STATE GetState(void) { return m_state;}
	void SetState(const STATE state) { m_state = state; }
	// ボムフラグのセット・取得
	void SetBomb(const bool bBomb) { m_bBomb = bBomb; }
	bool HaveBomb(void) { return m_bBomb; }
	// 残機数ののセット・取得
	void SetRemain(const int nRemain) { m_nRemain = nRemain; }
	int GetRemain(void) { return m_nRemain; }
	//MPののセット・取得
	void SetMp(const float fMp) { m_fMp = fMp; }
	float GetMp(void) { return m_fMp; }
	// HPのセット・取得・回復
	void SetLife(const float fLife) { m_fLife = fLife; }
	float GetLife(void) { return m_fLife; }
	// ボムフラグのセット・取得
	void SetFever(const bool bfever) { m_bFever = bfever; }
	bool GetFever(void) { return m_bFever; }
private:
	void StateManager(void);   // 状態管理
	void ControlManager(void); // 操作の管理
	void FeverManager(void);   // フィーバーの処理

	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	static bool m_bAlive;                 // 生存中か

	CGauge *    m_pGauge[PLAYER_GAUGE_NUM];  // ゲージクラスのポインタ
	D3DXVECTOR3 m_move;                      // 移動量
	int         m_nCntBullet;                // 弾の間隔確保用
	int         m_nStateCount;               // ステート遷移用
	STATE       m_state;                     // ステート
	bool        m_bBomb;                     // ボムを持っているか
	int         m_nRemain;                   // 残機
	int         m_nBulletInterval;           // 連射速度
	float       m_fMp;                       // MP
	float       m_fLife;                     // 体力
	float       m_fRotAngle;                 // 回転角度
	float       m_fRotAngleDist;             // 回転角度の目標値
	float       m_fFeverGauge;               // フィーバー値
	bool        m_bFever;                    // フィーバー中の判定
	CFeverLogo* m_pFeverLogo;                // フィーバーのロゴ
	CFeverScreen* m_pFeverScreen;            // フィーバー画面
};

#endif