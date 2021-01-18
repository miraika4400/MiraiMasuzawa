//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���V ����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �N���X�錾
//*****************************************************************************

// �T�E���h�N���X
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
	// �v���g�^�C�v�錾
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �p�����[�^�\���̒�`
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	// �����o�ϐ�
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2 *m_pXAudio2;								    // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice ;			    // �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
};
#endif