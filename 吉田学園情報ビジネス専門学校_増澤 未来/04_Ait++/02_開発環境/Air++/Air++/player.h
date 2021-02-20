//=============================================================================
//
// player�w�b�_ [player.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "character.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �O���錾
//*****************************

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CPlayer : public CCharacter
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	//�����o�֐�
	//==========================
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum); // �N���X����
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	// �v���C���[�ԍ��̎擾����
	int GetPlayerNum(void) { return m_nPlayerNum; }
private:
	void Move(void);    // �ړ�����
	void UseItem(void); // �A�C�e���g�p�̏���

	//==========================
	//�����o�ϐ�
	//==========================
	static Model m_model;   // ���f���\����
	static Shader m_shader; // �V�F�[�_�[�\����
	int m_nPlayerNum;       // �v���C���[ID
};
#endif