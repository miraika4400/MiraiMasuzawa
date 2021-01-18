//=============================================================================
//
// cursor�w�b�_ [cursor.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CURSOR_H_
#define _CURSOR_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// �N���X��`
//*****************************

// �J�[�\���N���X
class CCursor : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CCursor();
	~CCursor();
	static CCursor *Create(const D3DXVECTOR3 pos); // �N���X����
	static HRESULT Load(void);                     // �e�N�X�`�����[�h
	static void Unload(void);                      // �e�N�X�`���A�����[�h

	HRESULT Init(void);         // ������
	void Uninit(void);          // �I��
	void Update(void);          // �X�V
	void Draw(void);            // �`��
	D3DXVECTOR3 GetPos3d(void); // �J�[�\���̃��[���h���W�̎擾

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���̃|�C���^
	float m_fRotAngle;                    // ��]�p�x
	int   m_nCntAnim;                     // �A�j���[�V�����J�E���g
	int   m_nAnimX;                       // �A�j���[�V����X��
	int   m_nAnimY;                       // �A�j���[�V����Y��
};

#endif