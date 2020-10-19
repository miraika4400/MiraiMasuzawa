////////////////////////////////////////////////////
//
//    bulletクラスの処理[bullet.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "bullet.h"
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
#include "boss.h"

//*****************************
// マクロ定義
//*****************************
#define BULLET_TEXTURE_PATH "./data/Textures/bullet000.png" //テクスチャのパス

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9  CBullet::m_pTexture = NULL; // テクスチャポインタ
std::list<CBullet*> CBullet::m_bulletList;      // リスト

//******************************
// コンストラクタ
//******************************
CBullet::CBullet():CScene3d(OBJTYPE_BULLET)
{
	// 変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_user = BULLETUSER_PLAYER;
}

//******************************
// デストラクタ
//******************************
CBullet::~CBullet()
{
}

//******************************
// クリエイト
//******************************
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const  BULLETUSER user,const D3DCOLOR col)
{
	// メモリの確保
	CBullet *pBullet;
	pBullet = new CBullet;
	// 初期化
	pBullet->Init();

	// 各値の代入・セット
	pBullet->SetPos(pos);                // 座標
	pBullet->SetColor(col);              // 色
	pBullet->SetObjType(OBJTYPE_BULLET); // オブジェクトタイプ
	pBullet->m_move = move;              // 移動量
	pBullet->m_nLife = nLife;            // 寿命
	pBullet->m_user = user;        // バレットユーザー

	// リストに追加
	m_bulletList.push_back(pBullet);
	return pBullet;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CBullet::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CBullet::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	
	// テクスチャ割り当て
	BindTexture(m_pTexture);
	// サイズの設定
	SetSize(D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f));

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CBullet::Uninit(void)
{
	// リストから削除
	std::list<CBullet*>::iterator iteretor;
	for (iteretor = m_bulletList.begin(); iteretor != m_bulletList.end(); )
	{
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = m_bulletList.erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}

	}

	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = GetPos(); // 弾の座標

	// 位置更新
	pos += m_move;
	SetPos(pos);

	// エフェクト生成
	CEffect::Create(pos, GetSize(), GetColor(), m_pTexture);

	// 弾の寿命の処理
	m_nLife--;
	if (m_nLife < 0)
	{
		// 爆発生成
		CExplosion::Create(GetPos(), D3DXVECTOR3(40, 40, 0), GetColor());
		// 弾を消す
		Uninit();

		return;
	}

	// 当たり判定
	CollisionBullet(m_user);
}

//******************************
// 描画処理
//******************************
void CBullet::Draw(void)
{
	CScene3d::Draw();
}


//******************************
// 当たり判定
//******************************
void CBullet::CollisionBullet(BULLETUSER user)
{
	D3DXVECTOR3 pos = GetPos(); // 弾の座標

	// 当たり判定

	if (user == BULLETUSER_PLAYER)
	{ // プレイヤーの弾

		//エネミーへの当たり判定

	    // エネミーリストの取得
		std::list<CEnemy*>::iterator enemyIteretor;
		for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
		{
			D3DXVECTOR3 size = GetSize();  // 弾のサイズ

			D3DXVECTOR3 enemyPos = (*enemyIteretor)->GetPos();   // 敵の座標
			D3DXVECTOR3 enemySize = (*enemyIteretor)->GetSize(); // 敵のサイズ

			if (pos.x - size.x <= enemyPos.x + enemySize.x / 3 &&
				pos.x + size.x >= enemyPos.x - enemySize.x / 3 &&
				pos.y - size.y <= enemyPos.y + enemySize.y / 3 &&
				pos.y + size.y >= enemyPos.y - enemySize.y / 3)
			{
				// エネミー爆発SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
				
				// エネミーを消す
				(*enemyIteretor)->HitAttack(1);

				// 弾を消す
				Uninit();
				return;
			}
		}

		if (CBoss::IsAlive())
		{
			// ボスへの当たり判定
			D3DXVECTOR3 size = GetSize();  // 弾のサイズ

			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();   // 敵の座標
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize(); // 敵のサイズ

			if (pos.x - size.x <= bossPos.x + bossSize.x / 3 &&
				pos.x + size.x >= bossPos.x - bossSize.x / 3 &&
				pos.y - size.y <= bossPos.y + bossSize.y / 3 &&
				pos.y + size.y >= bossPos.y - bossSize.y / 3)
			{
				// エネミー爆発SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

				// エネミーを消す
				CGame::GetBoss()->HitAttack(1);

				// 弾を消す
				Uninit();
				return;
			}

		}
	}
	else if (user == BULLETUSER_ENEMY)
	{// 敵の弾
		if (CPlayer::IsAlive())
		{
			CPlayer *pPlayer = CGame::GetPlayer();

			D3DXVECTOR3 size = GetSize();  // 弾のサイズ

			D3DXVECTOR3 playerPos = pPlayer->GetPos();   // Playerの座標
			D3DXVECTOR3 playerSize = pPlayer->GetSize(); // Playerのサイズ

			if (pos.x - size.x / 2 <= playerPos.x + 20 &&
				pos.x + size.x / 2 >= playerPos.x - 20 &&
				pos.y - size.y / 2 <= playerPos.y + 20 &&
				pos.y + size.y / 2 >= playerPos.y - 20)
			{
				if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
				{
					// 爆発SE
					CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
					// ライフを減らす
					pPlayer->HitAttack(1);
					//pPlayer->SetState(CPlayer::STATE_DAMAGE);
				}
				// 弾を消す
				Uninit();
				return;
			}
		}
	}
}
