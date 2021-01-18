////////////////////////////////////////////////////
//
//    destinationクラスの処理[destination.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "destination.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "mouse.h"
#include "model.h"
#include "player.h"
#include "game.h"

//*****************************
// マクロ定義
//*****************************
#define GRAVITY D3DXVECTOR3(0.0f,-40.0f,0.0f)  // 重力量
#define GRAVITY_RATE   0.1f                    // 重力の係数
#define DIRECTION_RATE 0.1f                    // 向きを変える係数
#define DISTANCE_RATE  0.05f                   // 距離の係数
#define ADD_DRIFT      0.0075f                 // ドリフト時の加算値

//*****************************
// 静的メンバ変数宣言
//*****************************


//******************************
// コンストラクタ
//******************************
CDestination::CDestination() :CScene(OBJTYPE_DESTINATION)
{
	// 変数のクリア
	m_pos = VEC3_ZERO;                        // 座標
	m_rot = VEC3_ZERO;                        // ロット
	m_rotDest = VEC3_ZERO;                    // ロット目標値
	m_move = VEC3_ZERO;                       // 移動量
	m_nPlayerNum = 0;                         // プレイヤー番号
	m_gravityVec = VEC3_ZERO;                 // 重力量
	m_bGravity = true;                        // 重力フラグ
	m_fDistancePlayer = 0;                    // プレイヤーとの距離
	m_fDistanceDest = DISTANCE_PLAYER_DEFAULT;   // プレイヤーとの距離の目標値
	m_bGoal = false;                          // ゴールフラグ
#ifdef _DEBUG
	m_pMeshModel = NULL;	//メッシュ情報へのポインタ
	m_pBuffMatModel = NULL;	//マテリアル情報へのポインタ
	m_nNumMatModel = NULL;	//マテリアル情報の数
	pModel = NULL;          // モデルクラスのポインタ
#endif
}

//******************************
// デストラクタ
//******************************
CDestination::~CDestination()
{
}

//******************************
// クリエイト関数
//******************************
CDestination * CDestination::Create(const int nPlayerNum, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// メモリの確保
	CDestination*pDest;
	pDest = new CDestination;
	
	if (pDest != NULL)
	{
		// 初期化
		pDest->Init();
		// 引数の代入
		pDest->m_pos = pos;                // 座標
		pDest->m_rot = rot;				   // ロット
		pDest->m_rotDest = rot;			   // ロット目標値
		pDest->m_nPlayerNum = nPlayerNum;  // プレイヤー番号
	}

	return pDest;
}

//******************************
// 初期化処理
//******************************
HRESULT CDestination::Init(void)
{
#ifdef _DEBUG

	//メッシュの生成
	CreateMesh();

	pModel = CModel::Create();
	if (pModel != NULL)
	{
		// モデル割り当て
		pModel->BindModel(m_pMeshModel, m_pBuffMatModel, 1);
	}
#endif
	// プレイヤーとの距離の初期化
	m_fDistancePlayer = DISTANCE_PLAYER_INIT;     // プレイヤーとの距離
	m_fDistanceDest = DISTANCE_PLAYER_DEFAULT;    // プレイヤーとの距離の目標値
	
	// ゴールフラグの初期化
	m_bGoal = false;

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CDestination::Uninit(void)
{
#ifdef _DEBUG
	//メッシュの破棄
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	//マテリアルの破棄
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}
#endif // _DEBUG

	Release();
}

//******************************
// 更新処理
//******************************
void CDestination::Update(void)
{
	// 重力の処理
	//Gravity();
	// 移動管理
	MoveControll();
	// 向きの管理
	Direction();

	// プレイヤー情報の取得
	CPlayer*pPlayer = CGame::GetPlayer(m_nPlayerNum);
	if (pPlayer != NULL)
	{
		// プレイヤーの座標の取得
		D3DXVECTOR3 playerPos = pPlayer->GetPos();
		// 高さをプレイヤーに合わせる
		m_pos.y = playerPos.y;

		playerPos.x = m_pos.x + cosf(-m_rot.y - D3DXToRadian(90)) * m_fDistancePlayer;
		playerPos.z = m_pos.z + sinf(-m_rot.y - D3DXToRadian(90)) * m_fDistancePlayer;
		pPlayer->SetPos(playerPos);

		D3DXVECTOR3 playerRot = pPlayer->GetRot();
		playerRot.y = atan2f(m_pos.z - playerPos.z, m_pos.x - playerPos.x);
		playerRot.y *= -1;
		pPlayer->SetRot(playerRot);
	}

#ifdef _DEBUG
	//m_pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	if (pModel != NULL)
	{
		pModel->SetPos(m_pos);
		pModel->SetRot(m_rot);
	}

#endif // _DEBUG
}

//******************************
// 描画処理
//******************************
void CDestination::Draw(void)
{
#ifdef _DEBUG

#endif // _DEBUG
}



//******************************
// 向きの管理
//******************************
void CDestination::Direction(void)
{

	// プレイヤー情報の取得
	CPlayer*pPlayer = CGame::GetPlayer(m_nPlayerNum);
	if (pPlayer != NULL)
	{
		if (!pPlayer->GetGoalFlag())
		{// プレイヤーがゴール状態じゃないとき

			if (pPlayer->GetDriftLeft())
			{
				m_rotDest.y += -ADD_DRIFT;
				m_rotDest.y += -ADD_DRIFT;
			}
			else if (pPlayer->GetDriftRight())
			{
				m_rotDest.y += ADD_DRIFT;
				m_rotDest.y += ADD_DRIFT;
			}
#ifdef _DEBUG
			// マウスでの向き操作
			m_rotDest.y += CManager::GetMouse()->GetMouseMove().x / MOUSE_SENSI_RATE;
#endif
			// コントローラーでの向き操作
			m_rotDest.y += CManager::GetJoypad()->GetStick(m_nPlayerNum).lX / JOYPAD_SENSI_RATE;

			m_rot += (m_rotDest - m_rot)*DIRECTION_RATE;
		}
		else
		{
			if (!m_bGoal)
			{
				m_rotDest.y += D3DXToRadian(-180);
				m_bGoal = true;

				// 回転しすぎないように差を調整
				while (m_rotDest.y - m_rot.y > 180.0f)
				{
					m_rotDest.y -= 360.0f;
				}
				while (m_rotDest.y - m_rot.y < -180.0f)
				{
					m_rotDest.y += 360.0f;
				}
			}
			
			m_rot += (m_rotDest - m_rot)*0.01f;
		}
	}

	
}

#ifdef _DEBUG
//******************************
// メッシュ生成
//******************************
void CDestination::CreateMesh(void)
{
	const float c_fRadius = 50.0f;
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateCylinder(pDevice, 1, c_fRadius, c_fRadius*1.5f, 3, 1,&m_pMeshModel, &m_pBuffMatModel);

	// 色の設定
	D3DXMATERIAL* mat    = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	mat->MatD3D.Ambient  = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Diffuse  = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Specular = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Emissive = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
}

#endif // _DEBUG

//******************************
// プレイヤーとの距離のセット
//******************************
void CDestination::SetDistancePlayer(float fDistance)
{
	// プレイヤーとの距離セット
	m_fDistancePlayer = fDistance;
	// プレイヤーとの距離の目標値セット
	//m_fDistanceDest = m_fDistancePlayer;

	// プレイヤー情報の取得
	CPlayer*pPlayer = (CPlayer*)CScene::GetTop(OBJTYPE_PLAYER);
	while (pPlayer != NULL)
	{
		if (pPlayer->GetPlayerNum() == m_nPlayerNum)
		{
			// プレイヤーの座標の取得
			D3DXVECTOR3 playerPos = pPlayer->GetPos();
			// 高さをプレイヤーに合わせる
			m_pos.y = playerPos.y;

			m_pos.x = playerPos.x + cosf(-m_rot.y - D3DXToRadian(90)) * -m_fDistancePlayer;
			m_pos.z = playerPos.z + sinf(-m_rot.y - D3DXToRadian(90)) * -m_fDistancePlayer;
			SetPos(m_pos);
			
			break;
		}

		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
}

//******************************
// 移動の管理
//******************************
void CDestination::MoveControll(void)
{
	//移動量の目標値
	D3DXVECTOR3 moveDest = VEC3_ZERO;

#if 1
	if (CGame::GetPlayer(m_nPlayerNum)->GetMoveFlag())
	{// 移動フラグがtrueの時
		if (!CGame::GetPlayer(m_nPlayerNum)->GetGoalFlag())
		{// ゴールしてないとき
			// 移動量の目標値の設定
			moveDest.x = cosf(-m_rot.y - D3DXToRadian(90))* CGame::GetPlayer(m_nPlayerNum)->GetMaxSpeed();
			moveDest.z = sinf(-m_rot.y - D3DXToRadian(90))* CGame::GetPlayer(m_nPlayerNum)->GetMaxSpeed();

			// プレイヤーの距離を目標値に近づける
			m_fDistancePlayer += (m_fDistanceDest - m_fDistancePlayer)*DISTANCE_RATE;
			SetDistancePlayer(m_fDistancePlayer);
		}
	}
	else
	{// プレイヤーとの距離のリセット
		SetDistancePlayer(DISTANCE_PLAYER_INIT);
	}
#else
	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
	{
		moveDest.x = cosf(-m_rot.y-D3DXToRadian(90))* PLAYER_SPEED;
		moveDest.z = sinf(-m_rot.y-D3DXToRadian(90))* PLAYER_SPEED;
	}

#endif

	// 慣性
	m_move += (moveDest - m_move) * CGame::GetPlayer(m_nPlayerNum)->GetMoveRate();

	// 移動量を足す
	m_pos += m_move;
}

//******************************
// 重力の管理
//******************************
void CDestination::Gravity(void)
{
	if (m_bGravity)
	{// 重力フラグが立ってたら

		// 重力量の設定
		m_gravityVec += (GRAVITY - m_gravityVec)*GRAVITY_RATE;

		// 座標に重力量のプラス
		//SetPos(GetPos() + m_gravityVec);

	}
	else
	{// 重力フラグが立ってないとき
		// 初期化
		m_gravityVec = VEC3_ZERO;
	}
}