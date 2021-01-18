//=============================================================================
//
// Accelfloor�w�b�_ [accelfloor.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ACCELFLOOR_H_
#define _ACCELFLOOR_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************
#define HOVER_RING 200 // �����O���ǂꂾ���n�ʂ��畂������

//*****************************
// �O���錾
//*****************************
class CCollision;
class CScene3d;

//*****************************
// �N���X��`
//*****************************

// �������N���X
class CAccelFloor : public CModel
{
public:
	//==========================
	// ��
	//==========================


	//==========================
	//�����o�֐�
	//==========================
	CAccelFloor();
	~CAccelFloor();
	static CAccelFloor *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot); // �N���X����
	static HRESULT Load(void);                                                // ���[�h
	static void Unload(void);                                                 // �A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	// �����t���O�̎擾�A�Z�b�g
	bool GetAdjustFlag(void) { return m_bAdjust; }
	void SetAdjustFlag(bool bAdjust) { m_bAdjust = bAdjust; }

private:
	void CollisionPlayer(void); // �v���C���[�Ƃ̓����蔻��
	void AdjustPos(void);       // �������Ɉʒu����

	//==========================
	//�����o�ϐ�
	//==========================
	static Model m_model;      // ���f���\����
	static LPDIRECT3DTEXTURE9 m_pTexture; // ���̃e�N�X�`��
	CCollision*m_pCollision;       // �����蔻��
	CScene3d*m_pScene3d;           // ���̃p�l���|���S��
	float m_fScroll;               // ���̃e�N�X�`�����炷�p
	bool m_bAdjust;                // �ʒu�����t���O
	bool m_bHit[MAX_PLAYER_NUM];   // �q�b�g���̃t���O
	int m_nCntHit[MAX_PLAYER_NUM]; // �q�b�g�t���O�������J�E���g
};
#endif