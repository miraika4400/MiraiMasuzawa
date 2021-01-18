//=============================================================================
//
// bomb_ui�w�b�_ [bomb_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BOMB_UI_H_
#define _BOMB_UI_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//�O���錾
//*****************************
class CPolygon;

//*****************************
//�N���X��`
//*****************************

//�{��UI�N���X
class CBombUi : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CBombUi();
	~CBombUi();
	// static
	static CBombUi *Create(void); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���ւ̃|�C���^
	CPolygon* m_pPolygon;                 // ���C�t�`��p�̃|���S��

};

#endif