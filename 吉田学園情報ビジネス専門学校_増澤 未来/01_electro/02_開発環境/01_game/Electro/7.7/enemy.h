//=============================================================================
//
// enemy�w�b�_ [enemy.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �N���X��`
//*****************************

// �G�l�~�[�N���X
class CEnemy : public CScene3d
{
public:
	//============
	// �񋓒�`
	//============
	// �G�l�~�[�̎��
	typedef enum
	{
		ENEMYTYPE_YELLOW = 0,  // ���F�G�l�~�[
		ENEMYTYPE_PURPLE,      // ���G�l�~�[
		ENEMYTYPE_RED,         // �ԃG�l�~�[
		ENEMYTYPE_MAX,         // �G�l�~�[�^�C�v�̍ő吔
	}ENEMYTYPE;

	// �X�e�[�g
	typedef enum
	{
		STATE_NORMAL = 0, // �ʏ�
		STATE_DAMAGE,     // �_���[�W
		STATE_MAX,        // ��Ԃ̍ő吔
	}STATE;

	//============
	// �\���̒�`
	//============
	// �G�l�~�[�̗N���Ǘ��p�\����
	typedef struct
	{
		int nMaxEnemy;   // �G�l�~�[�̍ő吔
		int nSpawnCount; // �G�l�~�[�̗N���p�x
		int nPhaseTime;  // �t�F�[�Y��؂�ւ��鎞��
		int nCircleNum;  // �T�[�N����
	}ENEMY_SPAWN;

	//============
	//�����o�֐�
	//============
	CEnemy();
	~CEnemy();

	// static
	static CEnemy *Create(const D3DXVECTOR3 pos,const ENEMYTYPE type);     // �N���X����
	static HRESULT Load(void);                                             // �e�N�X�`�����[�h
	static void Unload(void);                                              // �e�N�X�`���A�����[�h
	static void LoadDefficult(void);                                       // ��Փx�ʂ̓G�̗N���f�[�^�̓ǂݍ���
	static void UnloadDefficult(void);                                     // ��Փx�ʂ̓G�̗N���f�[�^�̃A�����[�h
	static void SpawnManager(void);                                        // �G�l�~�[�̂킫�̊Ǘ�
	static int GetEnemyNum(void) { return m_nNumEnemy; }                   // �G�l�~�[���̎擾����
	static void PlusEnemy(void) { m_nNumEnemy++; }                         // �G�l�~�[���𑝂₷
	static void MinusEnemy(void) { m_nNumEnemy--; }                        // �G�l�~�[���𑝂₷
	static std::list<CEnemy*>*GetEnemylist(void) { return &m_enemyList; }  // ���X�g�̎擾

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	D3DXVECTOR3 GetMove(void) { return m_move; } // �ړ��ʂ̎擾
	void HitAttack(int nDamage);                 // ��e����
private:
	void MoveManager(void);    // �ړ��̊Ǘ�
	void ParticleCreate(void); // �p�[�e�B�N������

	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_MAX]; // �e�N�X�`���̃|�C���^
	static std::list<CEnemy*> m_enemyList;               // �G�l�~�[���X�g
	static int m_nCntSpawn;                              // �X�|�[���J�E���g
	static int m_nNumEnemy;                              // �G�l�~�[�ƃ{�X�̐��Ǘ��p
	static ENEMY_SPAWN * m_pSpawn;                       // �G�l�~�[�̂킫�Ǘ��p
	static int m_nMaxPhaseNum;                           // �t�F�[�Y�ő吔
	static int m_nPhaseNum;                              // �t�F�[�Y��

	D3DXVECTOR3 m_move;        // �ړ���
	ENEMYTYPE   m_type;        // �G�̎��
	STATE       m_state;       // �X�e�[�g
	int         m_nCntMove;    // �ړ��J�E���g
	int         m_nCntBullet;  // �e�̊Ԋu�m�ۗp
	int         m_nLife;       // ���C�t
	int         m_nStateCount; // �X�e�[�g�J�ڗp
	int         m_nCntEncount; // �G���J�E���g��ԉ������̃J�E���g�p
	float       m_fMoveAngle;  // �ړ��p�x
	bool        m_bPlusAngle;  // �W�O�U�O�����G�l�~�[�̊p�x���v���X���邩�}�C�i�X���邩

	
};

#endif