//=============================================================================
//
// playerヘッダ [player.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "rank_ui.h"
#include "locus.h"
#include "course.h"
#include "item_ui.h"
#include "trap.h"
#include "attack.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH     "./data/Models/player.x" // モデルのパス
#define SHADER_PATH    "./data/HLSL/Shader.fx"  // HLSLファイルのパス
#define LOCUS_DISTANCE 80.0f                    // 軌跡を出す距離
#define LOCUS_ADJUST 10.0f                      // 軌跡を出す高さの調整
#define TRAP_SET_DISTACE 150.0f                 // トラップを置く距離
#define ATTACK_SET_DISTACE 300.0f                 // トラップを置く距離

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CPlayer::m_model = {};  //メッシュ情報へのポインタ
CScene::Shader CPlayer::m_shader = {};  // シェーダー構造体

//******************************
// コンストラクタ
//******************************
CPlayer::CPlayer() :CCharacter(OBJTYPE_PLAYER)
{
	// 変数のクリア
	m_nPlayerNum = -1;  // プレイヤーID
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
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum)
{
	// メモリの確保
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	// プレイヤー番号の設定
	pPlayer->m_nPlayerNum = nPlayerNum; 

	// 初期化
	pPlayer->Init();

	// 各値の代入・セット
	pPlayer->SetPos(pos); // 座標のセット
	pPlayer->SetRot(rot); // 回転のセット
	
	return pPlayer;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// テクスチャ読み込み
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	// HLSLファイルの読み込み
	D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CPlayer::Unload(void)
{
	//メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//******************************
// 初期化処理
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// シェーダーのセット
	SetShader(m_shader);

	// 順位UIの生成
	CRankUi::Create(m_nPlayerNum);
	// アイテムUIの生成
	CItemUi::Create(m_nPlayerNum);
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();
}

//******************************
// 更新処理
//******************************
void CPlayer::Update(void)
{
	// 軌跡用データの保持
	D3DXVECTOR3 posOld = GetPos();
	float fAngeleOld = -GetRot().y;

	if (!GetGoalFlag())
	{
		// 移動量の設定
		Move();
	}
	else
	{
		// 移動量のセット
		SetMoveDist(VEC3_ZERO);
	}

	// キャラクタークラスの更新処理*移動処理・向きの処理・重力処理・当たり判定
	CCharacter::Update();

	// コースの当たり判定
	CGame::GetCourse()->CollisionCharacter((CCharacter*)this);

	// アイテム使用
	UseItem();

	//***********************************************************
	// 後で関数化したい
	
	if (GetActiveAcceleration())
	{// 加速状態の時

		// 軌跡を出す
		// プレイヤーの向いている向きの取得
		float fAngele = -GetRot().y;

		D3DXVECTOR3 aPos[NUM_VERTEX / 2] =
		{
			{ GetPos().x + cosf(fAngele) *  LOCUS_DISTANCE, GetPos().y + LOCUS_ADJUST, GetPos().z + sinf(fAngele) *  LOCUS_DISTANCE },
			{ GetPos().x + cosf(fAngele) * -LOCUS_DISTANCE, GetPos().y + LOCUS_ADJUST, GetPos().z + sinf(fAngele) * -LOCUS_DISTANCE },
		};

		D3DXVECTOR3 aPosOld[NUM_VERTEX / 2] =
		{
			{ posOld.x + cosf(fAngeleOld) *  LOCUS_DISTANCE , posOld.y + LOCUS_ADJUST, posOld.z + sinf(fAngeleOld) *  LOCUS_DISTANCE },
			{ posOld.x + cosf(fAngeleOld) * -LOCUS_DISTANCE , posOld.y + LOCUS_ADJUST, posOld.z + sinf(fAngeleOld) * -LOCUS_DISTANCE },
		};
		CLocus::Create(aPos, aPosOld, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	}
	//***********************************************************
}

//******************************
// 描画処理
//******************************
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// 移動量の設定処理
//******************************
void CPlayer::Move(void)
{
	// 移動方向ベクトル
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W)||GetActiveAcceleration())
	{
		// 座標の取得
		D3DXVECTOR3 pos = GetPos();
		// カメラの位置の取得
		D3DXVECTOR3 cameraPos = CGame::GetCamera(m_nPlayerNum)->GetPos();

		// カメラから自分の方向ベクトル
		move = pos - cameraPos;
		// Y方向には移動しない
		move.y = 0.0f;
		// 正規化
		D3DXVec3Normalize(&move, &move);
		// スピードをかける
		move *= GetSpeed();
	}
	
	// 移動量のセット
	SetMoveDist(move);

}

//******************************
// アイテム使用の処理
//******************************
void CPlayer::UseItem(void)
{
	if (GetItem() != CItem::ITEM_NONE && CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		
		switch (GetItem())
		{
		case CItem::ITEM_ACCELERATION:
			// 加速モードにする
			SetActiveAcceleration(true);

			break;
		case CItem::ITEM_ATTACK:
		{
			// 頭から出す
			D3DXVECTOR3 attackPos = GetPos();
			// プレイヤーの後ろ座標の取得
			float fAngle = (-GetRot().y) + D3DXToRadian(90);
			attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
			attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

			// 攻撃生成
			CAttack::Create(attackPos, D3DXVECTOR3(0.0f, GetRot().y + D3DXToRadian(90), 0.0f), GetRankData().nRank ,GetID());

		}
			break;

		case CItem::ITEM_TRAP:
		{
			// けつから出す
			D3DXVECTOR3 trapPos = GetPos();
			// プレイヤーの後ろ座標の取得
			float fAngle = (-GetRot().y) + D3DXToRadian(-90);
			trapPos.x += cosf(fAngle) * TRAP_SET_DISTACE;
			trapPos.z += sinf(fAngle) * TRAP_SET_DISTACE;

			// トラップ生成
			CTrap::Create(trapPos);
		}
			break;
		default:
			break;
		}
		// アイテム情報の初期化
		SetItem(CItem::ITEM_NONE);
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		// 頭から出す
		D3DXVECTOR3 attackPos = GetPos();
		// プレイヤーの後ろ座標の取得
		float fAngle = (-GetRot().y) + D3DXToRadian(90);
		attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
		attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

		// 攻撃生成
		CAttack::Create(attackPos, D3DXVECTOR3(0.0f, GetRot().y + D3DXToRadian(90), 0.0f), GetRankData().nRank, GetID());
	}
}
