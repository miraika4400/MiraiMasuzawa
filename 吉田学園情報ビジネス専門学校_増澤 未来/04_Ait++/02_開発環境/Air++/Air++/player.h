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
#include "character.h"

//*****************************
// マクロ定義
//*****************************

//*****************************
// 前方宣言
//*****************************

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CPlayer : public CCharacter
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	//メンバ関数
	//==========================
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum); // クラス生成
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// プレイヤー番号の取得処理
	int GetPlayerNum(void) { return m_nPlayerNum; }
private:
	void Move(void);    // 移動処理
	void UseItem(void); // アイテム使用の処理

	//==========================
	//メンバ変数
	//==========================
	static Model m_model;   // モデル構造体
	static Shader m_shader; // シェーダー構造体
	int m_nPlayerNum;       // プレイヤーID
};
#endif