//=============================================================================
//
// explosion�w�b�_ [explosion.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �N���X��`
//*****************************

// �����N���X
class CExplosion : public CScene3d
{
public:
	//============
	// �����o�֐�
	//============
	CExplosion();
	~CExplosion();
	// static
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col); // �N���X����
	static HRESULT Load(void);													 // �e�N�X�`�����[�h
	static void Unload(void);													 // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	int m_nCntAnim; // �A�j���[�V�����J�E���g
	int m_nAnimX;   // �A�j���[�V����X��
	int m_nAnimY;   // �A�j���[�V����Y��
};

#endif