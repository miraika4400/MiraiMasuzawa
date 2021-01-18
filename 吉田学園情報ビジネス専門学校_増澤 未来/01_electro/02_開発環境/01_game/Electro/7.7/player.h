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
#include "scene3d.h"

//*****************************
// �O���錾
//*****************************
class CGauge;
class CFeverLogo;
class CFeverScreen;

//*****************************
// �}�N����`
//*****************************
#define PLAYER_MAX_HP    8.0f   // �ő�̗�
#define PLAYER_MAX_MP    50.0f  // �ő�MP
#define IN_FEVER_NUM    50.0f   // �t�B�[�o�[�̓���l
#define PLAYER_GAUGE_NUM 3      // �g���Q�[�W��
// �Q�[�W���
// (HP)
#define PLAYER_LIFE_WIDTH 300.0f // HP�o�[�̉����̍ő�
#define PLAYER_LIFE_HEGHT 15     // HP�o�[�̏c��
#define PLAYER_LIFE_POS_X 150.0f // X�ʒu
#define PLAYER_LIFE_POS_Y 618.0f // Y�ʒu
// (MP)
#define PLAYER_MP_WIDTH 300.0f // HP�o�[�̉����̍ő�
#define PLAYER_MP_HEGHT 12     // HP�o�[�̏c��
#define PLAYER_MP_POS_X 150.0f // X�ʒu
#define PLAYER_MP_POS_Y 645.0f // Y�ʒu
// (�t�B�[�o�[)
#define FEVER_WIDTH 600.0f // HP�o�[�̉����̍ő�
#define FEVER_HEGHT 10     // HP�o�[�̏c��
#define FEVER_POS_X SCREEN_WIDTH/2-600.0f/2
#define FEVER_POS_Y 680.0f // Y�ʒu

//*****************************
// �N���X��`
//*****************************

//�v���C���[�N���X
class CPlayer : public CScene3d
{
public:

	//============
	// ��ԗ�
	//============
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,     
		STATE_SPAWN,    
		STATE_GAMEOVER,
		STATE_MAX,      
	}STATE;

	//============
	// �����o�֐�
	//============
	CPlayer();
	~CPlayer();

	// static
	static CPlayer *Create(const D3DXVECTOR3 pos); // �N���X����
	static HRESULT Load(void);                     // �e�N�X�`�����[�h
	static void Unload(void);                      // �e�N�X�`���A�����[�h
	static bool IsAlive(void) { return m_bAlive;}  // �����Ă��邩�̎擾

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
	void HitAttack(int nDamage);                 // �U���q�b�g���̏���
	void RecoveryLife(const int nRecovery);      // �̗͂̉�
	void RecoveryMp(float fRecoveryMp);          // MP�̉�
	void ProgressFever(float fAdd);              // �t�B�[�o�[�l�𑝂₷

    // �ړ��ʂ̎擾�E�Z�b�g
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetState(const D3DXVECTOR3 move) { m_move = move; }
	// �X�e�[�g�̎擾�E�Z�b�g
	STATE GetState(void) { return m_state;}
	void SetState(const STATE state) { m_state = state; }
	// �{���t���O�̃Z�b�g�E�擾
	void SetBomb(const bool bBomb) { m_bBomb = bBomb; }
	bool HaveBomb(void) { return m_bBomb; }
	// �c�@���̂̃Z�b�g�E�擾
	void SetRemain(const int nRemain) { m_nRemain = nRemain; }
	int GetRemain(void) { return m_nRemain; }
	//MP�̂̃Z�b�g�E�擾
	void SetMp(const float fMp) { m_fMp = fMp; }
	float GetMp(void) { return m_fMp; }
	// HP�̃Z�b�g�E�擾�E��
	void SetLife(const float fLife) { m_fLife = fLife; }
	float GetLife(void) { return m_fLife; }
	// �{���t���O�̃Z�b�g�E�擾
	void SetFever(const bool bfever) { m_bFever = bfever; }
	bool GetFever(void) { return m_bFever; }
private:
	void StateManager(void);   // ��ԊǗ�
	void ControlManager(void); // ����̊Ǘ�
	void FeverManager(void);   // �t�B�[�o�[�̏���

	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	static bool m_bAlive;                 // ��������

	CGauge *    m_pGauge[PLAYER_GAUGE_NUM];  // �Q�[�W�N���X�̃|�C���^
	D3DXVECTOR3 m_move;                      // �ړ���
	int         m_nCntBullet;                // �e�̊Ԋu�m�ۗp
	int         m_nStateCount;               // �X�e�[�g�J�ڗp
	STATE       m_state;                     // �X�e�[�g
	bool        m_bBomb;                     // �{���������Ă��邩
	int         m_nRemain;                   // �c�@
	int         m_nBulletInterval;           // �A�ˑ��x
	float       m_fMp;                       // MP
	float       m_fLife;                     // �̗�
	float       m_fRotAngle;                 // ��]�p�x
	float       m_fRotAngleDist;             // ��]�p�x�̖ڕW�l
	float       m_fFeverGauge;               // �t�B�[�o�[�l
	bool        m_bFever;                    // �t�B�[�o�[���̔���
	CFeverLogo* m_pFeverLogo;                // �t�B�[�o�[�̃��S
	CFeverScreen* m_pFeverScreen;            // �t�B�[�o�[���
};

#endif