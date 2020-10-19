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
#define BULLET_SPEED_ENEMY 4     // 速度(エネミーの弾)
#define BULLET_SIZE 15    // サイズ

//*****************************
// クラス定義
//*****************************

// 弾クラス
class CBullet : public CScene3d
{
public:
	typedef enum
	{
		BULLETUSER_PLAYER,
		BULLETUSER_ENEMY,
		BULLETUSER_MAX,
	}BULLETUSER;

	//メンバ関数
	CBullet();
	~CBullet();
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const  BULLETUSER user, const D3DCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static std::list<CBullet*>*GetBulletlist(void) { return &m_bulletList; }
	void SetUser(BULLETUSER user) { m_user = user; }
	BULLETUSER GetUser(void) { return m_user; }
protected:
	void CollisionBullet(BULLETUSER user);
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
private:
	// メンバ変数
	static std::list<CBullet*> m_bulletList;  // リスト
	D3DXVECTOR3 m_move; // 移動量
	int m_nLife;        // 寿命
	BULLETUSER m_user; // バレットユーザー
};

#endif