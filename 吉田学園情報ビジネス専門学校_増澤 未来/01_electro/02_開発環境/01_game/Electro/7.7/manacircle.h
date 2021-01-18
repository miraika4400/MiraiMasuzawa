//=============================================================================
//
// manacircre�w�b�_ [manacircre.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _MANACIRCRE_H_
#define _MANACIRCRE_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �}�N����`
//*****************************
#define CIRCLE_MAX 6  // �T�[�N���̍ő吔 

//*****************************
// �N���X��`
//*****************************

// �T�[�N���N���X
class CManaCircle : public CScene3d
{
public:
	//============
	// �����o�֐�
	//============
	CManaCircle();
	~CManaCircle();
	// static
	static CManaCircle *Create(const D3DXVECTOR3 pos);                          // �N���X����
	static HRESULT Load(void);                                                  // �e�N�X�`�����[�h
	static void Unload(void);                                                   // �e�N�X�`���A�����[�h
	static void SpawnManager(void);                                             // �T�[�N���̂킫�̊Ǘ�
	static void SetCircleNum(int nNum) { m_nNumCircle = nNum; }                 // �T�[�N�����̃Z�b�g
	static void SetCircleMax(int nNum) { m_nMaxCircle = nNum; }                 // �T�[�N���̍ő吔�̃Z�b�g
	static CManaCircle *GetCircle(int nIndex) { return m_apCircle[nIndex]; }    // �������Ă�T�[�N���|�C���^�̎擾
	
	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
	
private:
	static D3DXVECTOR3 GetRandCirclePos(void); // �T�[�N�������p�̃����_���ȍ��W�̎擾
	void AnimationManager(void);               // �e�N�X�`���A�j���[�V�����̊Ǘ�
	void CollisionCircle(void);                // �����蔻��̊Ǘ�
	void LostCircle(void);                     // �T�[�N���̍폜

	//============
	// �����o�֐�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;       // �e�N�X�`��
	static CManaCircle *m_apCircle[CIRCLE_MAX]; // �������Ă���T�[�N���|�C���^�̊i�[�p
	static int m_nNumCircle;                    // �o�����Ă���T�[�N���̐�
	static int m_nMaxCircle;                    // �o������T�[�N���̍ő吔
	int  m_nCntRecovery; // �T�[�N���ɓ����Ă���Ԃ̃J�E���g
	int  m_nIndex;       // �z��ԍ�
	bool bInCircle;      // �T�[�N���ɓ����Ă��邩
	int  m_nCntAnim;     // �A�j���[�V�����J�E���g
	int  m_nAnimX;       // �A�j���[�V����X��
	int  m_nAnimY;       // �A�j���[�V����Y��
};

#endif