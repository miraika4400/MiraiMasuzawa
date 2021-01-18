//=============================================================================
//
// bg�w�b�_ [bg.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BG_H_
#define _BG_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//  �O���錾
//*****************************
class CScene3d;

//*****************************
// �}�N����`
//*****************************
#define BG_TEXTURE_NUM 3 // BG�Ŏg���e�N�X�`����
#define BG_PARTS_NUM 3   // BG�Ŏg���|���S����

//*****************************
// �N���X��`
//*****************************

// �w�i�N���X
class CBg : public CScene
{
public:
	//�����o�֐�
	CBg();
	~CBg();

	static CBg *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture[BG_TEXTURE_NUM];
	CScene3d *m_apScene3d[BG_PARTS_NUM];

	float m_afTexMoveU[BG_PARTS_NUM]; // �e�N�X�`�������炷�p(U/��)
	float m_afTexMoveV[BG_PARTS_NUM]; // �e�N�X�`�������炷�p(V/�c)
};

#endif