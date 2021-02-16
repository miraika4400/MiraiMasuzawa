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

//*****************************
// マクロ定義
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // 重力量
#define CHARACTER_GRAVITY_RATE 0.03f                    // 重力の係数
#define CHARACTER_DIRECTION_RATE 0.3f                   // 回転角度の係数
#define CHARACTER_MOVE_RATE 0.05f                       // 移動量の係数
//#define CHARACTER_MOVE_RATE 1.0f                      // 移動量の係数
#define CHARACTER_RADIUS 100.0f                         // プレイヤー半径(当たり判定)

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
	m_bGravity   = true;       // 重力フラグ
	m_pCollision = NULL;       // コリジョン
	m_rankData   = {};         // 順位付け用データ
	m_bGoal = false;           // ゴールフラグ
	m_impact = VEC3_ZERO;      // 衝突したときの移動量

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

	// 順位付けデータの初期化
	m_rankData.nCheck = 0;
	m_rankData.nLap   = 0;
	m_rankData.nRank  = 1;

	// ゴールフラグの初期化
	m_bGoal = false;

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCharacter::Uninit(void)
{
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

	// 移動量徐々に目標値に近づける
	m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

	// 座標に移動量を足す
	SetPos(GetPos() + m_move);

	// モデルクラスの更新処理
	CModelShader::Update();

	// 当たり判定の位置更新
	m_pCollision->SetPos(GetPos());
}

//******************************
// 描画処理
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
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
	}
	else
	{// 重力フラグが立ってないとき
	 // 初期化
		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// 向きの管理
//******************************
void CCharacter::Direction(void)
{
	// 移動量が0じゃないとき
	if (m_move != VEC3_ZERO)
	{
		// 向きの取得
		D3DXVECTOR3 rot = GetRot();

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
