//=============================================================================
//
// effect�w�b�_ [effect.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �N���X��`
//*****************************

// �G�t�F�N�g�N���X
class CEffect : public CScene3d
{
public:
	//============
	// �����o�֐�
	//============
	CEffect();
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR col,const LPDIRECT3DTEXTURE9 pTexture); // �N���X����
	static HRESULT Load(void); // �e�N�X�`�����[�h
	static void Unload(void);  // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	int m_nLife;                          // ����
};

#endif