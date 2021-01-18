//===================================================
//
//    タイムクラスの処理[time.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "time.h"
#include "number.h"
#include "manager.h"

//==============================
//静的メンバ変数宣言
//==============================
int CTime::m_nTime = 0;
int CTime::m_nPauseCurrentTime = 0;
//==================================
// コンストラクタ
//==================================
CTime::CTime() :CScene(OBJTYPE_UI)
{
	// ナンバーのクリア
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nCurrentTime = timeGetTime();
	m_nPauseTime = 0;
	m_nPauseCurrentTime = 0;
}

//==================================
// デストラクタ
//==================================
CTime::~CTime()
{
}

//==================================
// クリエイト
//==================================
CTime * CTime::Create(void)
{
	// メモリの確保
	CTime *pTime = new CTime;

	// 初期化処理
	pTime->Init();

	// オブジェクトタイプの設定
	pTime->SetPriority(OBJTYPE_UI);

	return pTime;
}

//==================================
// 初期化処理
//==================================
HRESULT CTime::Init(void)
{
	// 最大分ループ
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		// ナンバー生成
		m_apNumber[nCntDigit] = CNumber::Create(0,
			D3DXVECTOR3(SCREEN_WIDTH/2-30*2+(nCntDigit*40), 120.0f, 0.0f),
			D3DXVECTOR3(20, 20, 0),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// タイムの初期化
	m_nTime = 0;
	// ゲーム開始時刻の取得
	m_nCurrentTime = timeGetTime();
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CTime::Uninit(void)
{
	// 最大分ループ
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		if (m_apNumber[nCntDigit] != NULL)
		{
			// 終了処理
			m_apNumber[nCntDigit]->Uninit();
			delete m_apNumber[nCntDigit];
			m_apNumber[nCntDigit] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CTime::Update(void)
{
	// 現在時刻の取得
	int nNowTime = timeGetTime();
	// ゲームの経過時間の計算
	m_nTime = (nNowTime - m_nCurrentTime);

	// 最大分ループ
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		// ナンバーのアップデート
		m_apNumber[nCntDigit]->Update();

		// 各桁の計算
		m_apNumber[nCntDigit]->SetNumber(((m_nTime/10) % (int)(powf(10.0f, (MAX_TIME_NUM - nCntDigit)))) / (float)(powf(10.0, (MAX_TIME_NUM - nCntDigit - 1))));
	}

	// ポーズ画面中のタイムのプラス
	if (!CManager::GetActivePause() && m_nPauseCurrentTime != 0)
	{
		// ポーズ中のタイムの加算
		m_nCurrentTime += (nNowTime - m_nPauseCurrentTime);
		// ポーズタイムの初期化
		m_nPauseCurrentTime = 0;
	}
}

//==================================
// 描画処理
//==================================
void CTime::Draw(void)
{
#ifdef _DEBUG
	// 最大分ループ
	for (int nCntDigit = 0; nCntDigit < MAX_TIME_NUM; nCntDigit++)
	{
		m_apNumber[nCntDigit]->Draw();
	}
#endif // _DEBUG
}
