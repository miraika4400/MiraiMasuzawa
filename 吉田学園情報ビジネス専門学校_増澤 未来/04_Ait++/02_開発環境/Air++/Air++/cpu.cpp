//=============================================================================
//
// playerヘッダ [player.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "cpu.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "collision.h"
#include "cpu_point.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH_STAR "./data/Models/cpu_star.x" // モデルのパス*ワープスター
#define MODEL_PATH_RUIN "./data/Models/cpu_ruin.x" // モデルのパス*ルインスター
#define SHADER_PATH     "./data/HLSL/Shader.fx"    // HLSLファイルのパス
#define CPU_DIST_RADIUS 150.0f                     // 目標の地点半径
#define DIST_RAND_AMPLITUDE 800                    // 目標地点設定の乱数の振幅
#define CPU_LEVEL_MAX 10                           // CPU強さレベルの上限

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model  CCpu::m_aModel[CCpu::CPU_TYPE_MAX] = {};  //メッシュ情報へのポインタ
CScene::Shader CCpu::m_shader = {};                      // シェーダー構造体

//******************************
// コンストラクタ
//******************************
CCpu::CCpu() :CCharacter(OBJTYPE_CPU)
{
	m_nCpuPointNum = 0;        // コースデータ配列管理用
	m_cpuType = CPU_TYPE_STAR; // CPUの種類
	m_pDIstCollision = NULL;   // 移動目標値当たり判定
	m_nLevel = 0;              // CPUの強さ
}

//******************************
// デストラクタ
//******************************
CCpu::~CCpu()
{
}

//******************************
// クリエイト
//******************************
CCpu * CCpu::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CPUTYPE cpuType , int nLevel)
{
	// メモリの確保
	CCpu *pCpu;
	pCpu = new CCpu;

	// タイプの設定
	pCpu->m_cpuType = cpuType; // タイプ
	
	// 初期化
	pCpu->Init();

	// 各値の代入・セット
	pCpu->SetPos(pos);       // 座標のセット
	pCpu->SetRot(rot);       // 座標のセット
	pCpu->m_nLevel = nLevel; // レベルのセット 

	// レベルの上限、下限の範囲
	pCpu->m_nLevel = min(max(pCpu->m_nLevel, 1), CPU_LEVEL_MAX);

	return pCpu;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CCpu::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	// ワープスター
	D3DXLoadMeshFromX(MODEL_PATH_STAR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_aModel[CPU_TYPE_STAR].pBuffMat, NULL, &m_aModel[CPU_TYPE_STAR].nNumMat, &m_aModel[CPU_TYPE_STAR].pMesh);
	// ルインスター
	D3DXLoadMeshFromX(MODEL_PATH_RUIN, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_aModel[CPU_TYPE_RUIN].pBuffMat, NULL, &m_aModel[CPU_TYPE_RUIN].nNumMat, &m_aModel[CPU_TYPE_RUIN].pMesh);

	for (int nCnt = 0; nCnt < CPU_TYPE_MAX; nCnt++)
	{
		// テクスチャ読み込み
		if (m_aModel[nCnt].nNumMat != 0)
		{
			D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_aModel[nCnt].pBuffMat->GetBufferPointer();
			for (int nCnt = 0; nCnt < (int)m_aModel[nCnt].nNumMat; nCnt++)
			{
				if (pMat[nCnt].pTextureFilename != NULL)
				{
					char cPath[64] = {};
					sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
					// テクスチャの生成
					D3DXCreateTextureFromFile(pDevice, cPath, &m_aModel[nCnt].apTexture[nCnt]);
				}
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
void CCpu::Unload(void)
{
	for (int nCnt = 0; nCnt < CPU_TYPE_MAX; nCnt++)
	{
		//メッシュの破棄
		if (m_aModel[nCnt].pMesh != NULL)
		{
			m_aModel[nCnt].pMesh->Release();
			m_aModel[nCnt].pMesh = NULL;
		}

		//マテリアルの破棄
		if (m_aModel[nCnt].pBuffMat != NULL)
		{
			m_aModel[nCnt].pBuffMat->Release();
			m_aModel[nCnt].pBuffMat = NULL;
		}
	}
}

//******************************
// 初期化処理
//******************************
HRESULT CCpu::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_aModel[m_cpuType].pMesh, m_aModel[m_cpuType].pBuffMat, m_aModel[m_cpuType].nNumMat);

	//テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_aModel[m_cpuType].nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_aModel[m_cpuType].apTexture[nCnt]);
	}

	// シェーダーのセット
	SetShader(m_shader);

	// コースデータ配列管理用変数の初期化
	m_nCpuPointNum = 0;
	
	
	// 初期目標座標の設定

	// コースデータの取得
	CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum);
	// 次の目標地点
	D3DXVECTOR3 distPos = pointData.pos;
	// 次の目標地点の角度
	float fDistAngle = pointData.fAngle;

	// 乱数生成
	int nRand = (rand() % DIST_RAND_AMPLITUDE) - DIST_RAND_AMPLITUDE / 2;

	// 目標地点の設定
	distPos.x = distPos.x + cosf(-fDistAngle)*nRand;
	distPos.y = distPos.y;
	distPos.z = distPos.z + sinf(-fDistAngle)*nRand;

	// 目標地点
	m_pDIstCollision = CCollision::CreateSphere(CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum).pos, CPU_DIST_RADIUS);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCpu::Uninit(void)
{
	CCharacter::Uninit();
}

//******************************
// 更新処理
//******************************
void CCpu::Update(void)
{
	if (!GetGoalFlag())
	{
		// 移動量の設定
		Move();

		// 目標地点の管理
		DistManager();
	}
	else
	{
		// 移動量のセット
		SetMoveDist(VEC3_ZERO);
	}

	// キャラクタークラスの更新処理
	CCharacter::Update();

	// コースの当たり判定
	CGame::GetCourse()->CollisionCharacter((CCharacter*)this);
}

//******************************
// 描画処理
//******************************
void CCpu::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// 移動量の設定処理
//******************************
void CCpu::Move(void)
{
	// 移動量の取得
	D3DXVECTOR3 move = GetMove();
	
	// 現在地点から目標地点へのベクトルの取得
	D3DXVECTOR3 moveDist = m_pDIstCollision->GetPos() - GetPos();
	// Y軸は移動しない
	moveDist.y = 0;
	// 正規化
	D3DXVec3Normalize(&moveDist, &moveDist);
	// スピードをかける
	moveDist *= GetSpeed();
	
	// 移動量のセット
	SetMoveDist(moveDist);

}

//******************************
// 目標地点の管理
//******************************
void CCpu::DistManager(void)
{
	// 目標地点にぶつかったとき
	if (CCollision::CollisionSphere(GetCollision(), m_pDIstCollision))
	{
		// 目標地点インデックスを進める
		m_nCpuPointNum++;

		// 目標値点数を超えたらインデックスを0にする
		if (m_nCpuPointNum >= CGame::GetCpuPoint()->GetPointNum())
		{
			m_nCpuPointNum = 0;
		}

		CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nCpuPointNum);
		// 次の目標地点
		D3DXVECTOR3 distPos = pointData.pos;
		// 次の目標地点の角度
		float fDistAngle = pointData.fAngle;

		int nRand = 0;

		switch (pointData.inType)
		{
		case CCpuPoint::POINT_TYPE_NORMAL:

			// 乱数生成
			nRand = (rand() % DIST_RAND_AMPLITUDE) - DIST_RAND_AMPLITUDE / 2;

			break;

		case CCpuPoint::POINT_TYPE_IN_LEFT:
		{
			// 乱数生成
			nRand = (rand() % DIST_RAND_AMPLITUDE / 2);

			// レベルで走るコースの調整
			int nAdjust = DIST_RAND_AMPLITUDE / 2 - (((DIST_RAND_AMPLITUDE / 2) / CPU_LEVEL_MAX) *m_nLevel);

			nRand -= nAdjust;
		}
			break;

		case CCpuPoint::POINT_TYPE_IN_RIGHT:
		{
			// 乱数生成
			nRand = -(rand() % DIST_RAND_AMPLITUDE / 2);
			// レベルで走るコースの調整
			int nAdjust = DIST_RAND_AMPLITUDE / 2 - (((DIST_RAND_AMPLITUDE / 2) / CPU_LEVEL_MAX) *m_nLevel);
			nRand += nAdjust;
		}
			break;

		default:
			break;
		}

		// 目標地点の設定
		distPos.x = distPos.x + cosf(-fDistAngle)*nRand;
		distPos.y = distPos.y;
		distPos.z = distPos.z + sinf(-fDistAngle)*nRand;

		// 次の目標地点に移動
		m_pDIstCollision->SetPos(distPos);
	}
}
