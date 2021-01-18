//=============================================================================
//
// bullet�w�b�_ [bullet.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
// �}�N����`
//*****************************
#define BULLET_SPEED_PLAYER 8    // ���x(�v���C���[�̒e)
#define BULLET_SPEED_ENEMY  4    // ���x(�G�l�~�[�̒e)
#define BULLET_SIZE         15   // �T�C�Y

//*****************************
// �N���X��`
//*****************************

// �e�N���X
class CBullet : public CScene3d
{
public:
	//============
	// �񋓒�`
	//============
	// ������̃^�C�v
	typedef enum
	{
		BULLETUSER_PLAYER, // �v���C���[
		BULLETUSER_ENEMY,  // �G�l�~�[
		BULLETUSER_MAX,	   // �^�C�v�̍ő吔
	}BULLETUSER;

	//============
	// �����o�֐�
	//============
	CBullet();
	~CBullet();
	// static
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const  BULLETUSER user, const D3DCOLOR col); // �N���X����
	static HRESULT Load(void);                                                                                                          // �e�N�X�`�����[�h
	static void Unload(void);                                                                                                           // �e�N�X�`���A�����[�h
	static std::list<CBullet*>*GetBulletlist(void) { return &m_bulletList; }                                                            // ���X�g�̎擾

	HRESULT Init(void);// ������
	void Uninit(void); // �I��
	void Update(void); // �X�V
	void Draw(void);   // �`��
	
	void SetUser(BULLETUSER user) { m_user = user; } // ������̃^�C�v�̐ݒ�
	BULLETUSER GetUser(void) { return m_user; }      // ������̃^�C�v�̎擾
protected:
	void CollisionBullet(BULLETUSER user); // �����蔻��̊Ǘ�
	
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
private:
	// �����o�ϐ�
	static std::list<CBullet*> m_bulletList;  // ���X�g
	D3DXVECTOR3 m_move;                       // �ړ���
	int         m_nLife;                      // ����
	BULLETUSER  m_user;                       // �o���b�g���[�U�[
};

#endif