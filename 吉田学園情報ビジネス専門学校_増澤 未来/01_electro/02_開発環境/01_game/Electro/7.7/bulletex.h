//=============================================================================
//
// bulletex�w�b�_ [bulletex.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BULLETEX_H_
#define _BULLETEX_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "bullet.h"

//*****************************
// �O���錾
//*****************************
class CScene2d;

//*****************************
// �N���X��`
//*****************************

// �܂���������Ȃ��e�N���X
class CBulletEx : public CBullet
{
public:
	//============
	// �񋓒�`
	//============
	// �e�̎��
	typedef enum
	{
		BULLETTYPE_ROTATION,       // �������ʒu�𒆐S�ɉ��
		BULLETTYPE_ROTATION_SPREAD,// �������ʒu�𒆐S�ɉ���Ĕ��a���L�����Ă�
		BULLETTYPE_TARGET,         // �܂�������񂾌�Ɏ��@�_���̒e���o��
		BULLETTYPE_HOMING,         // �z�[�~���O
		BULLETTYPE_MAX
	}BULLETTYPE;

	//============
	// �����o�֐�
	//============
	CBulletEx();
	~CBulletEx();

	// �e�^�C�v�̒e�̐���

	static CBulletEx *Create_Rotation(const D3DXVECTOR3 pos, const D3DXVECTOR3 posParent, const D3DXVECTOR3 move, const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col);

	static CBulletEx *Create_Rotation_Spread(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife,
		const BULLETUSER user, const D3DCOLOR col, const float rotAngle = 0.0f);

	static CBulletEx *Create_Target(const D3DXVECTOR3 pos, const D3DXVECTOR3 move,const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col);

	static CBulletEx *Create_Homing(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size,
		const int nLife, const BULLETUSER user, const D3DCOLOR col,const int nTargetNum);
	

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_posParent;  // �e���W
	D3DXVECTOR3 m_move;       // �ړ���
	int         m_nLife;      // ����
	BULLETTYPE  m_bulletType; // �e�̎��
	float       m_fRotAngle;  // ��]�p�x
	float       m_fDistance;  // �e���W�Ƃ̋���
	int         m_nTargetNum; // �^�[�Q�b�g�̎��ʗp
};

#endif