//=============================================================================
//
// sceneヘッダ [scene.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _SCENE_H_
#define _SCENE_H_

//インクルード
#include "main.h"

//***********************************
//マクロ定義
//***********************************
#define MAX_SCENE 1280   // ポリゴンの最大数
#define PRIORITY_NUM OBJTYPE_MAX
//=============================
//クラス定義
//=============================

//シーンクラス
class CScene
{
public:
	// オブジェクトタイプ列挙
	// 描画順
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,          // 背景
		OBJTYPE_EFFECT,      // エフェクト
		OBJTYPE_PLAYER,      // プレイヤー
		OBJTYPE_DESTINATION, // 目標
		OBJTYPE_PARTICLE,    // パーティクル
		OBJTYPE_CHECKPOINT,  // チェックポイント
		OBJTYPE_MAP,         // マップ
		OBJTYPE_ACCELFLOOR,  // 加速床
		OBJTYPE_ITEM,        // アイテム
		OBJTYPE_MOTION,      // モーション
		OBJTYPE_COLLISION,   // コリジョン
		OBJTYPE_UI,          // UI
		OBJTYPE_MAX,         // タイプの最大数
	}OBJTYPE;

	// メンバ関数
	CScene(int nPriority = OBJTYPE_NONE);
	virtual ~CScene();
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumAll(void) { return m_nIDAll; }

	void SetPriority(const int nPriority);
	OBJTYPE GetType(void) { return m_type; }
	int GetID(void) { return m_nNumID; } // 配列番号の取得

	static CScene*GetTop(int nIndex) { return m_pTop[nIndex]; }
	static CScene*GetCur(int nIndex) { return m_pCur[nIndex]; }
	CScene*GetNext(void) { return m_pNext; }
	CScene*GetPrev(void) { return m_pPrev; }

	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

protected:
	void Release(void);
private:
	// メンバ変数
	void ReConnection(void);
	//static CScene *m_apScene[MAX_SCENE]; // ポリゴン管理用
	static int m_nIDAll;                   // 生成したポリゴン数
	int m_nNumID;                          // 生成したポリゴンの配列
	OBJTYPE m_type;                        // オブジェクトタイプ
	int m_nPriority;                       // プライオリティ
	bool m_bReleasFlag;                    // 削除フラグ

	// リスト構造
	static CScene * m_pTop[PRIORITY_NUM];  // 先頭
	static CScene * m_pCur[PRIORITY_NUM];  // 最後
	CScene * m_pNext;                      // 次 
	CScene * m_pPrev;                      // 前
};

#endif