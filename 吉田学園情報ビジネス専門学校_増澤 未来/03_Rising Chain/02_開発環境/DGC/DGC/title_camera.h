//=============================================================================
//
// title_camera�w�b�_ [title_camera.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _TITLE_CAMERA_H_
#define _TITLE_CAMERA_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// �N���X��`
//*****************************

//�J�����N���X
class CTitleCamera
{
public:
	//�����o�֐�
	CTitleCamera();
	~CTitleCamera();

	static CTitleCamera *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);


	D3DXVECTOR3 GetposV(void) { return m_posV; }
	D3DXVECTOR3 GetposR(void) { return m_posR; }
private:
	void Direction(void);
	void LoadText(void);
	// �����o�ϐ�
	D3DXVECTOR3 m_posV;            // �J�����̍��W
	D3DXVECTOR3 m_posR;            // �����_
	D3DXVECTOR3 m_vecU;            // ������x�N�g��
	D3DXMATRIX  m_mtxProjection;   // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  m_mtxView;         // �r���[�}�g���b�N�X
	int m_nNumPoint;               // �`�F�b�N�|�C���g�̐�
	D3DXVECTOR3 m_pointPos[256];   // �`�F�b�N�|�C���g�̍��W
	int m_nNum;                    // ���W�ԍ�
	int m_nNumNext;                // ���̍��W�ԍ�
	float m_fAngle;                // ����
	float m_fAngleDist;            // �����̖ڕW�l

};

#endif