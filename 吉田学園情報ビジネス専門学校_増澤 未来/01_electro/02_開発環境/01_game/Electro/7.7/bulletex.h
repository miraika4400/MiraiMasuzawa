//=============================================================================
//
// bulletexヘッダ [bulletex.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BULLETEX_H_
#define _BULLETEX_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "bullet.h"

//*****************************
// 前方宣言
//*****************************
class CScene2d;

//*****************************
// クラス定義
//*****************************

// まっすぐじゃない弾クラス
class CBulletEx : public CBullet
{
public:
	//============
	// 列挙定義
	//============
	// 弾の種類
	typedef enum
	{
		BULLETTYPE_ROTATION,       // 撃った位置を中心に回る
		BULLETTYPE_ROTATION_SPREAD,// 撃った位置を中心に回って半径が広がってく
		BULLETTYPE_TARGET,         // まっすぐ飛んだ後に自機狙いの弾を出す
		BULLETTYPE_HOMING,         // ホーミング
		BULLETTYPE_MAX
	}BULLETTYPE;

	//============
	// メンバ関数
	//============
	CBulletEx();
	~CBulletEx();

	// 各タイプの弾の生成

	static CBulletEx *Create_Rotation(const D3DXVECTOR3 pos, const D3DXVECTOR3 posParent, const D3DXVECTOR3 move, const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col);

	static CBulletEx *Create_Rotation_Spread(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife,
		const BULLETUSER user, const D3DCOLOR col, const float rotAngle = 0.0f);

	static CBulletEx *Create_Target(const D3DXVECTOR3 pos, const D3DXVECTOR3 move,const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col);

	static CBulletEx *Create_Homing(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col,const int nTargetNum);
	

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	// メンバ変数
	D3DXVECTOR3 m_posParent;  // 親座標
	D3DXVECTOR3 m_move;       // 移動量
	int         m_nLife;      // 寿命
	BULLETTYPE  m_bulletType; // 弾の種類
	float       m_fRotAngle;  // 回転角度
	float       m_fDistance;  // 親座標との距離
	int         m_nTargetNum; // ターゲットの識別用
};

#endif