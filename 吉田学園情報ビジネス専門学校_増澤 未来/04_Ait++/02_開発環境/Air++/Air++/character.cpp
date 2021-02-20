//=============================================================================
//
// characterヘッダ [character.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "collision.h"
#include "course.h"
#include "checkpoint.h"

//*****************************
// マクロ定義
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // 重力量
#define CHARACTER_GRAVITY_RATE 0.03f                    // 重力の係数
#define CHARACTER_DIRECTION_RATE 0.3f                   // 回転角度の係数
#define CHARACTER_MOVE_RATE 0.05f                       // 移動量の係数
//#define CHARACTER_MOVE_RATE 1.0f                      // 移動量の係数
#define CHARACTER_ROT_X_ADJUST_RANGE 300.0f             // 地面と角度を合わせるときの地面との距離の範囲
#define RAY_DISTANCE 50.0f                              // レイを出す距離*向き調整に使用
#define RAY_HEIGHT   100.0f                             // レイを出す高さ*向き調整に使用
#define CHARACTER_SPEED_BASE 30.0f                      // 移動スピード
#define CHARACTER_SPEED_ACCELERATION 45.0f              // 移動スピード*加速時
#define CHARACTER_ACCELERATION_RATE 0.06f               // 加速時のスピード変化の係数
#define CHARACTER_ACCELERATION_COUNT 70                 // 加速フレーム数
#define CHARACTER_STAN_FRAME 70                         // スタンの長さ(フレーム)
#define CHARACTER_FALL_COUNT 40                         // 重力フラグが一定フレーム経ってたら落下と判定する

//*****************************
// 静的メンバ変数宣言
//*****************************

//******************************
// コンストラクタ
//******************************
CCharacter::CCharacter(int nPliority) :CModelShader(nPliority)
{
	// 変数のクリア
	m_move       = VEC3_ZERO;  // 移動量
	m_moveDist   = VEC3_ZERO;  // 移動量の目標値
	m_gravityVec = VEC3_ZERO;  // 重力量
	m_bGravity   = false;       // 重力フラグ
	m_pCollision = NULL;       // コリジョン
	m_rankData   = {};         // 順位付け用データ
	m_bGoal = false;           // ゴールフラグ
	m_impact = VEC3_ZERO;      // 衝突したときの移動量
	m_item = CItem::ITEM_NONE; // 所持しているアイテム
	m_bAcceleration = false;   // 加速フラグ
	m_fSpeed = CHARACTER_SPEED_BASE; // 移動速度
	m_nCntAccleration = 0;     // 加速時のカウント
	m_bStan = false;           // スタンフラグ
	m_nCntStan = 0;            // スタンカウント
	m_nCntGravity = 0;         // 重力フラグが経っている間のカウント

	// オブジェクトタイプがプレイヤーの時、プレイヤーフラグをtrueにする
	if (nPliority == OBJTYPE_PLAYER) m_bIsPlayer = true;
	else m_bIsPlayer = false;
}

//******************************
// デストラクタ
//******************************
CCharacter::~CCharacter()
{
}

//******************************
// 初期化処理
//******************************
HRESULT CCharacter::Init(void)
{
	// モデルクラスの初期化処理
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// コリジョンの生成
	m_pCollision = CCollision::CreateSphere(GetPos(), CHARACTER_RADIUS);

	// 変数の取得
	m_bGoal = false;                 // ゴールフラグ
	m_item = CItem::ITEM_NONE;       // 所持しているアイテム
	m_bAcceleration = false;         // 加速フラグ
	m_fSpeed = CHARACTER_SPEED_BASE; // 移動速度
	m_nCntAccleration = 0;     // 加速時のカウント
	m_bStan = false;           // スタンフラグ
	m_nCntStan = 0;            // スタンカウント
	m_bGravity = false;        // 重力フラグ
	m_nCntGravity = 0;         // 重力フラグが経っている間のカウント

	// 順位付けデータの初期化
	m_rankData.nCheck = 0;
	m_rankData.nLap = 0;
	m_rankData.nRank = 1;

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCharacter::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModelShader::Uninit();
}

//******************************
// 更新処理
//******************************
void CCharacter::Update(void)
{
	// 重力処理
	Gravity();
	
	// 向きの処理
	Direction();

	// キャラ同士の当たり判定の処理
	CollisionCharacter();

	// スピードの管理
	SpeedManager();

	// 移動量徐々に目標値に近づける
	m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

	// 座標に移動量を足す
	SetPos(GetPos() + m_move);

	// モデルクラスの更新処理
	CModelShader::Update();

	if (m_pCollision != NULL)
	{
		// 当たり判定の位置更新
		m_pCollision->SetPos(GetPos());
	}
}

//******************************
// 描画処理
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// 加速フラグのセット
//******************************
void CCharacter::SetActiveAcceleration(bool bBool)
{
	// 引数のセット
	m_bAcceleration = bBool;

	if (m_bAcceleration)
	{// 引数がtrueだった時
		// カウントの初期化
		m_nCntAccleration = 0;

		// スタン状態の解除
		m_bStan = false;
	}
}

//******************************
// スタンフラグのセット
//******************************
void CCharacter::SetStan(bool bBool)
{
	// 引数のセット
	m_bStan = bBool;
	
	if (m_bStan)
	{// 引数がtrueだった時

		// 加速状態の解除
		m_bAcceleration = false;
	}
}

//******************************
// 重力の管理
//******************************
void CCharacter::Gravity(void)
{
	if (m_bGravity)
	{// 重力フラグが立ってたら

		// 重力量の設定
		m_gravityVec += (CHARACTER_GRAVITY - m_gravityVec)*CHARACTER_GRAVITY_RATE;

		// 座標に重力量のプラス
		SetPos(GetPos() + m_gravityVec);
		m_nCntGravity++;

		if (m_nCntGravity >= CHARACTER_FALL_COUNT)
		{
			if (m_rankData.nCheck != 0)
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(m_rankData.nCheck - 1)->GetPos());
			}
			else
			{
				SetPos(VEC3_ZERO);
			}
		}
	}
	else
	{// 重力フラグが立ってないとき
	 // 初期化
		if (m_nCntGravity != 0)m_nCntGravity = 0;

		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// 向きの管理
//******************************
void CCharacter::Direction(void)
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// Y軸

	// 移動量が0じゃないとき&&スタン状態じゃないとき
	if (m_move != VEC3_ZERO && !m_bStan)
	{
		
		// Y軸の目標値
		float fRotYDist = atan2f(m_move.x, m_move.z);

		// グルんと回転しないよう調整
		while (rot.y - fRotYDist > D3DXToRadian(180))
		{
			fRotYDist += D3DXToRadian(360);
		}
		while (rot.y - fRotYDist < D3DXToRadian(-180))
		{
			fRotYDist -= D3DXToRadian(360);
		}

		// 徐々に目標値に近づける
		rot.y += (fRotYDist - rot.y) * CHARACTER_DIRECTION_RATE;

		// 向きのセット
		SetRot(rot);
	}

	// X軸

	if (CGame::GetCourse() != NULL)
	{
		// レイを出す位置
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);

		// コースメッシュの取得
		LPD3DXMESH pCourseMesh = CGame::GetCourse()->GetMesh();
		// 
		BOOL bHit = false;
		float fHitDistance;

		// レイによる当たり判定
		D3DXIntersect(pCourseMesh,
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fHitDistance,
			NULL,
			NULL);

		if (bHit)
		{// 当たっていた時

			if (fHitDistance <= CHARACTER_ROT_X_ADJUST_RANGE)
			{
				// 当たっている座標
				D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X軸の目標値
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// 徐々に目標値に近づける
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				// 向きのセット
				SetRot(rot);
			}
		}
	}
}

//*****************************************
// キャラクター同士の当たり判定処理
//*****************************************
void CCharacter::CollisionCharacter(void)
{
	// キャラクター数分ループ
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター情報の取得
		CCharacter *pTarget = CGame::GetCharacter(nCnt);

		// NULLチェック&自分じゃないとき
		if (pTarget != NULL&&pTarget != this)
		{
			if (CCollision::CollisionSphere(m_pCollision, pTarget->m_pCollision))
			{// 当たっていた時
			 
				// 外に押し出す
				D3DXVECTOR3 vec = (GetPos() - pTarget->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pTarget->GetCollision()->GetCollisionRadius());
				// 座標の更新
				SetPos(pTarget->GetPos() + vec);
			}
		}
	}
}

//*****************************************
// スピード管理処理
//*****************************************
void CCharacter::SpeedManager(void)
{
	if (m_bAcceleration)
	{// 加速

		// 加速状態のスピードに近づける
		m_fSpeed += (CHARACTER_SPEED_ACCELERATION- m_fSpeed)*CHARACTER_ACCELERATION_RATE;
		
		// カウントを進める
		m_nCntAccleration++;
		if (m_nCntAccleration > CHARACTER_ACCELERATION_COUNT)
		{// 加速カウントが一定に達したら
			// 加速終了
			m_bAcceleration = false;
			// カウントの初期化
			m_nCntAccleration = 0;
		}
	}
	else if (m_bStan)
	{// スタン状態

		// 加速状態のスピードに近づける
		m_fSpeed += (0.0f - m_fSpeed)*CHARACTER_MOVE_RATE;

		// カウントを進める
		m_nCntStan++;
		D3DXVECTOR3 rot = GetRot();
		rot.y += D3DXToRadian(360 / CHARACTER_STAN_FRAME);
		SetRot(rot);
		if (m_nCntStan > CHARACTER_STAN_FRAME)
		{// 加速カウントが一定に達したら
		 // 加速終了
			m_bStan = false;
			// カウントの初期化
			m_nCntStan = 0;
		}
	}
	else
	{
		// 非加速状態のスピードに近づける
		m_fSpeed += (CHARACTER_SPEED_BASE - m_fSpeed)*CHARACTER_MOVE_RATE;
	}
}
