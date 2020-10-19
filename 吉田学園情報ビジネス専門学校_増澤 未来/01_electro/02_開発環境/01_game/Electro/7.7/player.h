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
// マクロ定義
//*****************************
#define PLAYER_MAX_MP 50
#define PLAYER_MAX_HP 8

//*****************************
// クラス定義
//*****************************

//プレイヤークラス
class CPlayer : public CScene3d
{
public:

	// 状態列挙
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_SPAWN,
		STATE_GAMEOVER,
		STATE_MAX,
	}STATE;

	// テクスチャの八方向のV軸
	typedef enum
	{
		DIRECTION_DOWN = 0,   // 下
		DIRECTION_LEFT,		  // 左
		DIRECTION_RIGHT,	  // 右
		DIRECTION_UP,		  // 上
		DIRECTION_LEFT_DOWN,  // 左下
		DIRECTION_LEFT_UP,	  // 左上
		DIRECTION_RIGHT_DOWN, // 右下
		DIRECTION_RIGHT_UP,	  // 右上
		DIRECTION_MAX,
	}DIRECTION;

	//メンバ関数
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static bool IsAlive(void) { return m_bAlive;}

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	// ステートの取得・セット
	STATE GetState(void) { return m_state;}
	void SetState(const STATE state) { m_state = state; }

	void HitAttack(int nDamage);
	void RecoveryLife(const int nRecovery);
	// ボムフラグのセット・取得
	void SetBomb(const bool bBomb) { m_bBomb = bBomb; }
	bool HaveBomb(void) { return m_bBomb; }
	// 残機数ののセット・取得
	void SetRemain(const int nRemain) { m_nRemain = nRemain; }
	int GetRemain(void) { return m_nRemain; }
	//MPののセット・取得
	void SetMp(const float fMp) { m_fMp = fMp; }
	float GetMp(void) { return m_fMp; }
	void RecoveryMp(float fRecoveryMp);
	// HPのセット・取得・回復
	void SetLife(const int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

private:
	void StateManager(void);
	void ControlManager(void);
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 m_move;   // 移動量
	int m_nCntBullet;     // 弾の間隔確保用
	int m_nStateCount;    // ステート遷移用
	STATE m_state;        // ステート
	bool m_bBomb;         // ボムを持っているか
	static bool m_bAlive; // 生存中か
	int m_nRemain;        // 残機
	float m_fMp;            // MP
	int m_nLife;          // 体力
	float m_fRotAngle;      // 回転角度
	float m_fRotAngleDist;  // 回転角度の目標値
};

#endif