//=============================================================================
//
// モデル処理 [model.h]
// Author : 増澤未来
//
//=============================================================================
#ifndef _MODEL_HIERARCHY_H_
#define _MODEL_HIERARCHY_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"
#include "model.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_PARTS_NUM 15 // パーツの最大数

//=============================================================================
// 階層構造付きモデルクラス
//=============================================================================
class CModelHierarchy : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CModelHierarchy(int nPliority = OBJTYPE_NONE);
	~CModelHierarchy();

	static void LoadModels(char * pPath, CModel::Model *model, int * pNumModel); // モデルの読み込み
	void LoadHierarchy(CModel::Model *model, char*pPath);                        // 階層構造の読み込み

	HRESULT Init(void); // 純関数用
	HRESULT Init(int nNumParts, CModel::Model *model, char*pPath); // 初期化
		void Uninit(void);                                         // 終了
	void Update(void);                                             // 更新
	void Draw(void);                                               // 描画 

	// 各変数の取得・セット

	// モデルデータ
	void SetModelData(CModel::Model *pModel);
	CModel::Model * GetModelData(void) { return &m_model[0]; }
	// パーツ数
	void SetPartsNum(int nNumParts) { m_nNumParts = nNumParts; }
	int GetPartsNum(void) { return m_nNumParts; }
	// 座標
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void)const { return m_pos; }
	// サイズ
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void)const { return m_size; }
	// 回転
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void)const { return m_rot; }
private:
	//============
	// メンバ変数
	//============
	CModel::Model m_model[MAX_PARTS_NUM];      // モデル情報
	D3DXVECTOR3 m_pos;                         // 座標
	D3DXVECTOR3 m_rot;                         // 回転
	D3DXVECTOR3 m_size;                        // サイズ
	D3DXMATERIAL m_defMat[MAX_PARTS_NUM][128]; // マテリアル保管用
	int m_nNumParts;                           // パーツ数
};
#endif 