//=============================================================================
//
// course�w�b�_ [course.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _COURSE_H_
#define _COURSE_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CCourse : public CModel
{
public:

	// �R�[�X�^�C�v�̗�
	typedef enum
	{
		COURSE_EASY=0,// �C�[�W�[
		COURSE_NORMAL,// �m�[�}��
		COURSE_HARD,  // �n�[�h
		COURSE_MAX,
	}COURSETYPE;

	//�����o�֐�
	CCourse();
	~CCourse();
	static CCourse *Create(COURSETYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ���b�V���̎擾
	LPD3DXMESH GetMesh(void) { return m_pMeshModel[m_courseType]; }
private:
	// �����蔻��
	void CollisionPlayer(void);       // �v���C���[
	void CollisionDestination(void);  // �ړ��ڕW
	void CollisionItem(void);         // �A�C�e��

	// �����o�ϐ�
	static LPD3DXMESH m_pMeshModel[COURSE_MAX];	        //���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel[COURSE_MAX];    //�}�e���A�����ւ̃|�C���^
	static DWORD m_nNumMatModel[COURSE_MAX];	        //�}�e���A�����̐�
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE]; // �e�N�X�`��
	static char *m_pTexPath[COURSE_MAX];                // �e�N�X�`���̃p�X
	COURSETYPE m_courseType;                            // �R�[�X�^�C�v
	int m_nCntInit;                                     // ����������Ă���̃t���[����
};

#endif