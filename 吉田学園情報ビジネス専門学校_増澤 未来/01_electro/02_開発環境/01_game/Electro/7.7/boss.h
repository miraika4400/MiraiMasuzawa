//=============================================================================
//
// boss�w�b�_ [boss.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BOSS_H_
#define _BOSS_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �O���錾
//*****************************
class CGauge;

//*****************************
//�}�N����`
//*****************************
#define BOSS_MAX_LIFE 200.0f  // �{�X�̗̑�
#define BOSS_SPAWN_SEC 60  //�{�X���킭�b��
#define BOSS_HP_WIDTH 500.0f                               // HP�o�[�̉����̍ő�
#define BOSS_HP_HEGHT 10                                   // HP�o�[�̏c��
#define BOSS_HP_POS_X SCREEN_WIDTH/2 - (BOSS_HP_WIDTH/2)   // X�ʒu
#define BOSS_HP_POS_Y 100.0f                               // Y�ʒu

//*****************************
// �N���X��`
//*****************************

//�{�X�N���X
class CBoss : public CScene3d
{
public:
	//============
	// �񋓒�`
	//============
	// ���
	typedef enum
	{
		STATE_NORMAL = 0, // �ʏ�
		STATE_DAMAGE,     // �_���[�W
		STATE_DEATH,      // ���S
		STATE_MAX,        // ��Ԃ̍ő吔
	}STATE;

	//============
	// �����o�֐�
	//============
	CBoss();
	~CBoss();

	// static
	static CBoss *Create(const D3DXVECTOR3 pos);     // �N���X����
	static HRESULT Load(void);                       // �e�N�X�`�����[�h
	static void Unload(void);                        // �e�N�X�`���A�����[�h
	static bool IsAlive(void) { return m_bIsAlive; } // �����Ă��邩�擾

	HRESULT Init(void);// ������
	void Uninit(void); // �I��
	void Update(void); // �X�V
	void Draw(void);   // �`��

	float GetLife(void) { return m_fLife; } // �̗͂̎擾 
	void HitAttack(int nDamage);            // ��e����

private:
	void AttackManager(void); // �U���̏���

	//============
	// �����o�֐�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���̃|�C���^
	static bool m_bIsAlive;               // ��������

	CGauge * m_pGauge;         // �Q�[�W�N���X�̃|�C���^
	int      m_nRandAttack;    // �U���p�^�[���̃����_��
	float    m_fLife;          // ���C�t
	float    m_fBulletAngle;   // �e�����p�x
	int      m_nCntBullet;     // �e�̊Ԋu�m�ۗp
	STATE    m_state;          // �X�e�[�g
	int      m_nStateCount;    // �X�e�[�g�J�ڗp
	int      m_nCntAnim;       // �A�j���[�V�����J�E���g
	int      m_nAnimX;         // �A�j���[�V����X��
	int      m_nAnimY;         // �A�j���[�V����Y��
	bool     m_bAttack;        // �U�����Ȃ��^�C�~���O�����p
};

#endif