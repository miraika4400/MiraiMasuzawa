//=============================================================================
//
// manacircreヘッダ [manacircre.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _MANACIRCRE_H_
#define _MANACIRCRE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// マクロ定義
//*****************************
#define CIRCLE_MAX 6  // サークルの最大数 

//*****************************
// クラス定義
//*****************************

// サークルクラス
class CManaCircle : public CScene3d
{
public:
	//============
	// メンバ関数
	//============
	CManaCircle();
	~CManaCircle();
	// static
	static CManaCircle *Create(const D3DXVECTOR3 pos);                          // クラス生成
	static HRESULT Load(void);                                                  // テクスチャロード
	static void Unload(void);                                                   // テクスチャアンロード
	static void SpawnManager(void);                                             // サークルのわきの管理
	static void SetCircleNum(int nNum) { m_nNumCircle = nNum; }                 // サークル数のセット
	static void SetCircleMax(int nNum) { m_nMaxCircle = nNum; }                 // サークルの最大数のセット
	static CManaCircle *GetCircle(int nIndex) { return m_apCircle[nIndex]; }    // 生成してるサークルポインタの取得
	
	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画
	
private:
	static D3DXVECTOR3 GetRandCirclePos(void); // サークル生成用のランダムな座標の取得
	void AnimationManager(void);               // テクスチャアニメーションの管理
	void CollisionCircle(void);                // 当たり判定の管理
	void LostCircle(void);                     // サークルの削除

	//============
	// メンバ関数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;       // テクスチャ
	static CManaCircle *m_apCircle[CIRCLE_MAX]; // 生成しているサークルポインタの格納用
	static int m_nNumCircle;                    // 出現しているサークルの数
	static int m_nMaxCircle;                    // 出現するサークルの最大数
	int  m_nCntRecovery; // サークルに入っている間のカウント
	int  m_nIndex;       // 配列番号
	bool bInCircle;      // サークルに入っているか
	int  m_nCntAnim;     // アニメーションカウント
	int  m_nAnimX;       // アニメーションX軸
	int  m_nAnimY;       // アニメーションY軸
};

#endif