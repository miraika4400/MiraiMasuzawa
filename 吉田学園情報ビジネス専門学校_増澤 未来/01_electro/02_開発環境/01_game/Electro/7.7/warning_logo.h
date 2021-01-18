//=============================================================================
//
// warning_logo�w�b�_ [warning_logo.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _WARNING_LOGO_H_
#define _WARNING_LOGO_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//=============================
// �O���錾
//=============================
class CPolygon;

//*****************************
//�N���X��`
//*****************************

//���[�j���O���S�N���X
class CWarningLogo : public CScene3d
{
public:
	//============
	// �����o�֐�
	//============
	CWarningLogo();
	~CWarningLogo();
	// static
	static CWarningLogo *Create(void); // �N���X����
	static HRESULT Load(void);         // �e�N�X�`�����[�h
	static void Unload(void);          // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���ւ̃|�C���^
};

#endif