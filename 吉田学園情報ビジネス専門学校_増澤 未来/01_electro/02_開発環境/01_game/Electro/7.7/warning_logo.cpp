////////////////////////////////////////////////////
//
//    リザルトクラスの処理[result.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "warning_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "stage.h"
#include "sound.h"

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CWarningLogo::m_pTexture = NULL;

//**********************************
// マクロ定義
//**********************************
#define  TEXTURE_PATH "./data/Textures/logo_warning3.png" // テクスチャ

//=============================
// コンストラクタ
//=============================
CWarningLogo::CWarningLogo():CScene3d(OBJTYPE_WARNING)
{
}

//=============================
// デストラクタ
//=============================
CWarningLogo::~CWarningLogo()
{
}

//=============================
// クリエイト
//=============================
CWarningLogo * CWarningLogo::Create(void)
{
	// メモリの確保
	CWarningLogo *pWarning = new CWarningLogo;
	// 初期化
	pWarning->Init();
	return pWarning;
}
//==================================
// テクスチャのロード
//==================================
HRESULT CWarningLogo::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CWarningLogo::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}
//=============================
// 初期化処理
//=============================
HRESULT CWarningLogo::Init(void)
{
	// 初期化
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// 座標の設定
	SetPos(D3DXVECTOR3(STAGE_POS.x, STAGE_POS.x , 0.0f));
	// サイズの設定
	SetSize(D3DXVECTOR3(STAGE_SIZE.x + 100, STAGE_SIZE.y + 100, 0.0f));
	//　テクスチャの設定
	BindTexture(m_pTexture);
	// オブジェくタイプの設定
	SetPriority(OBJTYPE_WARNING);
	// SEの再生
	CManager::GetSound()->Play(CSound::LABEL_SE_WARNING);
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CWarningLogo::Uninit(void)
{
	// SEの停止
	CManager::GetSound()->Stop(CSound::LABEL_SE_WARNING);
	CScene3d::Uninit();
}


//=============================
// 更新処理
//=============================
void CWarningLogo::Update(void)
{
	SetAngle(GetAngle() + 0.1f);
}


//=============================
// 描画処理
//=============================
void CWarningLogo::Draw(void)
{
	CScene3d::Draw();
}
