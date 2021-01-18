//=============================================================================
//
// bomb�w�b�_ [bomb.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �N���X��`
//*****************************

// �{���N���X
class CBomb : public CScene3d
{
public:

	//============
	// �����o�֐�
	//============
	CBomb();
	~CBomb();

	// staic
	static CBomb *Create(const D3DXVECTOR3 pos, const float fMaxsize , const D3DCOLOR col); // �N���X����
	static HRESULT Load(void);                                                              // �e�N�X�`�����[�h
	static void Unload(void);                                                               // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	int   m_nCntAnim;                     // �A�j���[�V�����J�E���g
	int   m_nAnimX;                       // �A�j���[�V����X��
	int   m_nAnimY;                       // �A�j���[�V����Y��
	bool  m_bHitBoss;                     // �{�X�ɑ��i�q�b�g���Ȃ��悤�Ƀt���O
	float m_fMaxsize;                     // �ő�T�C�Y

};

#endif