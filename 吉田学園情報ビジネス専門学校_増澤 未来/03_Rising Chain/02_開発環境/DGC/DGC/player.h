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
#include "model_hierarchy.h"
#include "game.h"
//*****************************
// マクロ定義
//*****************************
#define MAX_EVOLUTION 4        // 進化の段階数

//*****************************
// 前方宣言
//*****************************
class CCollision;
class CDestination;
class CGauge;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CPlayer : public CModelHierarchy
{
public:

	//メンバ関数
	CPlayer();
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos,int nPlayerNum);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void Goal(void); // ゴール処理
	bool GetGoalFlag(void) { return m_bGoal; } // ゴールフラグの取得
	
	CCollision*GetCollision(void) { return m_pCollision; }
	CDestination*GetDestination(void) { return m_pDest; }
	// 移動量の取得・セット
	/*D3DXVECTOR3  GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }*/
	// 重力フラグの取得・セット
	bool GetActiveGravity(void) { return m_bGravity; }
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	// チェックポイント数の取得・セット・加算
	int GetCheckNum(void) { return m_nNumCheckpoint; }
	void SetCheckNum(int nNum) { m_nNumCheckpoint = nNum; }
	void AddCheckNum(void) { m_nNumCheckpoint++; }
	// アイテム数の取得・セット・加算
	int  GetItemNum(void) { return m_nCollectItem; }
	void SetItemNum(int nNum) { m_nCollectItem = nNum; }
	void AdtItemNum(void) { m_nCollectItem++; }
	// チェインポイント数の取得・セット・加算
	int  GetChainNum(void) { return m_nChain; }
	void SetChainNum(int nNum) { m_nChain = nNum; }
	void AddChainNum(void);// { m_nChain++; }
	// チェインポイント数の取得・セット・加算
	int GetMaxChain(void) { return m_nMaxChain; }
	void  SetMaxChain(int nMaxChain) { m_nMaxChain = nMaxChain; }
	// 移動フラグの取得・セット
	bool GetMoveFlag(void) { return m_bMove; }
	void SetMoveFlag(bool bMove) { m_bMove = bMove; }
	// ドリフトフラグの取得・セット
	bool GetDriftLeft(void) { return m_bDriftLeft; }
	void SetDriftLeft(bool bDrift) { m_bDriftLeft = bDrift; }
	bool GetDriftRight(void) { return m_bDriftRight; }
	void SetDriftRight(bool bDrift) { m_bDriftRight = bDrift; }
	// ヒット状態の取得・セット
	bool  GetHitFrag(void) { return m_bHit; }
	void SetHitFrag(bool bHit) { m_bHit = bHit; }
	// 最高速度の取得・セット・加算
	float  GetMaxSpeed(void) { return m_fMaxSpeed; }
	void SetMaxSpeed(float fSpeed) { m_fMaxSpeed = fSpeed; }
	// 加速度の取得・セット・加算
	float  GetMoveRate(void) { return m_fMoveRate; }
	void SetMoveRate(float fRate) { m_fMoveRate = fRate; }
	// 加速状態の取得・セット
	bool  GetAccelerationFrag(void) { return m_bAccelerationFlag; }
	void SetAccelerationFrag(bool bAccele); //{ m_bAccelerationFlag = bAccele; }
	// 順位の取得・セット
	int GetRank(void) { return m_nRank; }
	void SetRank(int nRank) { m_nRank = nRank; }
	// プレイヤー番号の取得
	int GetPlayerNum(void) { return m_nPlayerNum; }
	// アイテムにあたったときの処理
	void HitItem(bool bSafe);

private:
	void MoveControll(void);   // 移動
	void Gravity(void);        // 重力
	void Evolution(void);      // 進化
	void Drift(void);          // ドリフト
	void Acceleration(void);   // 加速処理
	void CollisionPlayer(void);// プレイヤー同士の当たり判定

	// メンバ変数
	static CModel::Model m_model[MAX_PLAYER_NUM][MAX_PARTS_NUM];    // モデル構造体
	static int m_nNumModel[MAX_PLAYER_NUM];                         // モデルの数
	static char m_acModelText[MAX_PLAYER_NUM][64];   // モデルのテキストデータ
	static int m_nNumEvolutionParts[MAX_EVOLUTION];  // 進化ごとのパーツ数
	static int m_nSpeedData[MAX_EVOLUTION];          // 進化ごとの最高速度
	static float m_fRateData[MAX_EVOLUTION];         // 進化ごとの加速度
	static int m_nEvoData[MAX_EVOLUTION];            // 各進化までの値
	//D3DXVECTOR3 m_move;        // 移動量
	int m_nPlayerNum;          // プレイヤー番号
	D3DXVECTOR3 m_gravityVec;  // 重力
	bool m_bGravity;           // 重力フラグ
	CCollision*m_pCollision;   // コリジョンクラスのポインタ
	int m_nNumCheckpoint;      // チェックポイント数
	bool m_bGoal;              // ゴールフラグ
	bool m_bMove;              // 移動できる状態化判定用
	int m_nChain;              // チェイン数
	int m_nMaxChain;           // 最大チェイン数
	int m_nCollectItem;        // 回収したアイテム数
	int m_nNumEvolution;       // 進化回数
	float m_fEvoGauge;         // 進化ゲージの値
	CDestination *m_pDest;     // 移動移動目標
	bool m_bDriftLeft;         // ドリフト左
	bool m_bDriftRight;        // ドリフト右
	bool m_bHit;               // ヒット状態フラグ
	int m_nCntHit;             // ヒット状態解除時のカウント
	float m_fMaxSpeed;         // 最大速度
	bool m_bAccelerationFlag;  // 加速フラグ
	int m_nCntAcceleration;    // 加速中のカウント
	float m_fAcceleration;     // 加速状態の値
	float m_fMoveRate;         // 慣性の値
	int m_nRank;               // 順位
	CGauge*m_pGauge;           // ゲージクラスポインタ
};

#endif