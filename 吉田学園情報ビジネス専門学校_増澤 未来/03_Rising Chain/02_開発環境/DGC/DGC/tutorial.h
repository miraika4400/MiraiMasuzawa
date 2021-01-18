//=============================================================================
//
// tutorial�w�b�_ [tutorial.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// �O���錾
//*****************************
class CPolygon;

//*****************************
// �}�N����`
//*****************************
#define TUTORIAL_NUM 1

//*****************************
//�N���X��`
//*****************************

// �`���[�g���A���N���X
class CTutorial : public CScene
{
public:
	// �����o�֐�
	CTutorial();
	~CTutorial();
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture[TUTORIAL_NUM];        // �e�N�X�`���ւ̃|�C���^
	CPolygon *m_pPolygon;
	int m_nNumTutorial;

};

#endif