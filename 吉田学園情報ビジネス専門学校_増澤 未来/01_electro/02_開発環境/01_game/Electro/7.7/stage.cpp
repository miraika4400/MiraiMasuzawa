//===================================================
//
//    ステージクラスの処理[stage.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"
#include "time.h"
#include "boss.h"
#include "warning_logo.h"
#include "game.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define STAGE_COL_CHANGE_COUNT 100    // 色を変えるインターバル
#define STAGE_COL_RATE 0.05f          // 徐々に色を変えるときの係数
// テクスチャのパス
#define STAGE_TEXTURE_PATH1     "./data/Textures/electro_stage001.png"
#define STAGE_TEXTURE_PATH2     "./data/Textures/electro_stage000.png"
#define STAGE_TEXTURE_PATH_10   "./data/Textures/electro_stage000_10.png" 
#define STAGE_TEXTURE_PATH_9    "./data/Textures/electro_stage000_9.png"  
#define STAGE_TEXTURE_PATH_8    "./data/Textures/electro_stage000_8.png"  
#define STAGE_TEXTURE_PATH_7    "./data/Textures/electro_stage000_7.png"  
#define STAGE_TEXTURE_PATH_6    "./data/Textures/electro_stage000_6.png"  
#define STAGE_TEXTURE_PATH_5    "./data/Textures/electro_stage000_5.png"  
#define STAGE_TEXTURE_PATH_4    "./data/Textures/electro_stage000_4.png"  
#define STAGE_TEXTURE_PATH_3    "./data/Textures/electro_stage000_3.png"  
#define STAGE_TEXTURE_PATH_2    "./data/Textures/electro_stage000_2.png"  
#define STAGE_TEXTURE_PATH_1    "./data/Textures/electro_stage000_1.png"  
#define STAGE_TEXTURE_PATH_BOSS "./data/Textures/electro_stage000_boss.png" 

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CStage::m_apTexture[STAGE_MAX] = {};

//==================================
// コンストラクタ
//==================================
CStage::CStage(): CScene(OBJTYPE_MAP)
{
	// 変数のクリア
	memset(&pScene3d, 0, sizeof(pScene3d));
	m_distCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nColChangeCnt = 0;
	pWarning = NULL;
}

//==================================
// デストラクタ
//==================================
CStage::~CStage()
{
}

//==================================
// クリエイト
//==================================
CStage * CStage::Create(void)
{
	//メ モリの確保
	CStage *pStage;
	pStage = new CStage;
	// 初期化
	pStage->Init();

	return pStage;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CStage::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH1, &m_apTexture[STAGE_BACK]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH2, &m_apTexture[STAGE_FRONT]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_10, &m_apTexture[STAGE_10]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_9, &m_apTexture[STAGE_9]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_8, &m_apTexture[STAGE_8]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_7, &m_apTexture[STAGE_7]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_6, &m_apTexture[STAGE_6]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_5, &m_apTexture[STAGE_5]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_4, &m_apTexture[STAGE_4]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_3, &m_apTexture[STAGE_3]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_2, &m_apTexture[STAGE_2]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_1, &m_apTexture[STAGE_1]);
	D3DXCreateTextureFromFile(pDevice, STAGE_TEXTURE_PATH_BOSS, &m_apTexture[STAGE_BOSS]);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CStage::Unload(void)
{
	for (int nCntTex = 0; nCntTex < STAGE_PATS_NUM; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CStage::Init(void)
{
	// ポリゴン生成
	for (int nCntPolygon = 0; nCntPolygon < STAGE_PATS_NUM; nCntPolygon++)
	{
		if (pScene3d[nCntPolygon] == NULL)
		{
			// クリエイト
			pScene3d[nCntPolygon] = CScene3d::Create();
			// 座標の設定
			pScene3d[nCntPolygon]->SetPos(STAGE_POS);
			// サイズの設定
			pScene3d[nCntPolygon]->SetSize(STAGE_SIZE);
			// オブジェクトタイプの設定
			pScene3d[nCntPolygon]->SetObjType(OBJTYPE_MAP);
		}
	}
	//テクスチャの割り当て
	pScene3d[0]->BindTexture(m_apTexture[STAGE_BACK]);
	pScene3d[1]->BindTexture(m_apTexture[STAGE_FRONT]);
	// 加算合成にする
	pScene3d[1]->SetAddMode(true);

	// 目標カラーをランダムに初期化
	m_distCol.r = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.g = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.b = ((float)(rand() % 50) / 100) + 0.5f;
	m_distCol.a = 1.0f;
	// カウントの初期化
	m_nColChangeCnt = 0;

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CStage::Uninit(void)
{
	for (int nCntPolygon = 0; nCntPolygon < STAGE_PATS_NUM; nCntPolygon++)
	{
		if (pScene3d[nCntPolygon] != NULL)
		{
			// 終了処理
			pScene3d[nCntPolygon]->Uninit();
			// クリエイト
			pScene3d[nCntPolygon] = NULL;
		}
	}

	Release();
}

//==================================
// 更新処理
//==================================
void CStage::Update(void)
{
	// カウントを進める
	m_nColChangeCnt++;
	

	
	// 現在のカラーの取得
	D3DXCOLOR col = pScene3d[1]->GetColor();
	// 徐々に色を変える
	col.r += (m_distCol.r - col.r)*STAGE_COL_RATE;
	col.g += (m_distCol.g - col.g)*STAGE_COL_RATE;
	col.b += (m_distCol.b - col.b)*STAGE_COL_RATE;
	col.a = 0.6f;

	// カラーの設定
	pScene3d[1]->SetColor(col);

	
	if (CGame::GetWave() == CGame::WAVE_BOSS)
	{// ボス戦時

	    // テクスチャの切り替え
		pScene3d[1]->BindTexture(m_apTexture[STAGE_BOSS]);
		
		// 一定カウントで
		if (m_nColChangeCnt%STAGE_COL_CHANGE_COUNT == 0)
		{
			// 赤色にする
			m_distCol = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f);
		}
		else if (m_nColChangeCnt % (STAGE_COL_CHANGE_COUNT / 2) == 0)
		{
			// 黄色にする
			m_distCol = D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.6f);
		}

		// カラーの設定
		pScene3d[1]->SetColor(col);
	}
	else
	{// テクスチャの切り替えでボス戦のカウントダウン

		// 一定カウントで
		if (m_nColChangeCnt%STAGE_COL_CHANGE_COUNT == 0)
		{
			// 目標カラーをランダムに初期化
			m_distCol.r = ((float)(rand() % 50) / 100) + 0.5f;
			m_distCol.g = ((float)(rand() % 50) / 100) + 0.5f;
			m_distCol.b = ((float)(rand() % 50) / 100) + 0.5f;
		}

		// 10秒前からテクスチャの切り替えでカウントダウン
		for (int nCntTime = 0; nCntTime < 10; nCntTime++)
		{
			// 経過時間の取得
			int nTime = (CTime::GetTime() / 1000);
			if (nTime == BOSS_SPAWN_SEC - (nCntTime + 1))
			{
				if (pScene3d[1]->GetTexture() != m_apTexture[(BOSS_SPAWN_SEC - nTime) - 1])
				{
					// テクスチャの切り替え
					pScene3d[1]->BindTexture(m_apTexture[(BOSS_SPAWN_SEC - nTime) - 1]);
					// SE再生
					CManager::GetSound()->Play(CSound::LABEL_SE_COUNT);
				}

				// warningロゴを出す
				if (pWarning == NULL)
				{
					pWarning = CWarningLogo::Create();
				}
			}
		}
	}
}

//==================================
// 描画処理
//==================================
void CStage::Draw(void)
{
}
