//=============================================================================
//
// TitleLogo�w�b�_ [TitleLogo.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

//�^�C�g�����S�N���X
class CTitleLogo : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CTitleLogo();
	~CTitleLogo();

	static CTitleLogo *Create(void); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^
	int m_nCntAnim;       // �A�j���[�V�����J�E���g
	int m_nAnimX;         // �A�j���[�V����X��
	int m_nAnimY;         // �A�j���[�V����Y��
	int m_nCntSetAnim;    // �����_���Ȋ��o�ŃA�j���[�V�������Đ������邽�߂̃J�E���g
	bool m_bAnim;         // �A�j���[�V�����Đ���������p
};

#endif