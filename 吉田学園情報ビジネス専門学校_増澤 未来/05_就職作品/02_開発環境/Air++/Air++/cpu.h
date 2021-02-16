//=============================================================================
//
// cpu�w�b�_ [cpu.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CPU_H_
#define _CPU_H_

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
class CCpu : public CCharacter
{
public:
	//==========================
	// ��
	//==========================
	typedef enum
	{
		CPU_TYPE_STAR = 0,
		CPU_TYPE_RUIN,
		CPU_TYPE_MAX,
	}CPUTYPE;

	//==========================
	// �����o�֐�
	//==========================
	CCpu();
	~CCpu();

	static CCpu *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CPUTYPE cpuType , int nLevel); // �N���X����
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	void Move(void);        // �ړ�����
	void DistManager(void); // �ڕW�n�_�̊Ǘ��֐�

	//==========================
	// �����o�ϐ�
	//==========================
	static Model m_aModel[CPU_TYPE_MAX]; // ���f���\����
	static Shader m_shader;              // �V�F�[�_�[�\����
	CPUTYPE m_cpuType;                   // CPU�̎��
	CCollision * m_pDIstCollision;       // �ړ��ڕW�l�����蔻��
	int m_nCpuPointNum;                  // �R�[�X�|�C���g�z��Ǘ��p
	int m_nLevel;                        // CPU�̋���
};
#endif