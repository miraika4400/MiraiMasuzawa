//=============================================================================
//
// MiniMap�w�b�_ [minimap.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

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
//�}�N����`
//*****************************
#define MAX_MINIMAP_POLYGON 256

//*****************************
//�N���X��`
//*****************************

//�~�j�}�b�v�N���X
class CMiniMap : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CMiniMap();
	~CMiniMap();

	static CMiniMap *Create(void); // �N���X����
	static HRESULT Load(void);     // �e�N�X�`�����[�h
	static void Unload(void);      // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^
	static D3DXVECTOR3 m_stagePos;               // �X�e�[�W��POS
	CPolygon* m_apPolygon[MAX_MINIMAP_POLYGON];  // �~�j�}�b�v�`��p�̃|���S��
};

#endif