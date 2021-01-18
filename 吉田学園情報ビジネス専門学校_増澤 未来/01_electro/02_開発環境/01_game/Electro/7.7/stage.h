//=============================================================================
//
// stage�w�b�_ [stage.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _STAGE_H_
#define _STAGE_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// �O���錾
//*****************************
class CScene3d;
class CWarningLogo;

//*****************************
// �}�N����`
//*****************************
#define STAGE_POS D3DXVECTOR3(0.0f,0.0f,0.0f)         // ���W
#define STAGE_SIZE D3DXVECTOR3(1500.0f,1500.0f,0.0f)  // �T�C�Y
#define STAGE_PATS_NUM 2                              // �X�e�[�W�̃p�[�c��

//*****************************
// �N���X��`
//*****************************

//�X�e�[�W�N���X
class CStage : public CScene
{
public:
	//============
	// ��ԗ�
	//============
	// �X�e�[�W�^�C�v
	typedef enum
	{
		STAGE_1 = 0,  // 1
		STAGE_2,      // 2
		STAGE_3,      // 3
		STAGE_4,      // 4
		STAGE_5,      // 5
		STAGE_6,      // 6
		STAGE_7,      // 7
		STAGE_8,      // 8
		STAGE_9,      // 9
		STAGE_10,     // 10
		STAGE_BOSS,	  // !
		STAGE_BACK,   // �X�e�[�W�̌��̃|���S��
		STAGE_FRONT,  // �X�e�[�W�̑O�̃|���S��
		STAGE_MAX     // �X�e�[�W�̃^�C�v��
	}STAGE_PARTS;

	//�����o�֐�
	CStage();
	~CStage();

	static CStage *Create(void); // �N���X����
	static HRESULT Load(void);   // �e�N�X�`�����[�h
	static void Unload(void);    // �e�N�X�`���A�����[�h
	
	HRESULT Init(void);// ������
	void Uninit(void); // �I��
	void Update(void); // �X�V
	void Draw(void);   // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[STAGE_MAX]; //�e�N�X�`��
	CScene3d *pScene3d[STAGE_PATS_NUM]; // �|���S��
	CWarningLogo*pWarning;              // ���[�j���O���S�N���X
	D3DXCOLOR m_distCol;                // �F�̖ڕW�l
	int m_nColChangeCnt;                // �F�`�F���W�̃J�E���g
	
};

#endif