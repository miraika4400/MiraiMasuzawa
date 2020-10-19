//=============================================================================
//
// stageヘッダ [stage.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _STAGE_H_
#define _STAGE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// 前方宣言
//*****************************
class CScene3d;
class CWarningLogo;

//*****************************
// マクロ定義
//*****************************
#define STAGE_SIZE D3DXVECTOR3(1500.0f,1500.0f,0.0f)    // サイズ
#define STAGE_PATS_NUM 2                               // ステージのパーツ数
#define STAGE_POS D3DXVECTOR3(0.0f,0.0f,0.0f)          // 座標

//*****************************
// クラス定義
//*****************************

//ステージクラス
class CStage : public CScene
{
public:
	typedef enum
	{
		STAGE_1 = 0,
		STAGE_2,
		STAGE_3,
		STAGE_4,
		STAGE_5,
		STAGE_6,
		STAGE_7,
		STAGE_8,
		STAGE_9,
		STAGE_10,
		STAGE_BOSS,
		STAGE_BACK, // ステージの後ろのポリゴン
		STAGE_FRONT,  // ステージの前のポリゴン
		STAGE_MAX
	}STAGE_PARTS;

	//メンバ関数
	CStage();
	~CStage();

	static CStage *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[STAGE_MAX]; //テクスチャ
	CScene3d *pScene3d[STAGE_PATS_NUM]; // ポリゴン
	CWarningLogo*pWarning;         // ワーニングロゴクラス
	D3DXCOLOR m_distCol;           // 色の目標値
	int m_nColChangeCnt;           // 色チェンジのカウント
	
};

#endif