//===================================================
//
//    エフェクトクラスの処理[effect.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//*****************************
// マクロ定義
//*****************************
#define EFFECT_TEXTURE_PATH "./data/Textures/bullet000.png" // テクスチャのパス
#define EFFECT_LIFE 6 // エフェクトの寿命
#define EFFECT_SUB_SIZE 0.8f   // サイズの減少量
#define EFFECT_SUB_ALPHA 0.1f  // a値の減少量

//*****************************
// 静的メンバ変数宣言
//*****************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL; // テクスチャ

//*****************************
// コンストラクタ
//*****************************
CEffect::CEffect() : CScene3d(OBJTYPE_EFFECT)
{
	m_nLife = EFFECT_LIFE;
}

//*****************************
// デストラクタ
//*****************************
CEffect::~CEffect()
{
}

//*****************************
// クリエイト
//*****************************
CEffect * CEffect::Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,const D3DXCOLOR col, const LPDIRECT3DTEXTURE9 pTexture)
{
	// メモリの確保
	CEffect *pEffect;
	pEffect = new CEffect;

	// 初期化
	pEffect->Init();

	// 各値のセット
	pEffect->m_nLife = EFFECT_LIFE;      // 寿命
	pEffect->SetPos(pos);                // 座標
	pEffect->SetSize(size);              // サイズ
	pEffect->SetColor(col);              // カラー
	pEffect->SetObjType(OBJTYPE_EFFECT); // オブジェクトタイプ 
	pEffect->BindTexture(pTexture);      // テクスチャ割り当て
	pEffect->SetAddMode(true);           // 加算合成
	return pEffect;
}

//*****************************
// テクスチャのロード
//*****************************
HRESULT CEffect::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_PATH, &m_pTexture);
	//D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_PATH_BIRD, &m_pTexture[TEX_BIRD]);
	return S_OK;
}

//*****************************
// テクスチャのアンロード
//*****************************
void CEffect::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************
// 初期化処理
//*****************************
HRESULT CEffect::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

//*****************************
// 終了処理
//*****************************
void CEffect::Uninit(void)
{
	CScene3d::Uninit();
}

//*****************************
// 更新処理
//*****************************
void CEffect::Update(void)
{

	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	// サイズの縮小
	size.x -= EFFECT_SUB_SIZE;
	size.y -= EFFECT_SUB_SIZE;
	// サイスのセット
	SetSize(size);

	// カラーの取得
	D3DXCOLOR col= GetColor();
	// A値の減算
	col.a -= EFFECT_SUB_ALPHA;
	// カラーのセット
	SetColor(col);

	// 寿命を減らす
	m_nLife--;

	// 消滅判定
	if (m_nLife <= 0 || size.x <= 0 /*|| col.a <= 0*/)
	{
		Uninit();
	}
}

//*****************************
// 描画処理
//*****************************
void CEffect::Draw(void)
{
	CScene3d::Draw();
}
