//=============================================================================
//
// motion�w�b�_ [motion.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model_hierarchy.h"

//*****************************
// �O���錾
//*****************************
class CCollision;

//*****************************
// �}�N����`
//*****************************
#define MAX_KEYFRAME 10       // �L�[�t���[���̍ő吔

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CMotion :CScene
{
public:

	//�����o�֐�
	CMotion();
	~CMotion();
	static CMotion *Create(int nNumParts, const char *pPath, CModel::Model*pModel);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetModel(CModel::Model*pModel) { m_pModel = pModel; }
	void SetActiveAnimation(bool bActive);
	bool GetActiveAnimation(void) { return m_bAnim; }
	int GetKey(void) { return m_nCntKey; }
	int GetFrame(void) { return m_nCntFrame; }
private:
	void Load(const char *pPath);
	// �����o�ϐ�
	CModel::Model *m_pModel; // ���f�����
	D3DXVECTOR3 m_pos[MAX_KEYFRAME][MAX_PARTS_NUM];// ���W�̖ڕW�l
	D3DXVECTOR3 m_rot[MAX_KEYFRAME][MAX_PARTS_NUM];// ��]�̖ڕW�l
	D3DXVECTOR3 m_addPos[MAX_PARTS_NUM];     // ���W�̉��Z�l
	D3DXVECTOR3 m_addRot[MAX_PARTS_NUM];     // ��]�̉��Z�l
	int m_nNumKey;                 // �L�[�t���[���̐�
	int m_nNumParts;               // �p�[�c��
	int m_nNumFrame[MAX_KEYFRAME]; // �t���[����
	int m_nCntKey;                 // �L�[�J�E���g
	int m_nCntFrame;               // �t���[���J�E���g
	bool m_bAnim;                  // �A�j���[�V�������Ă��邩
	bool m_bLoop;                  // ���[�v���邩
};

#endif