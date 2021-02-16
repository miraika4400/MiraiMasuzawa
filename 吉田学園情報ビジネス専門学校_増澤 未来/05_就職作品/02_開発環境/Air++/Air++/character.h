//=============================================================================
//
// character�w�b�_ [character.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model_shader.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �O���錾
//*****************************
class CCollision;

//*****************************
// �N���X��`
//*****************************

// �L�����N�^�[�N���X
class CCharacter : public CModelShader
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	// �\���̒�`
	//==========================
	// ���ʕt���ɗ��p����\����
	typedef struct
	{
		int nCheck; // �`�F�b�N�|�C���g��
		int nLap;   // ���b�v��
		int nRank;  // ����
	}RankData;

	//==========================
	// �����o�֐�
	//==========================
	CCharacter(int nPliority = OBJTYPE_NONE);
	~CCharacter();

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
	
	// �Z�b�^�[�E�Q�b�^�[

	// �ړ���
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	// �ړ��ʂ̖ڕW�l
	void SetMoveDist(D3DXVECTOR3 moveDist) { m_moveDist = moveDist; }
	D3DXVECTOR3 GetMoveDist(void) { return m_moveDist; }
	// �d�̓t���O
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	bool GetActiveGravity(void) { return m_bGravity; }
	// ���ʃf�[�^
	void SetRankData(RankData rankData) { m_rankData = rankData; }
	RankData GetRankData(void) { return m_rankData; }
	// �d�̓t���O
	void SetGoalFlag(bool bBool) { m_bGoal = bBool; }
	bool GetGoalFlag(void) { return m_bGoal; }

	// �R���W�����擾����
	CCollision* GetCollision(void) { return m_pCollision; }

private:
	void Gravity(void);            // �d�͏���
	void Direction(void);          // �����̊Ǘ�����
	void CollisionCharacter(void); // �L�����N�^�[���m�̓����蔻��

	//==========================
	// �����o�ϐ�
	//==========================
	bool         m_bIsPlayer;  // �v���C���[��NPC������p
	D3DXVECTOR3  m_move;       // �ړ���
	D3DXVECTOR3  m_moveDist;   // �ړ��ʂ̖ڕW�l
	D3DXVECTOR3  m_gravityVec; // �d��
	bool         m_bGravity;   // �d�̓t���O
	CCollision * m_pCollision; // �R���W����
	RankData     m_rankData;   // ���ʕt���p�f�[�^
	bool         m_bGoal;      // �S�[�����Ă��邩�̔���p
	D3DXVECTOR3  m_impact;     // �Փ˂����Ƃ��̈ړ���

};
#endif