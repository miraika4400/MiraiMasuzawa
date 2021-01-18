////////////////////////////////////////////////////
//
//    bulletクラスの処理[bullet.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "manacircle.h"
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
#include "stage.h"
#include "bomb.h"
#include "particle.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MANACIRCRE_TEXTURE_PATH "./data/Textures/circle3.png" // テクスチャのパス
#define CIRCLE_SIZE D3DXVECTOR3(140.0f,140.0f,0.0f)          // サイズ
#define RECOVERYCOUNT 50                                     // サークルを消すカウント
#define CIRCLE_NUM_BOSS_WAVE 6                               // ボス戦時のサークル数
#define CIRCLE_RESPAWN_COUNT_NORMAL 150                      // ボス戦時のサークルのリスポーン時間
#define CIRCLE_RESPAWN_COUNT_HARD   200                      // ボス戦時のサークルのリスポーン時間
#define CIRCLE_ANIM_SPEED 10      // アニメーション速度
#define CIRCLE_MAX_ANIMATION_X 16 // アニメーション数 横
#define CIRCLE_MAX_ANIMATION_Y 1 // アニメーション数 縦
#define CIRCLE_DISTANCE_BOSSWAVE 400 // ボス戦時のサークルの中心からの距離
#define CIRCLE_BOMB_SIZE         300 // サークルの爆発のサイズ
#define MP_RECOVERY_S 10.0f          // サークルのMP回復量＊小
#define MP_RECOVERY_M 15.0f          // ＊中
// サークルに入っている間のMPの回復量
#define REGENE_MP_NORMAL 0.4f  // ノーマル
#define REGENE_MP_HARD 0.25f   // ハード

//******************************
// 静的メンバ変数宣言
//******************************
LPDIRECT3DTEXTURE9 CManaCircle::m_pTexture = NULL; // テクスチャポインタ
//std::list<CManaCircle*> CManaCircle::m_circleList; // サークルリスト
int CManaCircle::m_nNumCircle = 0;
int CManaCircle::m_nMaxCircle = 0;
CManaCircle *CManaCircle::m_apCircle[CIRCLE_MAX] = {};

//******************************
// コンストラクタ
//******************************
CManaCircle::CManaCircle() :CScene3d(OBJTYPE_MANACIRCLE)
{
	// 変数のクリア
	m_nCntRecovery = 0;
	bInCircle = false;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	// サークルの数のカウント
	m_nNumCircle++;

	// サークルの最大数分ループ
	for (int nCntCircle = 0; nCntCircle < CIRCLE_MAX; nCntCircle++)
	{
		if (m_apCircle[nCntCircle] == NULL)
		{
			m_apCircle[nCntCircle] = this;
			m_nIndex = nCntCircle;
			break;
		}
	}
}

//******************************
// デストラクタ
//******************************
CManaCircle::~CManaCircle()
{
}

//******************************
// クリエイト
//******************************
CManaCircle * CManaCircle::Create(const D3DXVECTOR3 pos)
{
	// メモリの確保
	CManaCircle *pMCircle;
	pMCircle = new CManaCircle;
	// 初期化
	pMCircle->Init();

	// 各値の代入・セット
	pMCircle->SetPos(pos);                    // 座標
	pMCircle->SetPriority(OBJTYPE_MANACIRCLE); // オブジェクトタイプ

	// サークルリストに追加
	//m_circleList.push_back(pMCircle);
	return pMCircle;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CManaCircle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MANACIRCRE_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CManaCircle::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// 生成の管理
//******************************
void CManaCircle::SpawnManager(void)
{

	// リスポーン時のカウント
	static int nCntReSpawn[CIRCLE_NUM_BOSS_WAVE] = {};

	if (CGame::GetWave() == CGame::WAVE_NORMAL)
	{// 通常WAVE
		// 静的変数の初期化
		for (int nCntCircle = 0; nCntCircle < CIRCLE_NUM_BOSS_WAVE; nCntCircle++)
		{
			if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL)
			{// ノーマル
				nCntReSpawn[nCntCircle] = CIRCLE_RESPAWN_COUNT_NORMAL;
			}
			else
			{// ハード
				nCntReSpawn[nCntCircle] = CIRCLE_RESPAWN_COUNT_HARD;
			}
		}

		// サークルを最大数にする
		if (m_nNumCircle < m_nMaxCircle)
		{
			// ランダムな座標の取得
			D3DXVECTOR3 randPos = GetRandCirclePos();

			// サークル生成
			CManaCircle::Create(randPos);
		}
	}
	else
	{//BOSSWAVE

		// ステージの座標の取得
		D3DXVECTOR3 stagePos = STAGE_POS;

		// サークルの最大数
		for (int nCntCircle = 0; nCntCircle < CIRCLE_NUM_BOSS_WAVE; nCntCircle++)
		{
			// CIRCLE_NUM_BOSS_WAVE方向の位置の取得
			D3DXVECTOR3 pos;
			pos.x = stagePos.x + cosf(nCntCircle*D3DXToRadian(360 / CIRCLE_NUM_BOSS_WAVE)) * CIRCLE_DISTANCE_BOSSWAVE;
			pos.y = stagePos.y + sinf(nCntCircle*D3DXToRadian(360 / CIRCLE_NUM_BOSS_WAVE)) * CIRCLE_DISTANCE_BOSSWAVE;
			pos.z = 0.0f;

			if (m_apCircle[nCntCircle] == NULL)
			{
				// カウントを進める
				nCntReSpawn[nCntCircle]++;
				if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL && nCntReSpawn[nCntCircle] >= CIRCLE_RESPAWN_COUNT_NORMAL||
					CGame::GetDefficult() == CGame::DEFFICULT_HARD   && nCntReSpawn[nCntCircle] >= CIRCLE_RESPAWN_COUNT_HARD)
				{
					// カウントの初期化
					nCntReSpawn[nCntCircle] = 0;
					// サークルの生成
					m_apCircle[nCntCircle] = CManaCircle::Create(pos);

					// 配列が前詰めで生成されるので調整
					if (nCntCircle != m_apCircle[nCntCircle]->m_nIndex)
					{
						m_apCircle[m_apCircle[nCntCircle]->m_nIndex] = NULL;
						m_apCircle[nCntCircle]->m_nIndex = nCntCircle;
					}
				}

			}
			else
			{
				// カウントの初期化
				nCntReSpawn[nCntCircle] = 0;

				// 座標の設定
				if (m_apCircle[nCntCircle]->GetPos() != pos)
				{
					m_apCircle[nCntCircle]->Init();
					m_apCircle[nCntCircle]->SetPos(pos);
				}
			}
		}
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CManaCircle::Init(void)
{
	// 初期化
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	// テクスチャ割り当て
	BindTexture(m_pTexture);
	// サイズ
	SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
	bInCircle = false;

	// UV座標の設定

	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / CIRCLE_MAX_ANIMATION_X;
	float fv = 1.0f / CIRCLE_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	// 色の設定
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	SetColor(col);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CManaCircle::Uninit(void)
{
	// NULL
	m_apCircle[m_nIndex] = NULL;

	// サークルの数を減らす
	m_nNumCircle--;

	// 終了処理
	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CManaCircle::Update(void)
{

	// アニメーション
	AnimationManager();

	// サークルのサイズ
	D3DXVECTOR3 size = GetSize();

	size += (CIRCLE_SIZE - size)*0.05f;

	SetSize(size);

	// 当たり判定
	CollisionCircle();
}

//******************************
// 描画処理
//******************************
void CManaCircle::Draw(void)
{
	CScene3d::Draw();
}

//******************************************
// サークルを置くランダムな座標の取得
//******************************************
D3DXVECTOR3 CManaCircle::GetRandCirclePos(void)
{
	// ステージ座標の取得
	D3DXVECTOR3 stagePos = STAGE_POS;

	// ランダムな角度
	int nRandAngle = rand() % 360;
	// ランダムな距離
	float fRandDistance = fmodf((float)rand(), STAGE_SIZE.x / 2 - CIRCLE_SIZE.x) - (STAGE_SIZE.x / 2 - CIRCLE_SIZE.x);
	// ランダムな座標
	D3DXVECTOR3 randPos = stagePos + D3DXVECTOR3(cosf(D3DXToRadian(nRandAngle))*fRandDistance, sinf(D3DXToRadian(nRandAngle))*fRandDistance, 0);

	// ほかのサークルと重なってないか
	for (int nCntCircle = 0; nCntCircle < CIRCLE_MAX; nCntCircle++)
	{
		if (m_apCircle[nCntCircle] != NULL)
		{
			// サークルの座標
			D3DXVECTOR3 circlePos = m_apCircle[nCntCircle]->GetPos();
			// サークルのサイズ
			D3DXVECTOR3 circleSize = m_apCircle[nCntCircle]->GetSize();

			while (powf(randPos.x - circlePos.x, 2) + powf(randPos.y - circlePos.y, 2) <= powf(CIRCLE_SIZE.x + circleSize.x, 2))
			{
				// ランダムな座標
				randPos = GetRandCirclePos();

				nCntCircle = 0;
			}
		}
	}

	return randPos;
}


//******************************
// アニメーションの管理
//******************************
void CManaCircle::AnimationManager(void)
{
	// アニメーションカウントを進める
	m_nCntAnim++;

	if (m_nCntAnim % CIRCLE_ANIM_SPEED == 0)
	{
		// アニメーションX軸の加算
		m_nAnimX++;

		if (m_nAnimX >= CIRCLE_MAX_ANIMATION_X)
		{
			// アニメーションX軸の初期化
			m_nAnimX = 0;
		}
		else
		{
			// UV座標の設定
			D3DXVECTOR2 uv[NUM_VERTEX];

			float fu = 1.0f / CIRCLE_MAX_ANIMATION_X;
			float fv = 1.0f / CIRCLE_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY - fv);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			// UV座標セット
			SetTextureUV(uv);
		}


	}
}

//******************************
// 当たり判定
//******************************
void CManaCircle::CollisionCircle(void)
{
	// サークルの座標
	D3DXVECTOR3 pos = GetPos();
	// サークルのサイズ
	D3DXVECTOR3 size = GetSize();
	// SE再生のチェック
	static bool bSE = false;
	// プレイヤーとの当たり判定

	// プレイヤーが生きているとき
	if (CPlayer::IsAlive())
	{
		// プレイヤーの座標
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
		// プレイヤーのサイズ
		D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();

		// 当たり判定
		if (powf(pos.x - playerPos.x, 2) + powf(pos.y - playerPos.y, 2) <= powf(size.x / 2 + playerSize.x / 2, 2))
		{
			if (!bSE)
			{
				bSE = true;
				// SE再生
				CManager::GetSound()->Play(CSound::LABEL_SE_CIRCLE);
			}
			
			// 入っているときa値を上げる
			D3DXCOLOR col = GetColor();
			col.a += (1.0f - col.a)*0.1f;
			SetColor(col);

			// サークルに入った
			bInCircle = true;

			// MPの回復
			if (CGame::GetDefficult() == CGame::DEFFICULT_NORMAL)
			{// ノーマル
				CGame::GetPlayer()->RecoveryMp(REGENE_MP_NORMAL);
			}
			else
			{// ハード
				CGame::GetPlayer()->RecoveryMp(REGENE_MP_HARD);
			}

			m_nCntRecovery++;

			// パーティクルの生成
			// 何個生成するかランダム
			int nRandNum = rand() % 5 + 3;//3～8個
			for (int nCntPartcle = 0; nCntPartcle < nRandNum; nCntPartcle++)
			{
				// ランダムな距離
				int nRandDistance = rand() % 30 + 10;// 10～40
													 // ランダムな角度
				int nRandAngle = rand() % 360;//0～360
											  // ↑の情報で座標の生成
				D3DXVECTOR3 randPos;
				randPos.x = cosf(D3DXToRadian(nRandAngle))*nRandDistance;
				randPos.y = sinf(D3DXToRadian(nRandAngle))*nRandDistance;
				randPos.z = 0;
				randPos += playerPos;

				// ランダムなサイズ
				int nRandSize = rand() % 10 + 5;//5～15

												// 移動量をプレイヤー方向に設定
				D3DXVECTOR3 move = playerPos - randPos;
				move.z = 0;
				D3DXVec3Normalize(&move, &move);
				// パーティクルの生成
				CParticle::Create(randPos,
					move*(float)(rand() % 100 + 1) / 50,
					D3DXVECTOR3((float)nRandSize,
					(float)nRandSize, 0.0f),
					30,
					D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f, 1.0f));

			}

			// サークルに入っていられる限界
			if (m_nCntRecovery >= RECOVERYCOUNT)
			{

				// MPの回復
				CGame::GetPlayer()->RecoveryMp(MP_RECOVERY_M);
				// ボムの生成
				CBomb::Create(pos, CIRCLE_BOMB_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				LostCircle();
				// SEを非再生状態にする
				bSE = false;
				return;
			}
		}
		else
		{
			m_nCntRecovery = 0;
			if (bInCircle)
			{
				// MPの回復
				CGame::GetPlayer()->RecoveryMp(MP_RECOVERY_S);
				// ボムの生成
				CBomb::Create(pos, CIRCLE_BOMB_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				LostCircle();

				// SEを非再生状態にする
				bSE = false;
				return;
			}
		}
	}

	// エネミーとの当たり判定 

	// エネミーリストの取得
	std::list<CEnemy*>::iterator iteretorEnemy;
	for (iteretorEnemy = CEnemy::GetEnemylist()->begin(); iteretorEnemy != CEnemy::GetEnemylist()->end(); iteretorEnemy++)
	{
		// エネミーの座標
		D3DXVECTOR3 enemyPos = (*iteretorEnemy)->GetPos();
		// エネミーのサイズ
		D3DXVECTOR3 enemySize = (*iteretorEnemy)->GetSize();

		// 当たり判定
		if (powf(pos.x - enemyPos.x, 2) + powf(pos.y - enemyPos.y, 2) <= powf(size.x / 2 + enemySize.x / 2, 2))
		{
			//	サークルから敵の角度
			float fAngle = atan2f(enemyPos.y - pos.y, enemyPos.x - pos.x);
			enemyPos.x = pos.x + cosf(fAngle)*(size.x / 2 + enemySize.x / 2);
			enemyPos.y = pos.y + sinf(fAngle)*(size.x / 2 + enemySize.x / 2);
			(*iteretorEnemy)->SetPos(enemyPos);

		}
	}
}

//******************************
// サークルの消失処理
//******************************
void CManaCircle::LostCircle(void)
{
	// SEの停止
	CManager::GetSound()->Stop(CSound::LABEL_SE_CIRCLE);
	
	if (CGame::GetWave() == CGame::WAVE_NORMAL)
	{
		// 初期化
		Init(); 
		// 位置をランダムで移動させる
		SetPos(GetRandCirclePos());
	}
	else
	{
		Uninit();
	}
}
