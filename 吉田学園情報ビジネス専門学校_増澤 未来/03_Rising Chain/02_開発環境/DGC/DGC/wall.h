//=============================================================================
//
// course�w�b�_ [course.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _WALL_H_
#define _WALL_H_

//*****************************
// �C���N���[�h
//*****************************
#include "model.h"
#include "course.h"

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CWall : public CModel
{
public:


	//�����o�֐�
	CWall();
	~CWall();
	static CWall *Create(CCourse::COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void CollisionPlayer(void);

	// �����o�ϐ�
	static LPD3DXMESH m_pMeshModel[CCourse::COURSE_MAX];	        //���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel[CCourse::COURSE_MAX];    //�}�e���A�����ւ̃|�C���^
	static DWORD m_nNumMatModel[CCourse::COURSE_MAX];	        //�}�e���A�����̐�
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE]; // �e�N�X�`��
	static char *m_pTexPath[CCourse::COURSE_MAX];                // �e�N�X�`���̃p�X
	CCourse::COURSETYPE m_courseType;                            // �R�[�X�^�C�v
};

#endif