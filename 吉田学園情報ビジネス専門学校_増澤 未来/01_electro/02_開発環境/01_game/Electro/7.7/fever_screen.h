//=============================================================================
//
// FeverScreen�w�b�_ [fever_screen.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _FEVERSCREEN_H_
#define _FEVERSCREEN_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

// �t�B�[�o�[���S�N���X
class CFeverScreen : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CFeverScreen();
	~CFeverScreen();
	
	static CFeverScreen *Create(void); // �N���X����
	static HRESULT Load(void);       // �e�N�X�`�����[�h
	static void Unload(void);        // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^

	int m_nCntAnim;                // �A�j���[�V�����J�E���g
	int m_nAnimX;                  // �A�j���[�V����X��
	int m_nAnimY;                  // �A�j���[�V����Y��
	int m_nCntColor;               // �F��ς���J�E���g
	D3DXCOLOR m_distColor;         // �J���[�ύX���̖ڕW�J���[
};

#endif