//=============================================================================
//
// characterヘッダ [character.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model_shader.h"
#include "item.h"

//*****************************
// マクロ定義
//*****************************
#define CHARACTER_RADIUS 100.0f // プレイヤー半径(当たり判定)

//*****************************
// 前方宣言
//*****************************
class CCollision;

//*****************************
// クラス定義
//*****************************

// キャラクタークラス
class CCharacter : public CModelShader
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	// 構造体定義
	//==========================
	// 順位付けに利用する構造体
	typedef struct
	{
		int nCheck; // チェックポイント数
		int nLap;   // ラップ数
		int nRank;  // 順位
	}RankData;

	//==========================
	// メンバ関数
	//==========================
	CCharacter(int nPliority = OBJTYPE_NONE);
	~CCharacter();

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// セッター・ゲッター

	// 移動量
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	// 移動量の目標値
	void SetMoveDist(D3DXVECTOR3 moveDist) { m_moveDist = moveDist; }
	D3DXVECTOR3 GetMoveDist(void) { return m_moveDist; }
	// 重力フラグ
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	bool GetActiveGravity(void) { return m_bGravity; }
	// 順位データ
	void SetRankData(RankData rankData) { m_rankData = rankData; }
	RankData GetRankData(void) { return m_rankData; }
	// 重力フラグ
	void SetGoalFlag(bool bBool) { m_bGoal = bBool; }
	bool GetGoalFlag(void) { return m_bGoal; }
	// アイテム情報
	void SetItem(CItem::ITEM_TYPE item) { m_item = item; }
	CItem::ITEM_TYPE GetItem(void) { return m_item; }
	// 加速フラグ
	void SetActiveAcceleration(bool bBool);
	bool GetActiveAcceleration(void) { return m_bAcceleration; }
	// 移動速度
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	float GetSpeed(void) { return m_fSpeed; }
	// スタンフラグ
	void SetStan(bool bBool);
	bool GetStan(void) { return m_bStan; }
	
	// プレイヤーフラグ取得処理
	bool GetIsPlayer(void) { return m_bIsPlayer; }
	// コリジョン取得処理
	CCollision* GetCollision(void) { return m_pCollision; }

private:
	void Gravity(void);            // 重力処理
	void Direction(void);          // 向きの管理処理
	virtual void CollisionCharacter(void); // キャラクター同士の当たり判定
	void SpeedManager(void);       // スピード管理処理

	//==========================
	// メンバ変数
	//==========================
	bool         m_bIsPlayer;  // プレイヤーかNPCか判定用
	D3DXVECTOR3  m_move;       // 移動量
	D3DXVECTOR3  m_moveDist;   // 移動量の目標値
	D3DXVECTOR3  m_gravityVec; // 重力
	bool         m_bGravity;   // 重力フラグ
	CCollision * m_pCollision; // コリジョン
	RankData     m_rankData;   // 順位付け用データ
	bool         m_bGoal;      // ゴールしているかの判定用
	D3DXVECTOR3  m_impact;     // 衝突したときの移動量
	CItem::ITEM_TYPE m_item;   // 所持しているアイテム情報
	bool         m_bAcceleration; // 加速フラグ
	float        m_fSpeed;     // 移動速度
	int          m_nCntAccleration; // 加速時カウント
	bool         m_bStan;      // スタン判定
	int          m_nCntStan;   // スタン中のカウント
	int          m_nCntGravity;// 重力フラグが経っている間カウントする
};
#endif