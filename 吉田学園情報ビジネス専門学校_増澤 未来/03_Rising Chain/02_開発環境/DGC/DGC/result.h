//=============================================================================
//
// result�w�b�_ [result.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//=============================
// �O���錾
//=============================
class CPolygon;

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CResult : public CScene
{
public:
	// �����o�֐�
	CResult();
	~CResult();
	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^
	CPolygon *m_pPolygon;

};

#endif