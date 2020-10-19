////////////////////////////////////////////////////
//
//    explosionクラスの処理[explosion.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "explosion.h"
#include "manager.h"
#include "renderer.h"

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//*****************************
// マクロ定義
//*****************************
#define EXPLOSION_ANIM_SPEED 4       // アニメーション速度
#define EXPLOSION_MAX_ANIMATION_X 7  // アニメーション数　横
#define EXPLOSION_MAX_ANIMATION_Y 1  // アニメーション数　縦

#define EXPLOSION_TEXTURE_PATH "./data/Textures/explosion003.png" // テクスチャのパス

//******************************
// コンストラクタ
//******************************
CExplosion::CExplosion() :CScene3d(OBJTYPE_EXPLOSION)
{
	// 変数のクリア
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//******************************
// デストラクタ
//******************************
CExplosion::~CExplosion()
{
}

//******************************
// クリエイト
//******************************
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// メモリの確保
	CExplosion *pExplosion;
	pExplosion = new CExplosion;

	// 初期化
	pExplosion->Init();

	// 各値のセット
	pExplosion->SetPos(pos);   // 座標
	pExplosion->SetSize(size); // サイズ
	pExplosion->SetColor(col); // 色
	pExplosion->SetObjType(OBJTYPE_EXPLOSION); // オブジェクトタイプ

	return pExplosion;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CExplosion::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CExplosion::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// 初期化処理
//******************************
HRESULT CExplosion::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// UV座標の初期化
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV座標の設定

	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / EXPLOSION_MAX_ANIMATION_X;
	float fv = 1.0f / EXPLOSION_MAX_ANIMATION_Y;
	
	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY - fv);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	
	// UV座標セット
	SetTextureUV(uv);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CExplosion::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// 更新処理
//******************************
void CExplosion::Update(void)
{
	// アニメーションカウントを進める
	m_nCntAnim++;

	if (m_nCntAnim % EXPLOSION_ANIM_SPEED == 0)
	{
		// アニメーションX軸の加算
		m_nAnimX++;

		if (m_nAnimX >= EXPLOSION_MAX_ANIMATION_X)
		{
			// アニメーションX軸の初期化
			Uninit();
		}
		else
		{
			// UV座標の設定
			D3DXVECTOR2 uv[NUM_VERTEX];

			float fu = 1.0f / EXPLOSION_MAX_ANIMATION_X;
			float fv = 1.0f / EXPLOSION_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY - fv);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			// UV座標セット
			SetTextureUV(uv);
		}


	}
}

//******************************
// 描画処理
//******************************
void CExplosion::Draw(void)
{
	CScene3d::Draw();
}
