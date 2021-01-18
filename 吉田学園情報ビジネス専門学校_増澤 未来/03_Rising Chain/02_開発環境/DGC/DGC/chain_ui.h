//=============================================================================
//
// chain_uiヘッダ [chain_ui.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CHAIN_UI_H_
#define _CHAIN_UI_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// 前方宣言
//*****************************
class CNumber;
class CPolygon;

//*****************************
// マクロ定義
//*****************************
#define MAX_CHAIN_DIGIT 3

//*****************************
// クラス定義
//*****************************

// チェイン数UIロゴクラス
class CChainUi : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CChainUi();
	~CChainUi();

	static CChainUi *Create(int nPlayerNum); // クラス生成
	static HRESULT Load(void);               // テクスチャロード
	static void Unload(void);                // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;       // テクスチャ
	CNumber*m_apNumber[MAX_CHAIN_DIGIT]; // チェイン数字
	int m_nPlayerNum;                    // プレイヤー番号
	int m_nDegit;                        // 表示桁数
	D3DXVECTOR3 m_pos;                   // 中心座標
	int m_nChain;                        // チェイン数
	CPolygon*m_pPolygon;                 // 文字画像
	D3DXVECTOR3 m_resultPos;       // リザルト時の座標
	D3DXVECTOR3 m_resultSize;      // リザルト時のサイズ
};

#endif