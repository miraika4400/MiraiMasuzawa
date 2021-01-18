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
#include "model_hierarchy.h"
#include "game.h"
//*****************************
// �}�N����`
//*****************************
#define MAX_EVOLUTION 4        // �i���̒i�K��

//*****************************
// �O���錾
//*****************************
class CCollision;
class CDestination;
class CGauge;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CPlayer : public CModelHierarchy
{
public:

	//�����o�֐�
	CPlayer();
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos,int nPlayerNum);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void Goal(void); // �S�[������
	bool GetGoalFlag(void) { return m_bGoal; } // �S�[���t���O�̎擾
	
	CCollision*GetCollision(void) { return m_pCollision; }
	CDestination*GetDestination(void) { return m_pDest; }
	// �ړ��ʂ̎擾�E�Z�b�g
	/*D3DXVECTOR3  GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }*/
	// �d�̓t���O�̎擾�E�Z�b�g
	bool GetActiveGravity(void) { return m_bGravity; }
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	// �`�F�b�N�|�C���g���̎擾�E�Z�b�g�E���Z
	int GetCheckNum(void) { return m_nNumCheckpoint; }
	void SetCheckNum(int nNum) { m_nNumCheckpoint = nNum; }
	void AddCheckNum(void) { m_nNumCheckpoint++; }
	// �A�C�e�����̎擾�E�Z�b�g�E���Z
	int  GetItemNum(void) { return m_nCollectItem; }
	void SetItemNum(int nNum) { m_nCollectItem = nNum; }
	void AdtItemNum(void) { m_nCollectItem++; }
	// �`�F�C���|�C���g���̎擾�E�Z�b�g�E���Z
	int  GetChainNum(void) { return m_nChain; }
	void SetChainNum(int nNum) { m_nChain = nNum; }
	void AddChainNum(void);// { m_nChain++; }
	// �`�F�C���|�C���g���̎擾�E�Z�b�g�E���Z
	int GetMaxChain(void) { return m_nMaxChain; }
	void  SetMaxChain(int nMaxChain) { m_nMaxChain = nMaxChain; }
	// �ړ��t���O�̎擾�E�Z�b�g
	bool GetMoveFlag(void) { return m_bMove; }
	void SetMoveFlag(bool bMove) { m_bMove = bMove; }
	// �h���t�g�t���O�̎擾�E�Z�b�g
	bool GetDriftLeft(void) { return m_bDriftLeft; }
	void SetDriftLeft(bool bDrift) { m_bDriftLeft = bDrift; }
	bool GetDriftRight(void) { return m_bDriftRight; }
	void SetDriftRight(bool bDrift) { m_bDriftRight = bDrift; }
	// �q�b�g��Ԃ̎擾�E�Z�b�g
	bool  GetHitFrag(void) { return m_bHit; }
	void SetHitFrag(bool bHit) { m_bHit = bHit; }
	// �ō����x�̎擾�E�Z�b�g�E���Z
	float  GetMaxSpeed(void) { return m_fMaxSpeed; }
	void SetMaxSpeed(float fSpeed) { m_fMaxSpeed = fSpeed; }
	// �����x�̎擾�E�Z�b�g�E���Z
	float  GetMoveRate(void) { return m_fMoveRate; }
	void SetMoveRate(float fRate) { m_fMoveRate = fRate; }
	// ������Ԃ̎擾�E�Z�b�g
	bool  GetAccelerationFrag(void) { return m_bAccelerationFlag; }
	void SetAccelerationFrag(bool bAccele); //{ m_bAccelerationFlag = bAccele; }
	// ���ʂ̎擾�E�Z�b�g
	int GetRank(void) { return m_nRank; }
	void SetRank(int nRank) { m_nRank = nRank; }
	// �v���C���[�ԍ��̎擾
	int GetPlayerNum(void) { return m_nPlayerNum; }
	// �A�C�e���ɂ��������Ƃ��̏���
	void HitItem(bool bSafe);

private:
	void MoveControll(void);   // �ړ�
	void Gravity(void);        // �d��
	void Evolution(void);      // �i��
	void Drift(void);          // �h���t�g
	void Acceleration(void);   // ��������
	void CollisionPlayer(void);// �v���C���[���m�̓����蔻��

	// �����o�ϐ�
	static CModel::Model m_model[MAX_PLAYER_NUM][MAX_PARTS_NUM];    // ���f���\����
	static int m_nNumModel[MAX_PLAYER_NUM];                         // ���f���̐�
	static char m_acModelText[MAX_PLAYER_NUM][64];   // ���f���̃e�L�X�g�f�[�^
	static int m_nNumEvolutionParts[MAX_EVOLUTION];  // �i�����Ƃ̃p�[�c��
	static int m_nSpeedData[MAX_EVOLUTION];          // �i�����Ƃ̍ō����x
	static float m_fRateData[MAX_EVOLUTION];         // �i�����Ƃ̉����x
	static int m_nEvoData[MAX_EVOLUTION];            // �e�i���܂ł̒l
	//D3DXVECTOR3 m_move;        // �ړ���
	int m_nPlayerNum;          // �v���C���[�ԍ�
	D3DXVECTOR3 m_gravityVec;  // �d��
	bool m_bGravity;           // �d�̓t���O
	CCollision*m_pCollision;   // �R���W�����N���X�̃|�C���^
	int m_nNumCheckpoint;      // �`�F�b�N�|�C���g��
	bool m_bGoal;              // �S�[���t���O
	bool m_bMove;              // �ړ��ł����ԉ�����p
	int m_nChain;              // �`�F�C����
	int m_nMaxChain;           // �ő�`�F�C����
	int m_nCollectItem;        // ��������A�C�e����
	int m_nNumEvolution;       // �i����
	float m_fEvoGauge;         // �i���Q�[�W�̒l
	CDestination *m_pDest;     // �ړ��ړ��ڕW
	bool m_bDriftLeft;         // �h���t�g��
	bool m_bDriftRight;        // �h���t�g�E
	bool m_bHit;               // �q�b�g��ԃt���O
	int m_nCntHit;             // �q�b�g��ԉ������̃J�E���g
	float m_fMaxSpeed;         // �ő呬�x
	bool m_bAccelerationFlag;  // �����t���O
	int m_nCntAcceleration;    // �������̃J�E���g
	float m_fAcceleration;     // ������Ԃ̒l
	float m_fMoveRate;         // �����̒l
	int m_nRank;               // ����
	CGauge*m_pGauge;           // �Q�[�W�N���X�|�C���^
};

#endif