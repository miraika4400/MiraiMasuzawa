//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 増澤 未来
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス宣言
//*****************************************************************************

// サウンドクラス
class CSound
{
public:
	CSound();
	~CSound();

	typedef enum
	{
		SOUND_LABEL_BGM_TITLE,
		SOUND_LABEL_BGM_GAME,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_SE_EVOLUTION,
		SOUND_LABEL_SE_ACCELERATION,
		SOUND_LABEL_SE_ITEM,
		SOUND_LABEL_SE_COUNT1,
		SOUND_LABEL_SE_COUNT2,
		SOUND_LABEL_SE_ENTER,
		SOUND_LABEL_SE_HIT,
		SOUND_LABEL_MAX,
	}LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);
private:
	// プロトタイプ宣言
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	// メンバ変数
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2 *m_pXAudio2;								    // XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice ;			    // マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};
#endif