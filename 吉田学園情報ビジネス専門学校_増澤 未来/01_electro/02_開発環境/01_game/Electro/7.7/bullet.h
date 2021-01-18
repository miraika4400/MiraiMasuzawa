//=============================================================================
//
// bulletヘッダ [bullet.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// マクロ定義
//*****************************
#define BULLET_SPEED_PLAYER 8    // 速度(プレイヤーの弾)
#define BULLET_SPEED_ENEMY  4    // 速度(エネミーの弾)
#define BULLET_SIZE         15   // サイズ

//*****************************
// クラス定義
//*****************************

// 弾クラス
class CBullet : public CScene3d
{
public:
	//============
	// 列挙定義
	//============
	// 持ち主のタイプ
	typedef enum
	{
		BULLETUSER_PLAYER, // プレイヤー
		BULLETUSER_ENEMY,  // エネミー
		BULLETUSER_MAX,	   // タイプの最大数
	}BULLETUSER;

	//============
	// メンバ関数
	//============
	CBullet();
	~CBullet();
	// static
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const  BULLETUSER user, const D3DCOLOR col); // クラス生成
	static HRESULT Load(void);                                                                                                          // テクスチャロード
	static void Unload(void);                                                                                                           // テクスチャアンロード
	static std::list<CBullet*>*GetBulletlist(void) { return &m_bulletList; }                                                            // リストの取得

	HRESULT Init(void);// 初期化
	void Uninit(void); // 終了
	void Update(void); // 更新
	void Draw(void);   // 描画
	
	void SetUser(BULLETUSER user) { m_user = user; } // 持ち主のタイプの設定
	BULLETUSER GetUser(void) { return m_user; }      // 持ち主のタイプの取得
protected:
	void CollisionBullet(BULLETUSER user); // 当たり判定の管理
	
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
private:
	// メンバ変数
	static std::list<CBullet*> m_bulletList;  // リスト
	D3DXVECTOR3 m_move;                       // 移動量
	int         m_nLife;                      // 寿命
	BULLETUSER  m_user;                       // バレットユーザー
};

#endif