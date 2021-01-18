////////////////////////////////////////////////////
//
//    particleクラスの処理[particle.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "camera.h"
#include "game.h"
#include "debug_log.h"
#include "collision.h"
#include "destination.h"
#include "mouse.h"
#include "item.h"
#include "rank.h"
#include "gauge.h"
#include "chain_ui.h"
#include "goal_ui.h"
#include "evolution_effect.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH           "./data/Models/testplayer.x"            //モデルのパス
#define HIERARCHY_TEXT_PATH1 "./data/Texts/hierarchy/Player1.txt"    //階層構造テキストのパス
#define HIERARCHY_TEXT_PATH2 "./data/Texts/hierarchy/Player2.txt"    //階層構造テキストのパス
#define HIERARCHY_TEXT_PATH3 "./data/Texts/hierarchy/Player3.txt"    //階層構造テキストのパス
#define HIERARCHY_TEXT_PATH4 "./data/Texts/hierarchy/Player4.txt"    //階層構造テキストのパス

#define PLAYER_GRAVITY D3DXVECTOR3(0.0f,-120.0f,0.0f)   // 重力量
#define PLAYER_GRAVITY_RATE 0.3f                     // 重力の係数
#define PLAYER_DIRECTION_RATE 0.1f                   // 向きの係数
#define PLAYER_RADIUS 150                            // 当たり判定の半径
#define HIT_FRAME  100                               // ヒット状態のフレーム数
#define ACCELERATION_FRAME  100                      // 加速持続フレーム
#define ACCELERATION_DIST   50                       // 加速時の加算値
#define ACCELERATION_RATE   0.03f                    // 加速値の係数
#define ACCELERATION_ITEM_DROP_INTERVAL 20           // 加速時にアイテムを落とすインターバル
#define CHAIN_SPEED_BONUS 1                          // チェインスピードボーナスの最大
#define CHAIN_SPEED_RATE  0.25f                      // チェインスピードボーナスの係数

// プレイヤー速度
#define PLAYER_SPEED_E1 100       // プレイヤー速度*1段階目
#define PLAYER_SPEED_E2 120       // プレイヤー速度*2段階目
#define PLAYER_SPEED_E3 140       // プレイヤー速度*3段階目
#define PLAYER_SPEED_E4 160       // プレイヤー速度*4段階目
#define PLAYER_SPEED_HIT 20       // 障害物ヒット時のプレイヤー速度
// 加速度
#define PLAYER_MOVE_RATE_E1 0.03f  // 慣性の係数*1段階目
#define PLAYER_MOVE_RATE_E2 0.03f  // 慣性の係数*2段階目
#define PLAYER_MOVE_RATE_E3 0.025f // 慣性の係数*3段階目
#define PLAYER_MOVE_RATE_E4 0.02f  // 慣性の係数*4段階目
// 進化毎のパーツ数
#define EVOLUTION_0 1             // 初期
#define EVOLUTION_1 2             // 2段階目
#define EVOLUTION_2 3             // 3段階目
#define EVOLUTION_3 m_nNumModel   // 4段階目

//　各進化までの値
#define EVOLUTION_NUM_1 5   // 2段階目
#define EVOLUTION_NUM_2 10   // 3段階目
#define EVOLUTION_NUM_3 15   // 4段階目

// 進化あゲージサイズ
#define GAUGE_SIZE D3DXVECTOR3(20.0f,150.0f,0.0f)

// 進化ゲージ位置
#define GAUGE_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2)                  , 0.0f)  // 画面分割してないとき
#define GAUGE_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define GAUGE_POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define GAUGE_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define GAUGE_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2-(SCREEN_HEIGHT/4)), 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define GAUGE_POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // プレイヤー3
#define GAUGE_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 50 , (SCREEN_HEIGHT/2+(SCREEN_HEIGHT/4)), 0.0f)  // プレイヤー4

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model CPlayer::m_model[MAX_PLAYER_NUM][MAX_PARTS_NUM] = {};
int CPlayer::m_nNumModel[MAX_PLAYER_NUM] = {};

// 進化ごとのパーツ数
int CPlayer::m_nNumEvolutionParts[MAX_EVOLUTION]=
{
	EVOLUTION_0,
	EVOLUTION_1,
	EVOLUTION_2,
	4,
};
// 進化ごとの最高速度
int CPlayer::m_nSpeedData[MAX_EVOLUTION] =
{
	PLAYER_SPEED_E1,       // プレイヤー速度*1段階目
	PLAYER_SPEED_E2,       // プレイヤー速度*2段階目
	PLAYER_SPEED_E3,       // プレイヤー速度*3段階目
	PLAYER_SPEED_E4,       // プレイヤー速度*4段階目
};

// 進化ごとの加速度
float CPlayer::m_fRateData[MAX_EVOLUTION] =
{
	PLAYER_MOVE_RATE_E1,   // 慣性の係数*1段階目
	PLAYER_MOVE_RATE_E2,   // 慣性の係数*2段階目
	PLAYER_MOVE_RATE_E3,   // 慣性の係数*3段階目
	PLAYER_MOVE_RATE_E4,   // 慣性の係数*4段階目
};

int CPlayer::m_nEvoData[MAX_EVOLUTION] =
{
	EVOLUTION_NUM_1, // 2段階目
	EVOLUTION_NUM_2, // 3段階目
	EVOLUTION_NUM_3, // 4段階目
	1,
};

char CPlayer::m_acModelText[MAX_PLAYER_NUM][64]=
{
	{ HIERARCHY_TEXT_PATH1 },
	{ HIERARCHY_TEXT_PATH2 },
	{ HIERARCHY_TEXT_PATH3 },
	{ HIERARCHY_TEXT_PATH4 },
};

//******************************
// コンストラクタ
//******************************
CPlayer::CPlayer() :CModelHierarchy(OBJTYPE_PLAYER)
{
	// 変数のクリア
	//m_move = VEC3_ZERO;              // 移動量
	m_nPlayerNum = 0;                  // プレイヤー番号
	m_gravityVec = VEC3_ZERO;          // 重力量
	m_bGravity = true;                 // 重力フラグ
	m_pCollision = NULL;               // コリジョン
	m_nNumCheckpoint = 0;              // チャックポイント数
	m_bGoal = false;                   // ゴールフラグ
	m_bMove = false;                   // 移動フラグ
	m_nChain = 0;                      // チェイン数
	m_nMaxChain = 0;                   // 最大チェイン数
	m_nCollectItem = 0;                // 回収したアイテム数
	m_nNumEvolution = 0;               // 進化回数
	m_fEvoGauge = 0.0f;                // 進化ゲージの値
	m_pDest = NULL;                    // 移動目標クラス
	m_bDriftLeft = false;              // ドリフト左
	m_bDriftRight = false;             // ドリフト右
	m_bHit = false;                    // ヒット状態フラグ
	m_nCntHit = 0;                     // ヒット状態解除時のカウント
	m_fMaxSpeed = PLAYER_SPEED_E1;     // 最大速度
	m_bAccelerationFlag = false;       // 加速フラグ
	m_nCntAcceleration = 0;            // 加速中のカウント
	m_fAcceleration = 0.0f;            // 加速状態の値
	m_fMoveRate = PLAYER_MOVE_RATE_E1; // 慣性の係数
	m_nRank = 0;                       // 順位
	m_pGauge = NULL;
}

//******************************
// デストラクタ
//******************************
CPlayer::~CPlayer()
{
}

//******************************
// クリエイト
//******************************
CPlayer * CPlayer::Create(D3DXVECTOR3 pos,int nPlayerNum)
{
	// メモリの確保
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	pPlayer->m_nPlayerNum = nPlayerNum;
	// 初期化
	pPlayer->Init();
	
	// 各値の代入・セット
	pPlayer->SetPos(pos);
	pPlayer->SetPriority(OBJTYPE_PLAYER); // オブジェクトタイプ
	// 当たり判定の生成
	pPlayer->m_pCollision = CCollision::CreateSphere(pos, PLAYER_RADIUS);
	
	// 移動目標の生成
	pPlayer->m_pDest = CDestination::Create(nPlayerNum, pos);
	return pPlayer;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CPlayer::Load(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER_NUM; nCnt++)
	{
		// モデルの読み込み
		LoadModels(m_acModelText[nCnt], &m_model[nCnt][0], &m_nNumModel[nCnt]);
		// 最終進化をパーツ数
		m_nNumEvolutionParts[MAX_EVOLUTION - 1] = m_nNumModel[nCnt];
	}

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CPlayer::Unload(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_NUM; nCntPlayer++)
	{
		for (int nCnt = 0; nCnt < m_nNumModel[nCntPlayer]; nCnt++)
		{
			//メッシュの破棄
			if (m_model[nCntPlayer][nCnt].pMesh != NULL)
			{
				m_model[nCntPlayer][nCnt].pMesh->Release();
				m_model[nCntPlayer][nCnt].pMesh = NULL;
			}
			//マテリアルの破棄
			if (m_model[nCntPlayer][nCnt].pBuffMat != NULL)
			{
				m_model[nCntPlayer][nCnt].pBuffMat->Release();
				m_model[nCntPlayer][nCnt].pBuffMat = NULL;
			}
		}
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CModelHierarchy::Init(m_nNumEvolutionParts[m_nNumEvolution], &m_model[m_nPlayerNum][0], m_acModelText[m_nPlayerNum])))
	{
		return E_FAIL;
	}

	// モデル情報の割り当て
	//BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);
	//
	//// テクスチャの割り当て
	//for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	//{
	//	if (m_model.apTexture[nCnt] != NULL)
	//	{
	//		BindTexture(nCnt, m_model.apTexture[nCnt]);
	//	}
	//}

	// 変数の初期化
	m_bGravity = true;                 // 重力フラグ
	m_nNumCheckpoint = 0;              // チェックポイント数
	m_bGoal = false;                   // ゴールフラグ
	m_bMove = false;                   // 移動フラグ
	m_nChain = 0;                      // チェイン数
	m_nCollectItem = 0;                // 回収したアイテム数
	m_bDriftLeft = false;              // ドリフト左
	m_bDriftRight = false;             // ドリフト右
	m_bHit = false;                    // ヒットフラグ
	m_nCntHit = 0;                     // ヒット時のカウント
	m_fMaxSpeed = PLAYER_SPEED_E1;     // 最大速度
	m_bAccelerationFlag = false;       // 加速フラグ
	m_fMoveRate = PLAYER_MOVE_RATE_E1; // 慣性の係数
	m_nRank = m_nPlayerNum + 1;        // 順位
	m_fEvoGauge = 0.0f;                // 進化ゲージの値
	m_nMaxChain = 0;                   // 最大チェイン数

	// ランクUIの生成
	CRank::Create(m_nPlayerNum);

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();
	D3DXVECTOR3 gaugePos;
	D3DXVECTOR3 gaugeSize;
	// プレイヤー番号によって分岐
	switch (m_nPlayerNum)
	{
	case 0:
		// プレイヤー1
		// 分割なし
		if (nNumPlayer == 1)
		{
			gaugePos = GAUGE_POS_PLAYER1_1;
			gaugeSize = GAUGE_SIZE;
		}
		// 二分割
		else if (nNumPlayer == 2) 
		{
			gaugePos = GAUGE_POS_PLAYER1_2; 
			gaugeSize = GAUGE_SIZE / 1.5f;
		}
		// 四分割
		else
		{
			gaugePos = GAUGE_POS_PLAYER1_4; 
			gaugeSize = GAUGE_SIZE / 2;
		}

		break;
	case 1:
		// プレイヤー2

		// 二分割
		if (nNumPlayer == 2)
		{
			gaugePos = GAUGE_POS_PLAYER2_2;
			gaugeSize = GAUGE_SIZE / 1.5f;
		}
		// 四分割
		else
		{
			gaugePos = GAUGE_POS_PLAYER2_4;
			gaugeSize = GAUGE_SIZE / 2;
		}

		break;
	case 2:
		// プレイヤー3
		gaugePos = GAUGE_POS_PLAYER3;
		gaugeSize = GAUGE_SIZE / 2;
		break;
	case 3:
		// プレイヤー4
		gaugePos = GAUGE_POS_PLAYER4;
		gaugeSize = GAUGE_SIZE / 2;
		break;
	default:
		break;
	}
	gaugePos.y -= gaugeSize.y / 2;
	m_pGauge = CGauge::Create(&m_fEvoGauge, D3DXVECTOR3(gaugePos.x, gaugePos.y + gaugeSize.y, 0.0f), gaugeSize.x, gaugeSize.y * 2, EVOLUTION_NUM_1, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	// チェインUIの生成
	CChainUi::Create(m_nPlayerNum);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CPlayer::Uninit(void)
{
	CModelHierarchy::Uninit();
}

//******************************
// 更新処理
//******************************
void CPlayer::Update(void)
{
	// 重力の処理
	Gravity();
	// ドリフトの処理
	Drift();
	
	// 当たり判定の位置更新
	m_pCollision->SetPos(GetPos());

	// ヒット状態の管理
	if (m_bHit)
	{// ヒット状態の時
		m_nCntHit--;
		if (m_nCntHit <= 0)
		{// カウントが0以下の時
			// ヒットフラグをfalseに
			m_bHit = false;
		}
		m_fMaxSpeed = PLAYER_SPEED_HIT;
	}
	else
	{
		// 最高速度の更新
		m_fMaxSpeed = m_nSpeedData[m_nNumEvolution];

		// チェイン数によって最高速度に加算
		m_fMaxSpeed += (CHAIN_SPEED_BONUS*m_nChain)*CHAIN_SPEED_RATE;
	}

	// 加速の処理
	Acceleration();
	// プレイヤー同士の当たり判定
	CollisionPlayer();
#ifdef _DEBUG
	// デバッグコマンド

	// 進化
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_E))
	{
		Evolution();
	}

	// アイテムドロップ
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_D))
	{
		if (m_nPlayerNum == 0)
		{
			// アイテムを飛び散らせる
			CItem::DropItemCircle(D3DXVECTOR3(GetPos().x, GetPos().y + 80, GetPos().z), 5, m_nPlayerNum);
		}
		
	}
	if (CManager::GetKeyboard()->GetKeyPress(DIK_S))
	{
	//	CItem::DropItem(GetPos(), m_nPlayerNum);
	}

	// ストップ
	if (CManager::GetKeyboard()->GetKeyPress(DIK_S))
	{
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyRelease(DIK_S))
	{
		m_bMove = true;
	}

	// 加速
	if (!m_bAccelerationFlag)
	{
		if (CManager::GetKeyboard()->GetKeyPress(DIK_A))
		{
			SetAccelerationFrag(true);
		}
	}
#endif
}

//******************************
// 描画処理
//******************************
void CPlayer::Draw(void)
{
	CModelHierarchy::Draw();
}

//******************************
// ゴール時の処理
//******************************
void CPlayer::Goal(void)
{
	// ゴールフラグをtrueに
	m_bGoal = true;
	// ゴールUIの生成
	CGoalUi::Create(m_nPlayerNum);
}

//******************************
// チェイン数の加算
//******************************
void CPlayer::AddChainNum(void)
{
	// 加算
	m_nChain++;

	if (m_nChain >= m_nMaxChain)
	{
		// 最大化チェイン数の保存
		m_nMaxChain = m_nChain;
	}
}

//******************************
// 加速フラグのセット
//******************************
void CPlayer::SetAccelerationFrag(bool bAccele)
{
	m_bAccelerationFlag = bAccele;
	
	if (m_bAccelerationFlag)
	{
		// カウントの初期化
		m_nCntAcceleration = 0;
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ACCELERATION);
	}
}

//******************************
// アイテムに当たったときの処理
//******************************
void CPlayer::HitItem(bool bSafe)
{
	if (bSafe)
	{// 自分アイテムの当たったとき
		m_nCollectItem++;
		AddChainNum();
		if (m_nNumEvolution < MAX_EVOLUTION)
		{
			m_fEvoGauge++;
			if (m_nEvoData[m_nNumEvolution] <= m_fEvoGauge)
			{
				Evolution();
			}
		}
		else
		{
			m_nNumEvolution = 1;
		}
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM);
	}
	else
	{// 自分以外のアイテムに当たったとき
		
		// チェイン数を0にする
		m_nChain = 0;
		// 進化の値を0にする
		m_fEvoGauge = 0.0f;
		// ヒットフラグを立てる
		m_bHit = true;
		// カウントの初期化
		m_nCntHit = HIT_FRAME;
		// 加速をfalseにする
		m_bAccelerationFlag = false;
		// アイテムを飛び散らせる
		CItem::DropItemCircle(D3DXVECTOR3(GetPos().x, GetPos().y + 80, GetPos().z), 5, m_nPlayerNum);
		// カメラを揺らす
		CGame::GetCamera(m_nPlayerNum)->Shake(true);

		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT);
	}
}

//******************************
// 操作の管理
//******************************
void CPlayer::MoveControll(void)
{
//	// 座標
//	D3DXVECTOR3 pos = GetPos();
//	//移動量の目標値
//	D3DXVECTOR3 moveDest = VEC3_ZERO;
//
//	// カメラの向きの取得
//	float fCameraAngle = CGame::GetCamera(m_nPlayerNum)->GetAngle();
//#if 0
//	if (m_bMove)
//	{
//		moveDest.x = cosf(fCameraAngle)* PLAYER_SPEED;
//		moveDest.z = sinf(fCameraAngle)* PLAYER_SPEED;
//	}
//
//#else
//	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
//	{
//		moveDest.x = cosf(fCameraAngle)* PLAYER_SPEED;
//		moveDest.z = sinf(fCameraAngle)* PLAYER_SPEED;
//	}
//
//#endif
//	// 慣性
//	m_move += (moveDest - m_move) * PLAYER_MOVE_RATE;
//
//	// 移動量を足す
//	pos += m_move;
//
//	// 座標のセット
//	SetPos(pos);
//
//	// プレイヤーの向きをカメラに合わせる
//	D3DXVECTOR3 rot = GetRot();
//	rot.y = -(fCameraAngle + D3DXToRadian(90));
//	//rot.x += 0.01f;
//	SetRot(rot);
}

//******************************
// 重力の管理
//******************************
void CPlayer::Gravity(void)
{
	if (m_bGravity)
	{// 重力フラグが立ってたら

		// 重力量の設定
		m_gravityVec += (PLAYER_GRAVITY - m_gravityVec)*PLAYER_GRAVITY_RATE;

		// 座標に重力量のプラス
		SetPos(GetPos() + m_gravityVec);

	}
	else
	{// 重力フラグが立ってないとき
		// 初期化
		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// 進化
//******************************
void CPlayer::Evolution(void)
{
	// 進化回数のカウント
	m_nNumEvolution++;
	if (m_nNumEvolution >= MAX_EVOLUTION)
	{// 進化数が最大を超えないように
		m_nNumEvolution = MAX_EVOLUTION - 1;
	}
	else
	{
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EVOLUTION);
		// 進化エフェクトとの生成
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			float fRandX = 0.0f;
			float fRandY = D3DXToRadian(rand() % 360);
			float fRandZ = 0.0f;
			CEvoEffect::Create(m_nPlayerNum)->SetRot(D3DXVECTOR3(fRandX, fRandY, fRandZ));
		}

		// アイテムを飛び散らせる
		CItem::DropItemCircle(GetPos(), 5, m_nPlayerNum);
	}

	// 最高速度の更新
	m_fMaxSpeed = m_nSpeedData[m_nNumEvolution];
	// 加速度
	m_fMoveRate = m_fRateData[m_nNumEvolution];
	// パーツ数の読み込み
	CModelHierarchy::Init(m_nNumEvolutionParts[m_nNumEvolution], &m_model[m_nPlayerNum][0], m_acModelText[m_nPlayerNum]);
	// 進化値の初期化
	m_fEvoGauge = 0.0f;
	// ゲージの最大数の更新
	m_pGauge->SetMaxNum(m_nEvoData[m_nNumEvolution]);
}

//******************************
// ドリフト
//******************************
void CPlayer::Drift(void)
{
	float fDistance = 0.0f;// 移動目標との距離

	if (!m_bDriftLeft && !m_bDriftRight)
	{// ドリフトがファルス状態の時

		// キーボード
		if (CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			if (CManager::GetMouse()->GetMouseMove().x > 0)
			{// 右ドリフト
				m_bDriftRight = true;

			}
			else if (CManager::GetMouse()->GetMouseMove().x < 0)
			{// 左ドリフト
				m_bDriftLeft = true;
			}
		}

		// コントローラー
		if (CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum))
		{
			if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX >= 10)
			{// 右ドリフト
				m_bDriftRight = true;
			}
			else if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX <= -10)
			{// 左ドリフト
				m_bDriftLeft = true;
			}
		}
	}
	else
	{
		// ドリフト状態の解除
		if (CManager::GetJoypad()->GetJoystickRelease(5, m_nPlayerNum) || CManager::GetKeyboard()->GetKeyRelease(DIK_LSHIFT))
		{
			m_bDriftLeft = false;
			m_bDriftRight = false;
		}
	}
	//if (m_pDest->GetDistanceDest() != fDistance)
	//{
	//	m_pDest->SetDistanceDest(fDistance);
	//}
}

//******************************
// 加速の処理
//******************************
void CPlayer::Acceleration(void)
{
	if (m_bAccelerationFlag)
	{// 加速フラグが立っているとき
		// ヒットフラグをfalseにする
		m_bHit = false;
		// 加速値を目標に近づける
		m_fAcceleration += (ACCELERATION_DIST - m_fAcceleration)*ACCELERATION_RATE;
		// カウントを進める
		m_nCntAcceleration++;

		if (m_nCntAcceleration%ACCELERATION_ITEM_DROP_INTERVAL == 0)
		{
			CItem::DropItem(GetPos(), m_nPlayerNum);
		}
		//一定のカウントに達したら
		if (m_nCntAcceleration > ACCELERATION_FRAME)
		{
			// カウントの初期化
			m_nCntAcceleration = 0;
			// 加速フラグをfalseにする
			m_bAccelerationFlag = false;
		}
	}
	else
	{
		// 加速値を0近づける
		m_fAcceleration += (0 - m_fAcceleration)*ACCELERATION_RATE;
	}

	m_fMaxSpeed += m_fAcceleration;
}

//******************************
// プレイヤー同士の当たり判定
//******************************
void CPlayer::CollisionPlayer(void)
{
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);
	while (pPlayer != NULL)
	{
		if (pPlayer->GetID() != GetID())
		{
			// 当たり判定
			if (CCollision::CollisionSphere(m_pCollision, pPlayer->GetCollision()))
			{// 当たってた時

				// 外に押し出す
				D3DXVECTOR3 vec = (GetPos() - pPlayer->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pPlayer->GetCollision()->GetCollisionRadius());

				D3DXVECTOR3 pos = GetPos();
				SetPos(pPlayer->GetPos() + vec);
			}
		}

		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
}
