////////////////////////////////////////////////////
//
//    courseクラスの処理[course.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"
#include "destination.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH_EASY   "./data/Models/Course_model/Syokyuu_Course_Wall.x"    //モデルのパス
#define MODEL_PATH_NORMAL "./data/Models/Course_model/CourseLv2_Wall.x"    //モデルのパス
#define MODEL_PATH_HARD   "./data/Models/Course_model/"    //モデルのパス
#define HOVER_HEIGHT 50

//*****************************
// 静的メンバ変数宣言
//*****************************
LPD3DXMESH   CWall::m_pMeshModel[CCourse::COURSE_MAX] = {};   	//メッシュ情報へのポインタ
LPD3DXBUFFER CWall::m_pBuffMatModel[CCourse::COURSE_MAX] = {};	//マテリアル情報へのポインタ
DWORD        CWall::m_nNumMatModel[CCourse::COURSE_MAX] = {};	    //マテリアル情報の数
																	// テクスチャのパスの格納
char *CWall::m_pTexPath[CCourse::COURSE_MAX] =
{
	MODEL_PATH_EASY,
	MODEL_PATH_NORMAL,
	MODEL_PATH_HARD
};
//******************************
// コンストラクタ
//******************************
CWall::CWall() :CModel(OBJTYPE_MAP)
{
}

//******************************
// デストラクタ
//******************************
CWall::~CWall()
{
}

//******************************
// クリエイト
//******************************
CWall * CWall::Create(CCourse::COURSETYPE type)
{
	// メモリの確保
	CWall *pWall;
	pWall = new CWall;
	// タイプの設定
	pWall->m_courseType = type;
	// 初期化
	pWall->Init();

	// 各値の代入・セット
	pWall->SetPriority(OBJTYPE_MAP); // オブジェクトタイプ

	return pWall;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CWall::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// コース数分ループ
	for (int nCntCourse = 0; nCntCourse < CCourse::COURSE_MAX; nCntCourse++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(m_pTexPath[nCntCourse],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMatModel[nCntCourse],
			NULL,
			&m_nNumMatModel[nCntCourse],
			&m_pMeshModel[nCntCourse]);

	}

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CWall::Unload(void)
{
	for (int nCnt = 0; nCnt < CCourse::COURSE_MAX; nCnt++)
	{
		//メッシュの破棄
		if (m_pMeshModel[nCnt] != NULL)
		{
			m_pMeshModel[nCnt]->Release();
			m_pMeshModel[nCnt] = NULL;
		}

		//マテリアルの破棄
		if (m_pBuffMatModel[nCnt] != NULL)
		{
			m_pBuffMatModel[nCnt]->Release();
			m_pBuffMatModel[nCnt] = NULL;
		}
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CWall::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_pMeshModel[m_courseType], m_pBuffMatModel[m_courseType], m_nNumMatModel[m_courseType]);
	// チェックポイントの生成
	CCheckPoint::Create(m_courseType);
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CWall::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// 更新処理
//******************************
void CWall::Update(void)
{
	// 当たり判定
	CollisionPlayer();
}

//******************************
// 描画処理
//******************************
void CWall::Draw(void)
{
#ifdef _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ワイヤーフレームで描画
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//　描画
	CModel::Draw();
	// ワイヤーフレームをもどす
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

#endif // _DEBUG
}

//******************************
// コース床の当たり判定
//******************************
void CWall::CollisionPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// プレイヤー座標の取得
		D3DXVECTOR3 playerPos = CGame::GetPlayer(nCntPlayer)->GetPos();

		BOOL bHit = FALSE;     // レイが当たっているか
		float fDistance = 0.0f;// レイが当たっている距離

		// レイを出す方向数
		const int c_nRayNum = 24;
		for (int nCntRay = 0; nCntRay < c_nRayNum; nCntRay++)
		{
			// レイを出す方向の設定
			float fDir = D3DXToRadian(360 / c_nRayNum);
			D3DXVECTOR3 rayDir;
			rayDir.x = cosf(fDir*nCntRay);
			rayDir.y = 0;
			rayDir.z = sinf(fDir*nCntRay);

			// レイ
			D3DXIntersect(m_pMeshModel[m_courseType],
				&playerPos,
				&rayDir,
				&bHit,
				NULL,
				NULL,
				NULL,
				&fDistance,
				NULL,
				NULL);

			// 当たっているとき
			if(bHit)
			{
				// プレイヤーの半径
				float fRadPlayer = (float)CGame::GetPlayer(nCntPlayer)->GetMaxSpeed()*2.5f;//CGame::GetPlayer(nCntPlayer)->GetCollision()->GetCollisionRadius();

				if (fDistance < fRadPlayer)
				{// レイの長さがプレイヤーの半径以下の時
					playerPos.x = (playerPos.x + cosf(fDir*nCntRay) * fDistance) - (cosf(fDir*nCntRay) * fRadPlayer);
					playerPos.z = (playerPos.z + sinf(fDir*nCntRay) * fDistance) - (sinf(fDir*nCntRay) * fRadPlayer);

					// プレイヤーの位置の設定
					CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);
					// プレイヤーのコリジョンの位置の設定
					CGame::GetPlayer(nCntPlayer)->GetCollision()->SetPos(playerPos);

					// 移動目標の位置設定

					// 移動目標の取得
					CDestination*pDest=(CDestination*)GetTop(OBJTYPE_DESTINATION);
					while (pDest != NULL)
					{
						if (pDest->GetPlayerNum() == CGame::GetPlayer(nCntPlayer)->GetPlayerNum())
						{// プレイヤー番号が一致しているとき
							D3DXVECTOR3 destPos = pDest->GetPos();
							destPos.x = playerPos.x + cosf(-CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90)) * -pDest->GetDistancePlayer();
							destPos.z = playerPos.z + sinf(-CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90)) * -pDest->GetDistancePlayer();
							pDest->SetPos(destPos);
							break;
						}
						pDest = (CDestination*)pDest->GetNext();
					}
					//break;
				}
			}
		}
	}
}
