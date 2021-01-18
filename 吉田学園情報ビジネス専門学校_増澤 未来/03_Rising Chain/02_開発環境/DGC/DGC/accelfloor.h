//=============================================================================
//
// Accelfloorヘッダ [accelfloor.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ACCELFLOOR_H_
#define _ACCELFLOOR_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model.h"
#include "game.h"

//*****************************
// マクロ定義
//*****************************
#define HOVER_RING 200 // リングをどれだけ地面から浮かすか

//*****************************
// 前方宣言
//*****************************
class CCollision;
class CScene3d;

//*****************************
// クラス定義
//*****************************

// 加速床クラス
class CAccelFloor : public CModel
{
public:
	//==========================
	// 列挙
	//==========================


	//==========================
	//メンバ関数
	//==========================
	CAccelFloor();
	~CAccelFloor();
	static CAccelFloor *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot); // クラス生成
	static HRESULT Load(void);                                                // ロード
	static void Unload(void);                                                 // アンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// 調整フラグの取得、セット
	bool GetAdjustFlag(void) { return m_bAdjust; }
	void SetAdjustFlag(bool bAdjust) { m_bAdjust = bAdjust; }

private:
	void CollisionPlayer(void); // プレイヤーとの当たり判定
	void AdjustPos(void);       // 生成時に位置調整

	//==========================
	//メンバ変数
	//==========================
	static Model m_model;      // モデル構造体
	static LPDIRECT3DTEXTURE9 m_pTexture; // 床のテクスチャ
	CCollision*m_pCollision;       // 当たり判定
	CScene3d*m_pScene3d;           // 床のパネルポリゴン
	float m_fScroll;               // 床のテクスチャずらす用
	bool m_bAdjust;                // 位置調整フラグ
	bool m_bHit[MAX_PLAYER_NUM];   // ヒット時のフラグ
	int m_nCntHit[MAX_PLAYER_NUM]; // ヒットフラグ初期化カウント
};
#endif