//=============================================================================
//
// checkpoint�w�b�_ [checkpoint.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// �}�N����`
//*****************************
#define MAX_CHECKPOINT_NUM 256

//*****************************
// �O���錾
//*****************************
class CCollision;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CCheckPoint : public CScene
{
public:
	
	//�����o�֐�
	CCheckPoint();
	~CCheckPoint();
	static CCheckPoint *Create(CCourse::COURSETYPE type);
	static CCheckPoint *GetCheckPoint(void) { return m_pCheckPoint; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	CCollision*GetCollision(int nInex) { return m_pCollision[nInex]; }
	int GetCheckPointNum(void) { return m_nNumPoint; }

private:
	void SaveCheckpoint(void);
	void LoadCheckpoint(CCourse::COURSETYPE type);
	// �����o�ϐ�
	static CCheckPoint * m_pCheckPoint;
	CCourse::COURSETYPE m_type;
	int m_nNumPoint; // �`�F�b�N�|�C���g�̐�
	CCollision*m_pCollision[MAX_CHECKPOINT_NUM];    // �R���W�����N���X�̃|�C���^
	D3DXVECTOR3 m_pointPos[MAX_CHECKPOINT_NUM];   // �`�F�b�N�|�C���g�̍��W
};

#endif