//=============================================================================
//
// scene�w�b�_ [scene.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _SCENE_H_
#define _SCENE_H_

//�C���N���[�h
#include "main.h"

//***********************************
//�}�N����`
//***********************************
#define MAX_SCENE 1280   // �|���S���̍ő吔
#define PRIORITY_NUM OBJTYPE_MAX
//=============================
//�N���X��`
//=============================

//�V�[���N���X
class CScene
{
public:
	// �I�u�W�F�N�g�^�C�v��
	// �`�揇
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,          // �w�i
		OBJTYPE_EFFECT,      // �G�t�F�N�g
		OBJTYPE_PLAYER,      // �v���C���[
		OBJTYPE_DESTINATION, // �ڕW
		OBJTYPE_PARTICLE,    // �p�[�e�B�N��
		OBJTYPE_CHECKPOINT,  // �`�F�b�N�|�C���g
		OBJTYPE_MAP,         // �}�b�v
		OBJTYPE_ACCELFLOOR,  // ������
		OBJTYPE_ITEM,        // �A�C�e��
		OBJTYPE_MOTION,      // ���[�V����
		OBJTYPE_COLLISION,   // �R���W����
		OBJTYPE_UI,          // UI
		OBJTYPE_MAX,         // �^�C�v�̍ő吔
	}OBJTYPE;

	// �����o�֐�
	CScene(int nPriority = OBJTYPE_NONE);
	virtual ~CScene();
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumAll(void) { return m_nIDAll; }

	void SetPriority(const int nPriority);
	OBJTYPE GetType(void) { return m_type; }
	int GetID(void) { return m_nNumID; } // �z��ԍ��̎擾

	static CScene*GetTop(int nIndex) { return m_pTop[nIndex]; }
	static CScene*GetCur(int nIndex) { return m_pCur[nIndex]; }
	CScene*GetNext(void) { return m_pNext; }
	CScene*GetPrev(void) { return m_pPrev; }

	// �������z�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

protected:
	void Release(void);
private:
	// �����o�ϐ�
	void ReConnection(void);
	//static CScene *m_apScene[MAX_SCENE]; // �|���S���Ǘ��p
	static int m_nIDAll;                   // ���������|���S����
	int m_nNumID;                          // ���������|���S���̔z��
	OBJTYPE m_type;                        // �I�u�W�F�N�g�^�C�v
	int m_nPriority;                       // �v���C�I���e�B
	bool m_bReleasFlag;                    // �폜�t���O

	// ���X�g�\��
	static CScene * m_pTop[PRIORITY_NUM];  // �擪
	static CScene * m_pCur[PRIORITY_NUM];  // �Ō�
	CScene * m_pNext;                      // �� 
	CScene * m_pPrev;                      // �O
};

#endif