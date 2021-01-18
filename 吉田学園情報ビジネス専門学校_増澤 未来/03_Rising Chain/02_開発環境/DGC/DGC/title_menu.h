//=============================================================================
//
//  [titlemenu.h]
// Author : Masuzawa Mirai
//
//=============================================================================

#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_
//***************************************************************************************
// �C���N���[�h�t�@�C��
//***************************************************************************************
#include "scene.h"

class CPolygon;

//***************************************************************************************
// �N���X��`
//***************************************************************************************

// �|�[�Y�N���X
class CTitleMenu : public CScene
{
public:
	//============
	// ��ԗ�
	//============
	// ���j���[�̍\���p�[�c
	typedef enum
	{
		//BACK = 0, // ���j���[�w�i
		TWO,      // ��l
		THREE,    // �O�l
		FOUR,     // �l�l
		PARTS_MAX // �p�[�c�̐�
	}MENU_PARTS;

	//============
	// �����o�֐�
	//============
	CTitleMenu();
	~CTitleMenu();

	// static
	static CTitleMenu* Create(void); // �N���X����
	static HRESULT Load(void);       // �e�N�X�`�����[�h
	static void Unload(void);        // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTS_MAX]; // �e�N�X�`��
	CPolygon* m_pPolygon[PARTS_MAX];                  // �|���S��
	int m_nMenu;                                      // ���j���[�I��p
	bool m_bMove;                                     // �ړ��t���O
};

#endif // !_TITEL_H_