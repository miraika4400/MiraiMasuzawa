//=============================================================================
//
// rank�w�b�_ [rank.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RANK_H_
#define _RANK_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

// �����NUI�N���X
class CRank : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CRank();
	~CRank();

	static CRank *Create(int nPlayerNum); // �N���X����
	static HRESULT Load(void);            // �e�N�X�`�����[�h
	static void Unload(void);             // �e�N�X�`���A�����[�h
	static void RankManage(void);         // ���ʂ̊Ǘ�

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 m_resultPos;       // ���U���g���̍��W
	D3DXVECTOR3 m_resultSize;      // ���U���g���̃T�C�Y

	int m_nCntAnim;                // �A�j���[�V�����J�E���g
	int m_nAnimX;                  // �A�j���[�V����X��
	int m_nAnimY;                  // �A�j���[�V����Y��
	int m_nPlayerNum;              // �v���C���[�ԍ�
};

#endif