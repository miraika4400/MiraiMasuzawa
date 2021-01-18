////////////////////////////////////////////////////
//
//    Itemクラスの処理[Item.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "accelfloor.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "destination.h"
#include "scene3d.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH   "./data/Models/item/Ring.x"     // モデルのパス
#define TEXTURE_PATH "./data/Textures/Speed_Up.png"  //テクスチャのパス
#define RADIUS       550                             // 半径
#define SIZE D3DXVECTOR3(1.8f,1.8f,1.8f)
#define PANEL_SIZE   D3DXVECTOR3(300.0f,0.0,-600.0f) // 床のサイズ
#define SCROLL_SPEED 0.01f                           // テクスチャのスクロール速度
#define HIT_FLAG_INIT_COUNT 30                       // ヒットフラグ初期化
#define NUM_CHAIN 5                                  // チェイン数

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CAccelFloor::m_model = {};         //メッシュ情報へのポインタ
LPDIRECT3DTEXTURE9 CAccelFloor::m_pTexture = NULL; // テクスチャ

//******************************
// コンストラクタ
//******************************
CAccelFloor::CAccelFloor() :CModel(OBJTYPE_ACCELFLOOR)
{
	// 変数のクリア
	m_pCollision = NULL; // 当たり判定
	m_pScene3d = NULL;   // 床ポリゴン
	m_fScroll = 0.0f;    // 床テクスチャスクロール用変数
	m_bAdjust = true;    // 位置調整フラグ
	memset(&m_bHit, 0, sizeof(m_bHit)); // ヒット時のフラグ
	memset(&m_nCntHit, 0, sizeof(m_nCntHit)); // ヒットフラグカウント
}

//******************************
// デストラクタ
//******************************
CAccelFloor::~CAccelFloor()
{
}

//******************************
// クリエイト
//******************************
CAccelFloor * CAccelFloor::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// メモリの確保
	CAccelFloor *pItem;
	pItem = new CAccelFloor;

	// 初期化
	pItem->Init();

	// 各値の代入・セット
	pItem->SetPriority(OBJTYPE_MAP); // オブジェクトタイプ
	pItem->SetPos(pos);              // 座標のセット
	pItem->SetRot(rot);              // 回転のセット

	// 当たり判定の生成
	pItem->m_pCollision = CCollision::CreateSphere(pos, RADIUS);
	return pItem;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CAccelFloor::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();


	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.nNumMat,
		&m_model.pMesh);

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

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CAccelFloor::Unload(void)
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

	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CAccelFloor::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);
	// テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}
	SetSize(SIZE);
	// 床のパネルの生成
	m_pScene3d = CScene3d::Create(D3DXVECTOR3(GetPos().x, GetPos().y - HOVER_RING, GetPos().z), PANEL_SIZE);
	// プライオリティの設定
	m_pScene3d->SetPriority(OBJTYPE_MAP);
	// テクスチャの割り当て
	m_pScene3d->BindTexture(m_pTexture);

	// 床テクスチャスクロール用変数の初期化
	m_fScroll = 0.0f;
	// 位置調整フラグの初期化
	m_bAdjust = true;
	// ヒット時のフラグの初期化
	memset(&m_bHit, 0, sizeof(m_bHit));
	// ヒットフラグカウントの初期化
	memset(&m_nCntHit, 0, sizeof(m_nCntHit));

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CAccelFloor::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModel::Uninit();
}

//******************************
// 更新処理
//******************************
void CAccelFloor::Update(void)
{
	if (m_bAdjust)
	{
		// 高さの調整
		AdjustPos();
		// 床とサークルの回転を合わせる
		D3DXVECTOR3 rot = GetRot();
		m_pScene3d->SetRot(D3DXVECTOR3(0.0f, rot.y, 0.0f));

		m_bAdjust = false;
		
	}
	CollisionPlayer();
	// 回転させる
	D3DXVECTOR3 addRot = D3DXVECTOR3(0.0f, 0.0f, 0.03f);
	SetRot(GetRot() + addRot);

	if (m_pScene3d != NULL)
	{
		// 床のテクスチャをスクロールさせる
		m_fScroll += SCROLL_SPEED;
		D3DXVECTOR2 uv[NUM_VERTEX];
		// UV座標の設定
		uv[0] = D3DXVECTOR2(0.0f, 0.0f + m_fScroll);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f + m_fScroll);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f + m_fScroll);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f + m_fScroll);

		// UV座標のセット
		m_pScene3d->SetTextureUV(uv);
		// 座標
		m_pScene3d->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y - HOVER_RING, GetPos().z));
	}
}

//******************************
// 描画処理
//******************************
void CAccelFloor::Draw(void)
{
	CModel::Draw();
}

//******************************
// プレイヤーとの当たり判定
//******************************
void CAccelFloor::CollisionPlayer(void)
{
	// プレイヤー情報の取得
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);

	while (pPlayer != NULL)
	{
		if (!pPlayer->GetGoalFlag())
		{// ゴール状態じゃないとき

			// プレイヤー番号の取得
			int nPlayerNum = pPlayer->GetPlayerNum();

			if (!m_bHit[nPlayerNum])
			{// ヒットフラグが立ってないとき

				// 当たり判定
				if (CCollision::CollisionSphere(m_pCollision, pPlayer->GetCollision()))
				{// 当たってた時
					// 加速状態にする
					pPlayer->SetAccelerationFrag(true);
					for (int nCnt = 0; nCnt < NUM_CHAIN; nCnt++)
					{
						pPlayer->AddChainNum();
					}
					// ヒットフラグ
					m_bHit[nPlayerNum] = true;
					// カウントの初期化
					m_nCntHit[nPlayerNum] = 0;
				}
			}
			else
			{// ヒットフラグが立っているとき

				// カウントを進める
				m_nCntHit[nPlayerNum]++;
				// 一定のカウントに到達したら
				if (m_nCntHit[nPlayerNum] >= HIT_FLAG_INIT_COUNT)
				{
					// カウントの初期化
					m_nCntHit[nPlayerNum] = 0;
					// ヒットフラグの初期化
					m_bHit[nPlayerNum] = false;
				}
			}
		}
		// ネクストのポインタに更新
		pPlayer = (CPlayer*)pPlayer->GetNext();
	}
	int num = 0;
}

//******************************
// 位置調整
//******************************
void CAccelFloor::AdjustPos(void)
{
	// コース情報の所得
	CCourse*pCourse = CGame::GetCourse();

	BOOL bHit = FALSE;
	float fDistance = 0.0f;
	// NULLチェック
	if (pCourse != NULL)
	{
		// レイ
		D3DXIntersect(pCourse->GetMesh(),
			&GetPos(),
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{// レイが当たっていたら
			D3DXVECTOR3 pos = GetPos();
			// プレイヤーの座標の更新
			pos.y = (pos.y - fDistance) + HOVER_RING;

			SetPos(pos);
			m_pCollision->SetPos(pos);
		}
	}
}
