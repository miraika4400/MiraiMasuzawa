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

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH     "./data/Models/player.x" // モデルのパス
#define SHADER_PATH    "./data/HLSL/Shader.fx"  // HLSLファイルのパス
#define PLAYER_SPEED 30.0f                      // 移動スピード
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

	// キャラクタークラスの更新処理
	CCharacter::Update();
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

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
	{
		// 座標の取得
		D3DXVECTOR3 pos = GetPos();
		// カメラの位置の取得
		D3DXVECTOR3 cameraPos = CGame::GetCamera()->GetPos();

		// カメラから自分の方向ベクトル
		move = pos - cameraPos;
		// Y方向には移動しない
		move.y = 0.0f;
		// 正規化
		D3DXVec3Normalize(&move, &move);
		// スピードをかける
		move *= PLAYER_SPEED;
	}
	
	// 移動量のセット
	SetMoveDist(move);

}
