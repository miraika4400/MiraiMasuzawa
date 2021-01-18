//=============================================================================
//
// start�w�b�_ [start.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _START_H_
#define _START_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"
#include "course.h"

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CStart : public CScene3d
{
public:
	// �����o�֐�
	CStart();
	~CStart();
	static CStart *Create(CCourse::COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	 // �e�N�X�`���ւ̃|�C���^
	int m_nCntAnim;       // �A�j���[�V�����J�E���g
	int m_nAnimX;         // �A�j���[�V����X��
	int m_nAnimY;         // �A�j���[�V����Y��
	CCourse::COURSETYPE m_type; // �R�[�X�̃^�C�v
	DWORD m_currentTime;  // ���Ԃ��v��p
	int m_nTime;          // �e�N�X�`���؂�ւ��^�C��
};
#endif