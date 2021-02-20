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
#include "item.h"

//*****************************
// �}�N����`
//*****************************
#define CHARACTER_RADIUS 100.0f // �v���C���[���a(�����蔻��)

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
	// �A�C�e�����
	void SetItem(CItem::ITEM_TYPE item) { m_item = item; }
	CItem::ITEM_TYPE GetItem(void) { return m_item; }
	// �����t���O
	void SetActiveAcceleration(bool bBool);
	bool GetActiveAcceleration(void) { return m_bAcceleration; }
	// �ړ����x
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	float GetSpeed(void) { return m_fSpeed; }
	// �X�^���t���O
	void SetStan(bool bBool);
	bool GetStan(void) { return m_bStan; }
	
	// �v���C���[�t���O�擾����
	bool GetIsPlayer(void) { return m_bIsPlayer; }
	// �R���W�����擾����
	CCollision* GetCollision(void) { return m_pCollision; }

private:
	void Gravity(void);            // �d�͏���
	void Direction(void);          // �����̊Ǘ�����
	virtual void CollisionCharacter(void); // �L�����N�^�[���m�̓����蔻��
	void SpeedManager(void);       // �X�s�[�h�Ǘ�����

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
	CItem::ITEM_TYPE m_item;   // �������Ă���A�C�e�����
	bool         m_bAcceleration; // �����t���O
	float        m_fSpeed;     // �ړ����x
	int          m_nCntAccleration; // �������J�E���g
	bool         m_bStan;      // �X�^������
	int          m_nCntStan;   // �X�^�����̃J�E���g
	int          m_nCntGravity;// �d�̓t���O���o���Ă���ԃJ�E���g����
};
#endif