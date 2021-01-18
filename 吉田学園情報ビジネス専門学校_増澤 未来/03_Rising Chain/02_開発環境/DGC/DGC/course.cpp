////////////////////////////////////////////////////
//
//    courseクラスの処理[course.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "course.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "wall.h"
#include "start.h"
#include "collision.h"
#include "camera.h"
#include "destination.h"
#include "item.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH_EASY   "./data/Models/Course_model/Syokyuu_Course.x"     //モデルのパス
#define MODEL_PATH_NORMAL "./data/Models/Course_model/CourseLv2.x"          //モデルのパス
#define MODEL_PATH_HARD   "./data/Models/Course_model/CourseLv2.x"          //モデルのパス
#define ADJUST_HEIGHT 30  // プレイヤーよりちょっと上からレイを出す
#define HOVER_HEIGHT 50   // プレイヤーがどれくらい浮いているか
#define HOVER_HEIGHT_ITEM 150   // アイテムがどれくらい浮いているか

//*****************************
// 静的メンバ変数宣言
//*****************************
LPD3DXMESH   CCourse::m_pMeshModel[CCourse::COURSE_MAX] = {};   	//メッシュ情報へのポインタ
LPD3DXBUFFER CCourse::m_pBuffMatModel[CCourse::COURSE_MAX] = {};	//マテリアル情報へのポインタ
DWORD        CCourse::m_nNumMatModel[CCourse::COURSE_MAX] = {};	    //マテリアル情報の数
LPDIRECT3DTEXTURE9 CCourse::m_apTexture[MAX_TEXTURE] = {}; // テクスチャ

// テクスチャのパスの格納
char *CCourse::m_pTexPath[CCourse::COURSE_MAX] =
{
	MODEL_PATH_EASY,
	MODEL_PATH_NORMAL,
	MODEL_PATH_HARD
};
//******************************
// コンストラクタ
//******************************
CCourse::CCourse():CModel(OBJTYPE_MAP)
{
	m_nCntInit = 0; // 初期化されてからのフレーム数
}

//******************************
// デストラクタ
//******************************
CCourse::~CCourse()
{
}

//******************************
// クリエイト
//******************************
CCourse * CCourse::Create(COURSETYPE type)
{
	// メモリの確保
	CCourse *pMap;
	pMap = new CCourse;
	// タイプの設定
	pMap->m_courseType = type;
	// 初期化
	pMap->Init();

	// 各値の代入・セット
	pMap->SetPriority(OBJTYPE_MAP); // オブジェクトタイプ
	
	return pMap;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CCourse::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// コース数分ループ
	for (int nCntCourse = 0; nCntCourse < COURSE_MAX; nCntCourse++)
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

		if (m_nNumMatModel != 0)
		{
			D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCntCourse]->GetBufferPointer();
			for (int nCnt = 0; nCnt < (int)m_nNumMatModel[nCntCourse]; nCnt++)
			{
				if (pMat[nCnt].pTextureFilename != NULL)
				{
					char cPath[256] = {};
					sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
					// テクスチャの生成
					D3DXCreateTextureFromFile(pDevice, cPath, &m_apTexture[nCnt]);
				}
			}
		}

	}

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CCourse::Unload(void)
{
	for (int nCnt = 0; nCnt < COURSE_MAX; nCnt++)
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
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//マテリアルの破棄
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
		
	}
}


//******************************
// 初期化処理
//******************************
HRESULT CCourse::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	//テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_nNumMatModel[m_courseType]; nCnt++)
	{
		BindTexture(nCnt, m_apTexture[nCnt]);
	}

	// モデル割り当て
	BindModel(m_pMeshModel[m_courseType],m_pBuffMatModel[m_courseType],m_nNumMatModel[m_courseType]);
	
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// チェックポイントの生成
		CCheckPoint::Create(m_courseType);
		// 壁の生成
		CWall::Create(m_courseType);
		// スタートの生成
		CStart::Create(m_courseType);
	}

	m_nCntInit = 0;
	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCourse::Uninit(void)
{
	CModel::Uninit();
}

//******************************
// 更新処理
//******************************
void CCourse::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		m_nCntInit++ ;
		// 当たり判定
		CollisionPlayer();
		//CollisionDestination();
		CollisionItem();
	}
}

//******************************
// 描画処理
//******************************
void CCourse::Draw(void)
{
	//デバイス情報の取得
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//if (m_courseType == COURSE_NORMAL)
	//{
	//
	//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // カリング
	//}

	CModel::Draw();

	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // カリング
}

//******************************
// コース床の当たり判定*プレイヤー
//******************************
void CCourse::CollisionPlayer(void)
{
	// プレイヤー数分ループ
	for (int nCntPlayer = 0; nCntPlayer < CGame::GetPlayerNum(); nCntPlayer++)
	{
		// プレイヤー座標の取得
		D3DXVECTOR3 playerPos = CGame::GetPlayer(nCntPlayer)->GetPos();
		// プレイヤーよりちょっと高い位置から出す
		playerPos.y += ADJUST_HEIGHT;

		BOOL bHit = FALSE;     // レイが当たっているか
		float fDistance = 0.0f;// レイが当たっている距離

		// レイ
		D3DXIntersect(m_pMeshModel[m_courseType],
			&playerPos,
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

			if (fDistance <= HOVER_HEIGHT+ ADJUST_HEIGHT -1)
			{// 床とプレイヤーの距離を一定以上に保つ

				// プレイヤーの座標の更新
				playerPos.y = (playerPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);

				if (CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// 重力フラグが立っているとき
					// 重力を切る
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT+ ADJUST_HEIGHT + 50)
			{// ある程度の範囲だったらコースに吸い付ける

				// プレイヤーの座標の更新
				playerPos.y = (playerPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				CGame::GetPlayer(nCntPlayer)->SetPos(playerPos);

				if (CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// 重力フラグが立っているとき
				    // 重力を切る
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(false);
				}
			}
			else
			{// 床から離れすぎてい時
				if (!CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
				{// 重力フラグが立ってないとき
				 // 重力フラグを建てる
					CGame::GetPlayer(nCntPlayer)->SetActiveGravity(true);
				}
			}
		}
		else
		{// 自分の下に床がない
			if (!CGame::GetPlayer(nCntPlayer)->GetActiveGravity())
			{// 重力フラグが立ってないとき
			    // 重力フラグを建てる
				CGame::GetPlayer(nCntPlayer)->SetActiveGravity(true);
			}
		}

		// 地形の向きとプレイヤーの向きを合わせる

		// ヒットフラグの初期化
		bHit = FALSE;
		// プレイヤーの進行方向＆ちょっと上からレイを飛ばす
		D3DXVECTOR3 rayPos;
		rayPos.x = playerPos.x + ( cosf(CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90))) * 50;
		rayPos.y = playerPos.y + HOVER_HEIGHT + ADJUST_HEIGHT;
		rayPos.z = playerPos.z + (-sinf(CGame::GetPlayer(nCntPlayer)->GetRot().y - D3DXToRadian(90))) * 50;

		// レイ
		D3DXIntersect(m_pMeshModel[m_courseType],
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{
			// レイの当たった座標の算出
			D3DXVECTOR3 hitPos;
			hitPos.x = rayPos.x;
			hitPos.y = rayPos.y - fDistance;
			hitPos.z = rayPos.z;

			// プレイヤーのロットの取得
			D3DXVECTOR3 playerRot = CGame::GetPlayer(nCntPlayer)->GetRot();

			float fRotZ = atan2f(playerPos.y - hitPos.y, 500);
			// カメラの高さの調整
			if (!CGame::GetCamera(nCntPlayer)->GetBackMirror())
			{// バックミラー状態じゃないとき
				CGame::GetCamera(nCntPlayer)->SetPhiDist(atan2f(hitPos.y - playerPos.y, 300)*1.2f + D3DXToRadian(-65));
			}
			else
			{// バックミラー状態の時
				CGame::GetCamera(nCntPlayer)->SetPhiDist(atan2f(hitPos.y - playerPos.y, 300)*-1.2f + D3DXToRadian(-85));
			}
			// プレイヤーの向き
			CGame::GetPlayer(nCntPlayer)->SetRot(D3DXVECTOR3(playerRot.x, playerRot.y, sinf(fRotZ)));
		}
	}
}

#if 0
//******************************
// コース床の当たり判定*目標
//******************************
void CCourse::CollisionDestination(void)
{
	// リストの先頭の取得
	CDestination*pDest = (CDestination*)GetTop(OBJTYPE_DESTINATION);

	while (pDest != NULL)
	{
		// プレイヤー座標の取得
		D3DXVECTOR3 destPos = pDest->GetPos();

		BOOL bHit = FALSE;     // レイが当たっているか
		float fDistance = 0.0f;// レイが当たっている距離

							   // レイ
		D3DXIntersect(m_pMeshModel[m_courseType],
			&destPos,
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

			if (fDistance <= HOVER_HEIGHT - 1)
			{// 床とプレイヤーの距離を一定以上に保つ

			 // プレイヤーの座標の更新
				destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT;
				pDest->SetPos(destPos);

				if (pDest->GetActiveGravity())
				{// 重力フラグが立っているとき
				 // 重力を切る
					pDest->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + 50)
			{// ある程度の範囲だったらコースに吸い付ける

			 // プレイヤーの座標の更新
				destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT;
				pDest->SetPos(destPos);

				if (pDest->GetActiveGravity())
				{// 重力フラグが立っているとき
				 // 重力を切る
					pDest->SetActiveGravity(false);
				}
			}
			else
			{// 床から離れすぎてい時
				if (!pDest->GetActiveGravity())
				{// 重力フラグが立ってないとき
				 // 重力フラグを建てる
					pDest->SetActiveGravity(true);
				}
			}
		}
		else
		{// 自分の下に床がない
			if (pDest->GetActiveGravity())
			{// 重力フラグが立ってないとき
			 // 重力フラグを建てる
				pDest->SetActiveGravity(false);
			}
		}
		// リストの次のポインタにする
		pDest = (CDestination*)pDest->GetNext();
	}
}
#endif

//******************************
// コース床の当たり判定*アイテム
//******************************
void CCourse::CollisionItem(void)
{
	// リストの先頭の取得
	CItem*pItem = (CItem*)GetTop(OBJTYPE_ITEM);

	while (pItem != NULL)
	{
		if (pItem->GetDropFlag())
		{
			// 座標の取得
			D3DXVECTOR3 destPos = pItem->GetPos();

			BOOL bHit = FALSE;     // レイが当たっているか
			float fDistance = 0.0f;// レイが当たっている距離

								   // レイ
			D3DXIntersect(m_pMeshModel[m_courseType],
				&destPos,
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

				if (fDistance <= HOVER_HEIGHT_ITEM - 1 || m_nCntInit <= 200)
				{// 床とプ距離を一定以上に保つ

					// 座標の更新
					destPos.y = (destPos.y - fDistance) + HOVER_HEIGHT_ITEM;
					pItem->SetPos(destPos);

					pItem->SetDropFlag(false);
				}

			}
			else
			{

			}
		}
		// リストの次のポインタにする
		pItem = (CItem*)pItem->GetNext();
	}
}