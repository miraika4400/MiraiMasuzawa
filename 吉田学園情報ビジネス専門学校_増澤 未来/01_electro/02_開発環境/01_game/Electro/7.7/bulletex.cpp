////////////////////////////////////////////////////
//
//    bulletexクラスの処理[bulletex.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "bulletex.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "effect.h"
#include "game.h"

//******************************
 // コンストラクタ
 //******************************
CBulletEx::CBulletEx()
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_fRotAngle = 0.0f;
	m_fDistance = 10.0f;
	m_nTargetNum = 0;
}

//******************************
// デストラクタ
//******************************
CBulletEx::~CBulletEx()
{
}

<<<<<<< HEAD
//******************************
// クリエイト
//******************************
=======
>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
CBulletEx * CBulletEx::Create_Rotation(const D3DXVECTOR3 pos, const D3DXVECTOR3 posParent, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col)
{
		// メモリの確保
		CBulletEx *pBulletEx;
		pBulletEx = new CBulletEx;
		// 初期化
		pBulletEx->Init();
	
		// 各値の代入・セット
		pBulletEx->SetPos(pos);                        // 座標
		pBulletEx->SetColor(col);                      // 色
		pBulletEx->SetObjType(OBJTYPE_BULLET);         // オブジェクトタイプ
		pBulletEx->m_posParent = posParent;            // 親座標
		pBulletEx->m_nLife = nLife;                    // 寿命
		pBulletEx->SetUser(user);                      // バレットユーザー
		pBulletEx->m_bulletType = BULLETTYPE_ROTATION; // 弾の種類
		pBulletEx->m_move = move;                      // 移動量
		// 親座標との距離の計算
		pBulletEx->m_fDistance = sqrtf(powf(pos.x - posParent.x, 2) + powf(pos.y - posParent.y, 2));

		// リストに追加
		GetBulletlist()->push_back(pBulletEx);
		return pBulletEx;
}

<<<<<<< HEAD
=======
//******************************
// クリエイト
//******************************
//CBulletEx * CBulletEx::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const BULLETUSER user, const D3DCOLOR col, const BULLETTYPE type, const float rotAngle)
//{
//	// メモリの確保
//	CBulletEx *pBulletEx;
//	pBulletEx = new CBulletEx;
//	// 初期化
//	pBulletEx->Init();
//
//	// 各値の代入・セット
//	pBulletEx->SetPos(pos);                // 座標
//	pBulletEx->SetColor(col);              // 色
//	pBulletEx->SetObjType(OBJTYPE_BULLET); // オブジェクトタイプ
//	pBulletEx->m_posParent = pos;          // 親座標
//	pBulletEx->m_move = move;              // 移動量
//	pBulletEx->m_nLife = nLife;            // 寿命
//	pBulletEx->m_user = user;              // バレットユーザー
//	pBulletEx->m_bulletType = type;        // 弾の種類
//	pBulletEx->m_fRotAngle = rotAngle;
//
//
//	return pBulletEx;
//}

>>>>>>> 1f4259f10d71f719b19dc40ae4c297906db09178
CBulletEx * CBulletEx::Create_Rotation_Spread(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col, const float rotAngle)
{
	// メモリの確保
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// 初期化
	pBulletEx->Init();

	// 各値の代入・セット
	pBulletEx->SetPos(pos);                // 座標
	pBulletEx->SetColor(col);              // 色
	pBulletEx->SetObjType(OBJTYPE_BULLET); // オブジェクトタイプ
	pBulletEx->SetSize(size);              // サイズのセット
	pBulletEx->m_posParent = pos;          // 親座標
	pBulletEx->m_nLife = nLife;            // 寿命
	pBulletEx->SetUser(user);              // バレットユーザー
	pBulletEx->m_bulletType = BULLETTYPE_ROTATION_SPREAD;        // 弾の種類
	pBulletEx->m_fRotAngle = rotAngle;

	// リストに追加
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

CBulletEx * CBulletEx::Create_Target(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col)
{
	// メモリの確保
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// 初期化
	pBulletEx->Init();

	// 各値の代入・セット
	pBulletEx->SetPos(pos);                // 座標
	pBulletEx->SetColor(col);              // 色
	pBulletEx->SetObjType(OBJTYPE_BULLET); // オブジェクトタイプ
	pBulletEx->SetSize(size);              // サイズのセット
	pBulletEx->m_posParent = pos;          // 親座標
	pBulletEx->m_move = move;              // 移動量
	pBulletEx->m_nLife = nLife;            // 寿命
	pBulletEx->SetUser(user);              // バレットユーザー
	pBulletEx->m_bulletType = BULLETTYPE_TARGET;        // 弾の種類

														// リストに追加
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

CBulletEx * CBulletEx::Create_Homing(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const BULLETUSER user, const D3DCOLOR col, const int nTargetNum)
{
	// メモリの確保
	CBulletEx *pBulletEx;
	pBulletEx = new CBulletEx;
	// 初期化
	pBulletEx->Init();

	// 各値の代入・セット
	pBulletEx->SetPos(pos);                // 座標
	pBulletEx->SetColor(col);              // 色
	pBulletEx->SetObjType(OBJTYPE_BULLET); // オブジェクトタイプ
	pBulletEx->SetSize(size);              // サイズのセット
	pBulletEx->m_posParent = pos;          // 親座標
	pBulletEx->m_move = move;              // 移動量
	pBulletEx->m_nLife = nLife;            // 寿命
 	pBulletEx->SetUser(user);              // バレットユーザー
	pBulletEx->m_bulletType = BULLETTYPE_HOMING;        // 弾の種類
	pBulletEx->m_nTargetNum = nTargetNum;

	// リストに追加
	GetBulletlist()->push_back(pBulletEx);
	return pBulletEx;
}

//******************************
// 初期化処理
//******************************
HRESULT CBulletEx::Init(void)
{
	CBullet::Init();

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CBulletEx::Uninit(void)
{
	// リストから削除
	std::list<CBullet*>::iterator iteretor;
	for (iteretor = GetBulletlist()->begin(); iteretor != GetBulletlist()->end(); )
	{
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = GetBulletlist()->erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}

	}

	CBullet::Uninit();
}

//******************************
// 更新処理
//******************************
void CBulletEx::Update(void)
{
	// エフェクト生成
	CEffect::Create(GetPos(), GetSize(), GetColor(),m_pTexture);

	// 弾座標の設定
	D3DXVECTOR3 pos = GetPos();

	switch (m_bulletType)
	{
	case BULLETTYPE_ROTATION:

		// 親座標を中心に半径"m_fDistance"の角度"m_fRotAngle"
		m_fRotAngle = atan2f(pos.y - m_posParent.y, pos.x - m_posParent.x);

		// 角度を足す
		m_fRotAngle += 0.02f;
		pos.x = m_posParent.x + cosf(m_fRotAngle) * m_fDistance;
		pos.y = m_posParent.y + sinf(m_fRotAngle) * m_fDistance;
		pos.z = 0.0f;

		//
		m_posParent += m_move; 
		pos += m_move;
		break;

	case BULLETTYPE_ROTATION_SPREAD:

		// 親座標を中心に半径"m_fDistance"の角度"m_fRotAngle"
		pos.x = m_posParent.x + cosf(m_fRotAngle) * m_fDistance;
		pos.y = m_posParent.y + sinf(m_fRotAngle) * m_fDistance;
		pos.z = 0.0f;

		m_fRotAngle += 0.02f;
		// 半径を広げる
		m_fDistance += 2;
		break;

	case BULLETTYPE_HOMING:

		if (GetUser() == BULLETUSER_PLAYER)
		{
			// エネミーリストの取得
			std::list<CEnemy*>::iterator iteretor;
			for (iteretor = CEnemy::GetEnemylist()->begin(); iteretor != CEnemy::GetEnemylist()->end(); iteretor++)
			{
				if (m_nTargetNum == (*iteretor)->GetID())
				{
					D3DXVECTOR3 targetPos = (*iteretor)->GetPos();
					float fAngle = atan2f(targetPos.y - GetPos().y, targetPos.x - GetPos().x);

					m_move = D3DXVECTOR3(cosf(fAngle)*BULLET_SPEED_PLAYER, sinf(fAngle)*BULLET_SPEED_PLAYER, 0.0f);
					break;
				}
			}
		}
		pos += m_move;
		break;

	default:
		// 座標に移動量を足す
		pos += m_move;

		break;
	}

	SetPos(pos);

	// 弾の寿命の処理
	m_nLife--;

	if (m_nLife < 0)
	{
		if (m_bulletType == BULLETTYPE_TARGET)
		{
			if (GetUser() == BULLETUSER_ENEMY)
			{
				// 一定フレーム停止
				m_move = D3DXVECTOR3(0, 0, 0);

				// 自機狙いの弾を出す
				if (m_nLife < -50)
				{
					// 敵からプレイヤーの角度
					float fAngle = 0.0f;
					// プレイヤー情報取得
					CPlayer*pPlayer = CGame::GetPlayer();

					if (CPlayer::IsAlive())
					{
						// 敵からプレイヤーの角度の取得
						fAngle = atan2f(pPlayer->GetPos().y - GetPos().y, pPlayer->GetPos().x - GetPos().x);
					}

					// 弾の生成
					CBullet::Create(GetPos(),
						D3DXVECTOR3(cosf(fAngle)*BULLET_SPEED_ENEMY, sinf(fAngle)*BULLET_SPEED_ENEMY, 0.0f),
						150,
						CBullet::BULLETUSER_ENEMY,
						GetColor());

					// 弾を消す
					Uninit();
				}
			}
		}
		else
		{
			// 爆発生成
			CExplosion::Create(GetPos(), D3DXVECTOR3(40, 40, 0), GetColor());
			// 弾を消す
			Uninit();
		}
		
		

		return;
	}

	// 当たり判定
	CollisionBullet(GetUser());
}

//******************************
// 描画処理
//******************************
void CBulletEx::Draw(void)
{
	CBullet::Draw();
}