//===================================================
//
//    エネミークラスの処理[enemy.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "renderer.h"
#include "bullet.h"
#include "bulletex.h"
#include "player.h"
#include "explosion.h"
#include "mouse.h"
#include "game.h"
#include "stage.h"
#include "score.h"
#include "fade.h"
#include "enemy.h"
#include "sound.h"
#include "particle.h"

//*****************************
// マクロ定義
//*****************************
#define BOSS_SIZE 200.0f            // エネミーのサイズ
#define BOSS_STATE_COUNT 10         // ステート遷移用
#define BOSS_STATE_COUNT_DEATH 100  // ステート遷移用
#define BOSS_ATTACK_INTERVAL 60     // 攻撃のインターバル
#define BOSS_ATTACK_INTERVAL2 20    // 攻撃のインターバル
#define BOSS_BULLET_SPEED 4         // ボスの弾の速度
#define BOSS_ANIM_SPEED 14          // アニメーション速度
#define BOSS_MAX_ANIMATION_X 3      // アニメーション数 横
#define BOSS_MAX_ANIMATION_Y 1      // アニメーション数 縦
#define BOSS_ATTACK_PATARN 3        // 攻撃パターン数
#define BOSS_ATTACK_ON_COUNT 300    // 攻撃状態へ移行するカウント
#define BOSS_ATTACK_OFF_COUNT 500   // 非攻撃状態へ移行するカウント
#define BOSS_TEXTURE_PATH "./data/Textures/balor000.png"//テクスチャのパス

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture  = NULL; // テクスチャ
bool CBoss::m_bIsAlive = false;               // 生きているかのフラグ

//==================================
// コンストラクタ
//==================================
CBoss::CBoss():CScene3d(OBJTYPE_ENEMY)
{
	// メンバ変数のクリア
	m_fBulletAngle = 0.0f;
	m_state = STATE_NORMAL;
	m_nStateCount = 0;
	m_nLife = BOSS_MAX_LIFE;
	m_nCntBullet = 0;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_bAttack = false;
	m_nRandAttack = 0;
}

//==================================
// デストラクタ
//==================================
CBoss::~CBoss()
{
}

//==================================
// クリエイト
//==================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos)
{
	// メモリの確保
	CBoss *pBoss = new CBoss;

	// 初期化
	pBoss->Init();
	// 座標の設定
	pBoss->SetPos(pos);

	// オブジェクトタイプの設定
	pBoss->SetObjType(OBJTYPE_ENEMY);

	return pBoss;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CBoss::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CBoss::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}

//==================================
// 初期化処理
//==================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// 生存フラグ
	m_bIsAlive = true;
	
	// 攻撃パターンをランダムに
	m_nRandAttack = rand() % BOSS_ATTACK_PATARN;

	// テクスチャUV座標の初期化
	m_nAnimX = 2;
	m_nAnimY = 0;

	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / BOSS_MAX_ANIMATION_X;
	float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);
	// エネミー数のカウント
	CEnemy::PlusEnemy();
	// 警告SEの停止
	CManager::GetSound()->Stop(CSound::LABEL_SE_WARNING);
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CBoss::Uninit(void)
{
	
	m_bIsAlive = false;
	// エネミー数のカウントを減らす
	CEnemy::MinusEnemy();

	CScene3d::Uninit();
}

//==================================
// 更新処理
//==================================
void CBoss::Update(void)
{
	// 生きてるとき
	if (m_bIsAlive)
	{
		// アタックの処理
		AttackManager();
	}
	// ステートの管理
	switch (m_state)
	{
	case STATE_NORMAL:
		// 通常時のカラー設定
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case STATE_DAMAGE:
		// ダメージ時のカラー設定
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		m_nStateCount--;
		if (m_nStateCount <= 0)
		{
			m_state = STATE_NORMAL;
		}

		break;
	case STATE_DEATH:

		m_bIsAlive = false;
		m_nStateCount--;
		if (m_nStateCount <= 0)
		{
			Uninit();
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
		break;
	default:
		break;
	}

}

//==================================
// 描画処理
//==================================
void CBoss::Draw(void)
{
	// 生きてるとき
	if (m_bIsAlive)
	{
		CScene3d::Draw();
	}
}

//==================================
// 攻撃がヒットした処理
//==================================
void CBoss::HitAttack(int nDamage)
{
	m_nLife -= nDamage;


	if (m_nLife <= 0)
	{// ライフが0以下
	    
		// ライフがマイナスにならないように
		m_nLife = 0;
	    // 爆発の生成
		CExplosion::Create(GetPos(), D3DXVECTOR3(160, 160, 0), D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f));

		// パーティクル200個生成
		for (int nCntPart = 0; nCntPart < 200; nCntPart++)
		{
			// ランダムなサイズ
			int nRadSize = rand() % 30 + 10;
			// ランダムな角度
			float fRandAngle = D3DXToRadian(rand() % 360);
			// ランダムなスピード
			float fRandSpeed = (rand() % 600 + 100) / 100;
			// パーティクル生成
			CParticle::Create(GetPos(),
				D3DXVECTOR3(cosf(fRandAngle)*fRandSpeed, sinf(fRandAngle)*fRandSpeed, 0.0f),
				D3DXVECTOR3(nRadSize, nRadSize, 0.0f),
				rand() % 30 + 60,
				D3DXCOLOR(((float)(rand() % 100) / 100.0f), ((float)(rand() % 100) / 100.0f) , ((float)(rand() % 100) / 100.0f) , 1.0f),
				CParticle::PARTICLE_STAR);
		}

		// スコア加算
		CScore::AddScore(1000);
		// ステートを死亡状態に
		m_state = STATE_DEATH;
		// カウントセット
		m_nStateCount = BOSS_STATE_COUNT_DEATH;
		return;
	}
	else
	{// ライフが残っている
		m_state = STATE_DAMAGE;
		m_nStateCount = BOSS_STATE_COUNT;
	}
}

//==================================
// 攻撃の管理
//==================================
void CBoss::AttackManager(void)
{
	// カウント増やす
	m_nCntBullet++;
	
	if (m_bAttack)
	{	// 攻撃状態の時

		// 攻撃パターンの分岐
		switch (m_nRandAttack)
		{
		case 0:

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL2 == 0)
			{
				if (CPlayer::IsAlive())
				{
					// 自分の座標
					D3DXVECTOR3 pos = GetPos();
					// プレイヤー座標の取得
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

					// 8wayに弾の生成
					for (int nCntAngle = 0; nCntAngle < 8; nCntAngle++)
					{
						float fAngle = nCntAngle*D3DXToRadian(180 / 8);

						CBullet::Create(GetPos(),
							D3DXVECTOR3(cosf(m_fBulletAngle + fAngle)*BOSS_BULLET_SPEED, sinf(m_fBulletAngle + fAngle)*BOSS_BULLET_SPEED, 0.0f),
							200,
							CBullet::BULLETUSER_ENEMY,
							D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
					}
					// 弾を出す角度を増やす
					m_fBulletAngle += 2.0f;
				}
			}

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL == 0)
			{
				if (CPlayer::IsAlive())
				{
					// 自分の座標
					D3DXVECTOR3 pos = GetPos();
					// プレイヤー座標の取得
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
					// 弾の角度を設定
					m_fBulletAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

					// 各直線の弾数
					const int nNumBullet = 4;
					// 星型に弾幕を作る
					float fStarAngle = 0;
					D3DXVECTOR3 startPos = pos - D3DXVECTOR3(cosf(D3DXToRadian(90)) * 50, sinf(D3DXToRadian(90)) * 50, 0.0f);
				
					float fDistance = sqrtf(powf(startPos.x + (startPos.x + cosf(D3DXToRadian(D3DXToRadian(360 / nNumBullet) * 2)) * 100), 2) -
						powf(startPos.x + (startPos.x + sinf(D3DXToRadian(D3DXToRadian(360 / nNumBullet) * 2)) * 100), 2));

					for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++)
					{

						for (int nCntAngle2 = 0; nCntAngle2 < nNumBullet; nCntAngle2++)
						{
							//
							// 弾の生成
							CBulletEx::Create_Rotation(startPos, pos, D3DXVECTOR3(cosf(m_fBulletAngle) * 6, sinf(m_fBulletAngle) * 6, 0.0f),
								D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f), 120, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f));

							// 角度をプラスする
							startPos += D3DXVECTOR3(cosf(fStarAngle) * fDistance / nNumBullet, sinf(fStarAngle) * fDistance / nNumBullet, 0.0f);
						}
						// プラスする角度の設定
						fStarAngle += D3DXToRadian(360 / 5) * 2;
					}
				}
			}
			break;

		case 1:

			if (m_nCntBullet % BOSS_ATTACK_INTERVAL == 0)
			{
				const int nNumBullet = 8;
				// 八方向に弾を撃ってから自機狙い
				for (int nCntAngle = 0; nCntAngle < nNumBullet; nCntAngle++)
				{
					CBulletEx::Create_Target(GetPos(),
						D3DXVECTOR3(cosf(D3DXToRadian(360 / nNumBullet)*nCntAngle) * BOSS_BULLET_SPEED, sinf(D3DXToRadian(360 / nNumBullet)*nCntAngle) * BOSS_BULLET_SPEED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0.0f),
						80,
						CBullet::BULLETUSER_ENEMY,
						D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
				}

			}

			// 発射するたびに発射角度を0.2fずつ増やす弾を4方向に出す
			if (m_nCntBullet % BOSS_ATTACK_INTERVAL2 == 0)
			{
				// 移動量
				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(m_fBulletAngle) * BOSS_BULLET_SPEED, sinf(m_fBulletAngle) * BOSS_BULLET_SPEED, 0.0f);
				// 弾の生成
				CBullet::Create(GetPos(), bulletMove, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				CBullet::Create(GetPos(), bulletMove*-1, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				// 移動量
				bulletMove = D3DXVECTOR3(cosf(-m_fBulletAngle) * BOSS_BULLET_SPEED, sinf(-m_fBulletAngle) * BOSS_BULLET_SPEED, 0.0f);
				// 弾の生成
				CBullet::Create(GetPos(), bulletMove, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				CBullet::Create(GetPos(), bulletMove*-1, 300, CBullet::BULLETUSER_ENEMY, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f));
				// 発射角度を増やす
				m_fBulletAngle += 0.2f;
			}
			break;

		case 2:

			
			if (rand() % 15 == 0)
			{// 自機狙いの弾

				// 自分の座標
				D3DXVECTOR3 pos = GetPos();
				// プレイヤー座標の取得
				D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
				// 弾の角度を設定
				m_fBulletAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
				m_fBulletAngle += D3DXToRadian(rand() % 20 - 10);
				// 弾のスピード
				float fSpeed = BOSS_BULLET_SPEED * 1.3f;

				// 弾の生成
				CBullet::Create(GetPos(),                                                                              // 座標
					D3DXVECTOR3(cosf(m_fBulletAngle)*fSpeed, sinf(m_fBulletAngle)*fSpeed, 0.0f),                       // 移動量
					450,                                                                                               // 寿命
					CBullet::BULLETUSER_ENEMY,                                                                         // 弾の持ち主
					D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f))->                                                               // 色
				SetSize(D3DXVECTOR3(BULLET_SIZE*1.5f, BULLET_SIZE*1.5f, 0.0f));                                          // サイズ

			}

			if (m_nCntBullet % (BOSS_ATTACK_INTERVAL2 / 2) == 0)
			{

				const int nNumBullet = 6;
				for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
				{
					// 弾の角度を設定
					m_fBulletAngle = D3DXToRadian((360 / nNumBullet)*nCntBullet);
					m_fBulletAngle += D3DXToRadian((360 / nNumBullet) / 2);
					// 弾のスピード
					float fSpeed = BOSS_BULLET_SPEED * 1.5f;

					// 弾の生成
					CBullet::Create(GetPos(),                                                        // 座標
						D3DXVECTOR3(cosf(m_fBulletAngle)*fSpeed, sinf(m_fBulletAngle)*fSpeed, 0.0f), // 移動量
						150,                                                                         // 寿命
						CBullet::BULLETUSER_ENEMY,                                                   // 弾の持ち主
						D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));                                          // 色

				}
			}
			break;

		default:
			break;
		}

		if (m_nCntBullet >= BOSS_ATTACK_OFF_COUNT)
		{
			// 攻撃状態を解除
			m_bAttack = false;
			// カウントの初期化
			m_nCntBullet = 0;

			m_nAnimX = 2;

			// UV座標の設定
			D3DXVECTOR2 uv[NUM_VERTEX];
			float fu = 1.0f / BOSS_MAX_ANIMATION_X;
			float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV座標セット
			SetTextureUV(uv);
		}


	}
	else
	{// 非攻撃状態時

		if (m_nCntBullet >= BOSS_ATTACK_ON_COUNT)
		{
			// 攻撃状態にする
			m_bAttack = true;
			// カウントの初期化
			m_nCntBullet = 0;

			// 攻撃パターンをランダムに
			m_nRandAttack = rand() % BOSS_ATTACK_PATARN;

			m_nAnimX = 1;

			// UV座標の設定
			D3DXVECTOR2 uv[NUM_VERTEX];
			float fu = 1.0f / BOSS_MAX_ANIMATION_X;
			float fv = 1.0f / BOSS_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV座標セット
			SetTextureUV(uv);
		}
	}
}
