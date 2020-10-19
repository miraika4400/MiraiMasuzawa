////////////////////////////////////////////////////
//
//    bombクラスの処理[bomb.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "bomb.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "boss.h"
#include "particle.h"
#include "bullet.h"
#include "score.h"

//*****************************
// マクロ定義
//*****************************
#define BOMB_TEXTURE_PATH "./data/Textures/bomb000.png" //テクスチャのパス
#define BOMB_ANIM_SPEED 4      // アニメーション速度
#define BOMB_MAX_ANIMATION_X 1 // アニメーション数 横
#define BOMB_MAX_ANIMATION_Y 1 // アニメーション数 縦
#define BOMB_RATE_SIZE 0.1f;

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9 CBomb::m_pTexture = NULL; // テクスチャポインタ

//******************************
// コンストラクタ
//******************************
CBomb::CBomb():CScene3d(OBJTYPE_BOMB)
{
	m_bHitBoss = false;
	m_fMaxsize = 0.0f;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//******************************
// デストラクタ
//******************************
CBomb::~CBomb()
{
}

//******************************
// クリエイト
//******************************
CBomb * CBomb::Create(const D3DXVECTOR3 pos, const float fMaxsize, const D3DCOLOR col)
{
	// メモリの確保
	CBomb *pBomb;
	pBomb = new CBomb;
	// 初期化
	pBomb->Init();

	// 各値の代入・セット
	pBomb->SetPos(pos);                // 座標
	pBomb->SetColor(col);              // 色
	pBomb->SetSize(D3DXVECTOR3(1.0f,1.0f,1.0f));              // サイズ
	pBomb->SetObjType(OBJTYPE_BOMB);   // オブジェクトタイプ
	pBomb->m_fMaxsize = fMaxsize;

	// SEの再生
	CManager::GetSound()->Play(CSound::LABEL_SE_BOMB);

	return pBomb;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CBomb::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BOMB_TEXTURE_PATH, &m_pTexture);


	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CBomb::Unload(void)
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
HRESULT CBomb::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_pTexture);

	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / BOMB_MAX_ANIMATION_X;
	float fv = 1.0f / BOMB_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CBomb::Uninit(void)
{

	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CBomb::Update(void)
{
	// 自身の座標の取得
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 size = GetSize();
	size.x += ((m_fMaxsize + 3.0f) - size.x)*BOMB_RATE_SIZE;
	size.y += ((m_fMaxsize + 3.0f) - size.y)*BOMB_RATE_SIZE;
	SetSize(size);

	// 当たり判定

	// エネミーリストの取得
	std::list<CEnemy*>::iterator enemyIteretor;
	for (enemyIteretor = CEnemy::GetEnemylist()->begin(); enemyIteretor != CEnemy::GetEnemylist()->end(); enemyIteretor++)
	{

		D3DXVECTOR3 size = GetSize()/2;  // 弾のサイズ

		D3DXVECTOR3 enemyPos = (*enemyIteretor)->GetPos();   // 敵の座標
		D3DXVECTOR3 enemySize = (*enemyIteretor)->GetSize(); // 敵のサイズ

		if (pos.x - size.x <= enemyPos.x + enemySize.x &&
			pos.x + size.x >= enemyPos.x - enemySize.x &&
			pos.y - size.y <= enemyPos.y + enemySize.y &&
			pos.y + size.y >= enemyPos.y - enemySize.y)
		{
			// 爆発の生成
			//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// エネミー爆発SE
			CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

			// エネミーを消す
			(*enemyIteretor)->HitAttack(5);
			// スコア加算
			CScore::AddScore(50);
			break;

		}
	}

	// バレットリストの取得
	std::list<CBullet*>::iterator bulletIteretor;
	for (bulletIteretor = CBullet::GetBulletlist()->begin(); bulletIteretor != CBullet::GetBulletlist()->end(); )
	{
		if ((*bulletIteretor)->GetUser() == CBullet::BULLETUSER_ENEMY)
		{
			D3DXVECTOR3 size = GetSize() / 2;  // 弾のサイズ

			D3DXVECTOR3 bulletPos = (*bulletIteretor)->GetPos();   // 敵の座標
			D3DXVECTOR3 bulletSize = (*bulletIteretor)->GetSize(); // 敵のサイズ

			if (pos.x - size.x <= bulletPos.x + bulletSize.x &&
				pos.x + size.x >= bulletPos.x - bulletSize.x &&
				pos.y - size.y <= bulletPos.y + bulletSize.y &&
				pos.y + size.y >= bulletPos.y - bulletSize.y)
			{
				// 爆発の生成
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				
				CBullet*pBullet = *bulletIteretor;
				bulletIteretor = CBullet::GetBulletlist()->erase(bulletIteretor);
				//	弾を消す
				pBullet->Uninit();
				//// リストの初期化
				//bulletIteretor = CBullet::GetBulletlist()->begin();

				// スコア加算
				CScore::AddScore(10);
			}
			else
			{
				bulletIteretor++;
			}
		}
		else
		{
			bulletIteretor++;
		}
	}

	if (CBoss::IsAlive())
	{
		if (!m_bHitBoss)
		{
			// ボスへの当たり判定
			D3DXVECTOR3 size = GetSize();  // 弾のサイズ

			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();   // 敵の座標
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize(); // 敵のサイズ

			if (pos.x - size.x/2 <= bossPos.x + bossSize.x / 2 &&
				pos.x + size.x/2 >= bossPos.x - bossSize.x / 2 &&
				pos.y - size.y/2 <= bossPos.y + bossSize.y / 2 &&
				pos.y + size.y/2 >= bossPos.y - bossSize.y / 2)
			{
				// 爆発の生成
				//CExplosion::Create(enemyPos, D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				// エネミー爆発SE
				CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);

				// エネミーを消す
				CGame::GetBoss()->HitAttack(5);
				// スコア加算
				CScore::AddScore(50);
				m_bHitBoss = true;
			}
		}
	}
	int nRnd = rand() % 10 + 5;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// ランダムな距離
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// ランダムな角度
		int nRandAngle = rand() % 360;
		
		// ランダムな座標
		D3DXVECTOR3 randPos;
		randPos.x = pos.x + cosf(D3DXToRadian(nRandAngle))*(size.x/2 + fRandDistance);
		randPos.y = pos.y + sinf(D3DXToRadian(nRandAngle))*(size.y/2 + fRandDistance);
		randPos.z = 0.0f;
	
		if (m_fMaxsize >= 500)
		{// アイテムのボム

			// ランダムなサイズ
			int nRandSize = rand() % 30 + 20;
			// パーティクルの生成
			CParticle::Create(randPos, 
				D3DXVECTOR3((float)(rand() % 3 - 1),
				(float)(rand() % 3 - 1), 0.0f), 
				D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
				rand() % 10 + 10,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f));
		}
		else
		{// マナサークルのボム

			// ランダムなサイズ
			int nRandSize = rand() % 20 + 15;
			// パーティクルの生成
			CParticle::Create(randPos,
				D3DXVECTOR3((float)(rand() % 3 - 1),
				(float)(rand() % 3 - 1), 0.0f),
				D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
				rand() % 10 + 10,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f, 1.0f));
		}
	}
	
	// 最大サイズに到達したら消す
	if (size.x > m_fMaxsize)
	{
		Uninit();
		return;
	}


	//// アニメーションカウントを進める
	//m_nCntAnim++;

	//if (m_nCntAnim % BOMB_ANIM_SPEED == 0)
	//{
	//	// アニメーションX軸の加算
	//	m_nAnimX++;

	//	if (m_nAnimX >= BOMB_MAX_ANIMATION_X)
	//	{
	//		// 消す
	//		Uninit();
	//		return;
	//	}

	//	// UV座標の設定
	//	D3DXVECTOR2 uv[NUM_VERTEX];
	//	float fu = 1.0f / BOMB_MAX_ANIMATION_X;
	//	float fv = 1.0f / BOMB_MAX_ANIMATION_Y;

	//	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	//	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	//	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	//	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	//	// UV座標セット
	//	SetTextureUV(uv);
	//}

}

//******************************
// 描画処理
//******************************
void CBomb::Draw(void)
{
//	CScene3d::Draw();
}

