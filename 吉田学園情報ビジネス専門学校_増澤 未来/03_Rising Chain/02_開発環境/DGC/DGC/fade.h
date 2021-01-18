//=============================================================================
//
// fade�w�b�_ [fade.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _FADE_H_
#define _FADE_H_
//***********************************
//�C���N���[�h
//***********************************
#include "main.h"
#include "manager.h"

//***********************************
// �O���錾
//***********************************
class CPolygon;

//=============================
//�N���X��`
//=============================

//�V�[���N���X
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX,
	}FADE;

	CFade();
	~CFade();
	static CFade*Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CManager::MODE mode);
private:
	// �����o�ϐ�
	CPolygon*m_pPolygon; // �t�F�[�h�|���S��
	D3DXCOLOR m_col;     // �J���[
	CManager::MODE m_modeNext;
	FADE m_fade;
};

#endif