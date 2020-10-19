//===================================================
//
//    エネミークラスの処理[enemy.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "bulletex.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "stage.h"
#include "score.h"
#include "boss.h"
#include "item.h"
#include "time.h"
#include "particle.h"
#include "manacircle.h"
#include "spawneffect.h"

//*****************************
// マクロ定義
//*****************************
#define ENEMY_SPEED 2.5f         // エネミーのスピード
#define MOVE_COUNT 100           // 移動方向変更時のカウント
#define ENEMY_SIZE 40.0f         // エネミーのサイズ
#define ENEMY_DEFAULT_LIFE 1     // デフォルトのライフ
#define ENEMY_STATE_COUNT 10     // ステート遷移用
#define ENEMY_SPAWN_COUNT 200 
#define ENEMY_SPAWN_DISTANCE 200  // スポーンするときの距離の最低
#define MOVE_CHANGE_DISTANCE 100

//テクスチャのパス
#define ENEMY_TEXTURE_PATH_YELLOW "./data/Textures/enemy000.png" // 黄色エネミー
#define ENEMY_TEXTURE_PATH_PURPLE "./data/Textures/enemy001.png" // 紫色エネミー
#define ENEMY_TEXTURE_PATH_RED "./data/Textures/enemy002.png"	 // 赤色エネミー

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_MAX] = {};
std::list<CEnemy*> CEnemy::m_enemyList; // エネミーリスト
int CEnemy::m_nCntSpawn = 0;            // スポーンカウント
int CEnemy::m_nNumEnemy = 0;            // エネミー、ボスの数

//==================================
// コンストラクタ
//==================================
CEnemy::CEnemy() :CScene3d(OBJTYPE_ENEMY)
{
	// メンバ変数のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_fMoveAngle = 0.0f;
	m_nLife = ENEMY_DEFAULT_LIFE;
	m_state = STATE_NORMAL;
	m_nStateCount = 0;
	m_nCntEncount = 0;
	m_nCntMove = 0;
	m_bPlusAngle = false;
}

//==================================
// デストラクタ
//==================================
CEnemy::~CEnemy()
{
}

//==================================
// クリエイト
//==================================
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const ENEMYTYPE type)
{
	// メモリの確保
	CEnemy *pEnemy = new CEnemy;
	// 種類の設定
	pEnemy->m_type = type;
	// 初期化
	pEnemy->Init();
	// 座標の設定
	pEnemy->SetPos(pos);

	// オブジェクトタイプの設定
	pEnemy->SetObjType(OBJTYPE_ENEMY);

	// エネミーリストに追加
	m_enemyList.push_back(pEnemy);

	return pEnemy;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_YELLOW, &m_pTexture[ENEMYTYPE_YELLOW]);// 黄色エネミー
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_PURPLE, &m_pTexture[ENEMYTYPE_PURPLE]);// 紫エネミー
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_PATH_RED, &m_pTexture[ENEMYTYPE_RED]);      // 赤エネミー

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CEnemy::Unload(void)
{
	// テクスチャの解放処理
	for (int nCntTex = 0; nCntTex < sizeof(m_pTexture) / sizeof(LPDIRECT3DTEXTURE9); nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// スポーンの管理
//==================================
void CEnemy::SpawnManager(void)
{
	int nMaxEnemy = 10;
	int nSpawnCount = 200;


	// 時間によって難易度調整
	if (CTime::GetTime() / 1000 >= BOSS_SPAWN_SEC)
	{
		// サークルの最大数
		CManaCircle::SetCircleMax(6);
		CGame::SetWave(CGame::WAVE_BOSS);
	}
	else if (CTime::GetTime() / 1000 >= 40)
	{// 60秒

		// 敵の最大数（およそ）
		nMaxEnemy = 25;
		// エネミーのスポーン頻度
		nSpawnCount = 100;
		// サークルの最大数
		CManaCircle::SetCircleMax(5);
	}
	else if (CTime::GetTime() / 1000 >= 30)
	{// 40秒

		 // 敵の最大数（およそ）
		nMaxEnemy = 20;
		// エネミーのスポーン頻度
		nSpawnCount = 125;
		// サークルの最大数
		CManaCircle::SetCircleMax(4);
	}
	else if (CTime::GetTime() / 1000 >= 20)
	{// 20秒
		
		// 敵の最大数（およそ）
		nMaxEnemy = 15;
		// エネミーのスポーン頻度
		nSpawnCount = 150;
	}
	else
	{
		// 敵の最大数（およそ）
		nMaxEnemy = 10;
		// エネミーのスポーン頻度
		nSpawnCount = 200;
		// サークルの最大数
		CManaCircle::SetCircleMax(3);
	}
	

	if (m_nNumEnemy <= nMaxEnemy)
	{
		// カウントを進める
		m_nCntSpawn++;

		// 一定の間隔で
		if (m_nCntSpawn%nSpawnCount == 0)
		{
			// 湧く数のランダム
			int nRand = rand() % 5 + 3;

			for (int nCnt = 0; nCnt < nRand; nCnt++)
			{
				// ステージ座標の取得
				D3DXVECTOR3 stagePos = STAGE_POS;

				// ランダムな角度
				int nRandAngle = rand() % 360;
				// ランダムな距離
				int nRandDistance = rand() % (int)STAGE_SIZE.x - (int)STAGE_SIZE.x / 2;
				// 上の情報で座標の生成
				D3DXVECTOR3 randPos = D3DXVECTOR3(stagePos.x + cosf(D3DXToRadian(nRandAngle))*nRandDistance, stagePos.y + sinf(D3DXToRadian(nRandAngle))*nRandDistance, 0);

				// プレイヤーが生きているとき
				if (CPlayer::IsAlive())
				{
					// プレイヤー座標の取得
					D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

					// プレイヤーに近かった時乱数の降りなおし
					while (ENEMY_SPAWN_DISTANCE > sqrtf(powf(randPos.x - playerPos.x, 2) + powf(randPos.y - playerPos.y, 2)))
					{
						// ランダムな角度
						nRandAngle = rand() % 361;
						// ランダムな距離
						nRandDistance = rand() % (int)STAGE_SIZE.x - (int)STAGE_SIZE.x / 2;
						// 上の情報で座標の生成
						randPos = D3DXVECTOR3(stagePos.x + cosf(D3DXToRadian(nRandAngle))*nRandDistance, stagePos.y + sinf(D3DXToRadian(nRandAngle))*nRandDistance, 0);
					}

					// エネミーの生成
					CEnemySpawn::Create(randPos, (CEnemy::ENEMYTYPE)(rand() % CEnemy::ENEMYTYPE_MAX));
				}
			}
		}
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CEnemy::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// テクスチャ割り当て
	BindTexture(m_pTexture[m_type]);

	// サイズの設定
	SetSize(D3DXVECTOR3(ENEMY_SIZE, ENEMY_SIZE, 0.0f));
	
	if (m_type == ENEMYTYPE_RED)
	{
		// 移動量の初期化
		int nRand = rand() % 360;

		// 移動量の設定
		m_move.x = cosf(D3DXToRadian(nRand))*ENEMY_SPEED*2.0f;
		m_move.y = sinf(D3DXToRadian(nRand))*ENEMY_SPEED*2.0f;
	}

	// エネミー数のカウント
	m_nNumEnemy++;

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CEnemy::Uninit(void)
{
	// リストから削除
	std::list<CEnemy*>::iterator iteretor;
	for (iteretor = m_enemyList.begin(); iteretor != m_enemyList.end(); )
	{
		// インデックス番号が同じだった時
		if (GetID() == (*iteretor)->GetID())
		{
			iteretor = m_enemyList.erase(iteretor);
			break;
		}
		else
		{
			iteretor++;
		}
	}

	CScene3d::Uninit();

	// エネミー数を減らす
	m_nNumEnemy--;
}

//==================================
// 更新処理
//==================================
void CEnemy::Update(void)
{

	// 現在の座標の取得
	D3DXVECTOR3 pos = GetPos();

	// 移動前の座標の保存
	D3DXVECTOR3 posOld = pos;

	if (CPlayer::IsAlive())
	{
		// 移動量の設定
		MoveManager();
	}

	// 座標に移動量のプラス
	pos += m_move;

	// 別のエネミーとの当たり判定
	std::list<CEnemy*>::iterator iteretor;
	for (iteretor = m_enemyList.begin(); iteretor != m_enemyList.end(); iteretor++)
	{
		if (GetID() != (*iteretor)->GetID())
		{
			// 自身のサイズ
			D3DXVECTOR3 size = GetSize();
			// 別の敵の座標
			D3DXVECTOR3 enemyPos = (*iteretor)->GetPos();
			// 別の敵のサイズ
			D3DXVECTOR3 enemySize = (*iteretor)->GetSize();

			if (powf(pos.x - enemyPos.x, 2) + powf(pos.y - enemyPos.y, 2) <= powf(ENEMY_SIZE, 2))
			{
				// 角度
				float fAngle = atan2f(enemyPos.y - pos.y, enemyPos.x - pos.x);
				// 座標の設定
				enemyPos.x = pos.x + cosf(fAngle)*ENEMY_SIZE;
				enemyPos.y = pos.y + sinf(fAngle)*ENEMY_SIZE;

				// 座標のセット
				(*iteretor)->SetPos(enemyPos);
			}
		}
	}

	// プレイヤーの当たり判定
	if (CPlayer::IsAlive())
	{

		D3DXVECTOR3 size = GetSize();  // 弾のサイズ

		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();   // プレイヤーの座標
		D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize(); // プレイヤーのサイズ

		if (powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2) <= powf(playerSize.x / 2 + size.x / 2, 2))
		{
			if (CGame::GetPlayer()->GetState() == CPlayer::STATE_NORMAL)
			{
				CGame::GetPlayer()->HitAttack(1);
			}
		}
	}

	// ステージとの当たり判定

	// ステージ座標の取得
	D3DXVECTOR3 stagePos = STAGE_POS;
	// 距離の計算
	float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

	// ステージ端の設定
	if (fDistanceStage + ENEMY_SIZE / 2 > STAGE_SIZE.x / 2)
	{
		// 角度
		float fAngle = atan2f(pos.y - stagePos.y, pos.x - stagePos.x);
		// 座標の設定
		pos.x = stagePos.x + cosf(fAngle)*(STAGE_SIZE.x / 2 - ENEMY_SIZE / 2);
		pos.y = stagePos.y + sinf(fAngle)*(STAGE_SIZE.x / 2 - ENEMY_SIZE / 2);
	}

	// 座標のセット
	SetPos(pos);

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
	default:
		break;
	}

	// ボス戦時エネミーを消す
	if (CGame::GetWave() == CGame::WAVE_BOSS)
	{
		
		// パーティクル生成
		ParticleCreate();

		// 消す
		Uninit();
	}
	
}

//==================================
// 描画処理
//==================================
void CEnemy::Draw(void)
{
	CScene3d::Draw();
}

//==================================
// 攻撃がヒットした処理
//==================================
void CEnemy::HitAttack(int nDamage)
{
	// ライフからダメージを引く
	m_nLife -= nDamage;


	if (m_nLife <= 0)
	{// ライフが0以下

		// パーティクル生成
		ParticleCreate();
		// スコア加算
		CScore::AddScore(100);

		if ((rand() % 10) == 0)
		{
			// アイテム生成
			CItem::Create(GetPos(), (CItem::ITEMTYPE)(rand() % CItem::ITEMTYPE_MAX));
		}
		// 消す
		Uninit();

		return;
	}
	else
	{// ライフが残っている
		m_state = STATE_DAMAGE;
		m_nStateCount = ENEMY_STATE_COUNT;
	}
}

//==================================
// 移動の管理
//==================================
void CEnemy::MoveManager(void)
{
	
	// 現在の座標の取得
	D3DXVECTOR3 pos = GetPos();

	// プレイヤー座標の取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	

	// タイプによって移動を分ける
	switch (m_type)
	{
	case ENEMYTYPE_YELLOW:
	{// 黄色エネミー

		// プレイヤーとの角度
		float fAnglePlayer = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		// 移動量の設定
		m_move.x = cosf(fAnglePlayer)*ENEMY_SPEED;
		m_move.y = sinf(fAnglePlayer)*ENEMY_SPEED;

		// 回転させる
		SetAngle(D3DXToDegree(fAnglePlayer));
	}
	break;

	case ENEMYTYPE_PURPLE:
	{// 紫エネミー

		// プレイヤーとの距離の取得
		float fDistance = sqrtf(powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2));

		// プレイヤーとの角度
		m_fMoveAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

		// ジグザグ動かす距離（一定の距離以内だと自機の方向に移動）
		if (fDistance >= MOVE_CHANGE_DISTANCE)
		{
			// カウントを進める
			m_nCntMove++;
			// 一定のカウントでプラスかマイナスか切り替え
			if (m_nCntMove >= MOVE_COUNT)
			{
				m_nCntMove = rand() % 20;
				m_bPlusAngle ^= true;
			}

			if (m_bPlusAngle)
			{// プラス
				m_fMoveAngle += D3DXToRadian(45);
			}
			else
			{// マイナス
				m_fMoveAngle -= D3DXToRadian(45);
			}
		}

		// 移動量の設定
		m_move.x = cosf(m_fMoveAngle)*ENEMY_SPEED*1.5f;
		m_move.y = sinf(m_fMoveAngle)*ENEMY_SPEED*1.5f;

		float fAngle = atan2f(m_move.y, m_move.x);
		// 回転させる
		SetAngle(D3DXToDegree(fAngle));
	}
	break;

	case ENEMYTYPE_RED:
	{// 赤エネミー

		// ステージ座標の取得
		D3DXVECTOR3 stagePos = STAGE_POS;
		// 距離の計算
		float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

		// ステージ端の設定
		if (fDistanceStage + ENEMY_SIZE / 2 > STAGE_SIZE.x / 2)
		{
			// プレイヤーとの角度
			m_fMoveAngle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

			// 移動量の設定
			m_move.x = cosf(m_fMoveAngle)*ENEMY_SPEED*2.0f;
			m_move.y = sinf(m_fMoveAngle)*ENEMY_SPEED*2.0f;
		}

		SetAngle(GetAngle()+2.0f);
	}
	break;

	default:
		break;
	}

	
}

//==================================
// エネミーロスト時のパーティクル生成
//==================================
void CEnemy::ParticleCreate(void)
{
	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		// ランダムな速度
		int nRandSpeed = rand() % 3 + 1;
		// ランダムな角度
		int nRandAngle = rand() % 360;
		// 上の情報で移動量の生成
		D3DXVECTOR3 particleMove;
		particleMove.x = cosf(D3DXToRadian(nRandAngle))*nRandSpeed;
		particleMove.y = sinf(D3DXToRadian(nRandAngle))*nRandSpeed;
		particleMove.z = 0.0f;

		// ランダムなサイズ
		int nRandSize = rand() % 10 + 10;

		// エネミーのタイプにっよって色分け
		D3DXCOLOR randCol = {};
		switch (m_type)
		{
		case ENEMYTYPE_YELLOW:
			// 黄色っぽく
			randCol = D3DXCOLOR(1.0f, 1.0f, (float)(rand() % 600) / 1000.0f, 1.0f);
			break;
		case ENEMYTYPE_PURPLE:
			// 紫っぽく
			randCol = D3DXCOLOR(1.0f, (float)(rand() % 600) / 1000.0f, 1.0f, 1.0f);
			break;
		case ENEMYTYPE_RED:
			// 赤っぽく
			randCol = D3DXCOLOR(1.0f, (float)(rand() % 600) / 1000.0f, (float)(rand() % 600) / 1000.0f, 1.0f);
			break;
		default:
			break;
		}

		// パーティクル生成
		CParticle::Create(GetPos(),
			particleMove,
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f), rand() % 10 + 20,
			randCol,
			CParticle::PARTICLE_STAR);

	}
}
