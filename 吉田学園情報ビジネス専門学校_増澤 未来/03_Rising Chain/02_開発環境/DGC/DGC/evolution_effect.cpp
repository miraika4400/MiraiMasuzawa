////////////////////////////////////////////////////
//
//    EvoEffectクラスの処理[EvoEffect.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "evolution_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "destination.h"
//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH         "./data/Models/Evolution.x"	// 赤モデルのパス
#define INIT_SIZE D3DXVECTOR3(0.4f,0.4f,0.4f) // 初期サイズ
#define DIST_SIZE D3DXVECTOR3(1.0f,1.0f,1.0f) // 目標サイズ
#define SIZE_RATE 0.03f                       // サイズ加算係数
#define LIFE      60                          // 寿命

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CEvoEffect::m_model = {};   	//メッシュ情報へのポインタ


//******************************
// コンストラクタ
//******************************
CEvoEffect::CEvoEffect() :CModel(OBJTYPE_EFFECT)
{
	// 変数のクリア
	m_nPlayerNum = -1;                             // ドロップ時どのプレイヤーが落としたか
	m_nLife = 0;                                   // 寿命
	m_fAlfa = 1.0f;                                // アルファ値
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);        // カラー
	m_distCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // カラー目標値
}

//******************************
// デストラクタ
//******************************
CEvoEffect::~CEvoEffect()
{
}

//******************************
// クリエイト
//******************************
CEvoEffect * CEvoEffect::Create(int nPlayerNum)
{
	// メモリの確保
	CEvoEffect *pEvoEffect;
	pEvoEffect = new CEvoEffect;
	// プレイヤー番号
	pEvoEffect->m_nPlayerNum = nPlayerNum;
	// 初期化
	pEvoEffect->Init();
	// オブジェクトタイプの設定
	pEvoEffect->SetPriority(OBJTYPE_UI);
	
	return pEvoEffect;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CEvoEffect::Load(void)
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

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CEvoEffect::Unload(void)
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
HRESULT CEvoEffect::Init(void)
{
	if (FAILED(CModel::Init()))
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

	SetSize(INIT_SIZE);

	m_nLife = 0;
	m_fAlfa = 1.0f;    // アルファ値
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);        // カラー

	switch (m_nPlayerNum)
	{
	case 0:
		m_distCol = D3DXCOLOR(0.7f, 0.0f, 0.0f, 0.0f); // 赤
		break;
	case 1:
		m_distCol = D3DXCOLOR(0.0f, 0.0f, 0.7f, 0.0f); // 青
		break;
	case 2:
		m_distCol = D3DXCOLOR(0.7f, 0.7f, 0.0f, 0.0f); // 黄
		break;
	case 3:
		m_distCol = D3DXCOLOR(0.0f, 0.7f, 0.0f, 0.0f); // 緑
		break;
	default:
		break;
	}
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CEvoEffect::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// 更新処理
//******************************
void CEvoEffect::Update(void)
{
	// プレイヤー情報の取得
	CPlayer*pPlayer = CGame::GetPlayer(m_nPlayerNum);
	if (pPlayer != NULL)
	{
		SetPos(pPlayer->GetPos());
	}

	// サイズを大きくする
	D3DXVECTOR3 size = GetSize();
	size += (DIST_SIZE - size)*SIZE_RATE;
	SetSize(size);

	if (size.x >= DIST_SIZE.x - 0.01)
	{
		Uninit();
	}

	// 回転させる
	D3DXVECTOR3 addRot = D3DXVECTOR3(0.0f, 0.05f, 0.0f);
	SetRot(GetRot() + addRot);

	// 寿命の処理
	m_nLife++;

	if (m_nLife >= LIFE/3)
	{
		// 色を目標値に近づける
		m_col.r += (m_distCol.r - m_col.r)*0.05;
		m_col.g += (m_distCol.g - m_col.g)*0.05;
		m_col.b += (m_distCol.b - m_col.b)*0.05;
		m_col.a += (m_distCol.a - m_col.a)*0.05;
		m_fAlfa -= 0.05f;
	}
	if (m_nLife >= LIFE)
	{
		Uninit();
	}
}

//******************************
// 描画処理
//******************************
void CEvoEffect::Draw(void)
{
	// すべてのパーツを赤色にする

	D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse = m_col;
	}

	CModel::Draw();
}