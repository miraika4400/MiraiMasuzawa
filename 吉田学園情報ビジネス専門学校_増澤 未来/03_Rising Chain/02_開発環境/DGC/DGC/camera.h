//=============================================================================
//
// camera�w�b�_ [camera.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// �N���X��`
//*****************************

//�J�����N���X
class CCamera
{
public:
	//�����o�֐�
	CCamera();
	~CCamera();

	static CCamera *Create(int nNmPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void Shake(bool bRand = false);// �u��

	D3DXVECTOR3 GetPos(void) { return m_posV; }
	// �o�b�N�~���[�t���O�̎擾
	bool GetBackMirror(void) { return m_bBackMirror; }

	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	D3DXMATRIX GetProjectionMtx(void) { return m_mtxProjection; }
	float GetAngle(void) { return m_fAngle; }

	void SetPhiDist(float fPhiDist) { m_fPhiDest = fPhiDist; }

	
	D3DXVECTOR3 GetposV(void) { return m_posV; }
	D3DXVECTOR3 GetposR(void) { return m_posR; }
private:
	void FovManage(void);// ����p�Ǘ�

	// �����o�ϐ�
	D3DXVECTOR3 m_posV;            // �J�����̍��W
	D3DXVECTOR3 m_posR;            // �����_
	D3DXVECTOR3 m_vecU;            // ������x�N�g��
	D3DXVECTOR3 m_shake;           // �u��
	D3DXVECTOR3 m_shakeDist;       // �u��
	int m_nCntShake;               // �u���̕����]���J�E���g
	D3DXMATRIX  m_mtxProjection;   // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  m_mtxView;         // �r���[�}�g���b�N�X
	int         m_nPlayerNum;      // �ǂ̃v���C���[�̃J������
	float       m_fAngle;
	bool        m_bBackMirror;     // �o�b�N�~���[
	float       m_fViewFOV;        // ����p
	float       m_fViewFOVDist;    // ����p�̖ڕW�l
	// ���ʍ��W
	float m_fRad;         // �ڕW����̋���
	float m_fTheta;       // �p�x�V�[�^
	float m_fThetaDest;   // �V�[�^�̖ڕW�l
	float m_fPhi;         // �p�x�t�@�C
	float m_fPhiDest;     // �t�@�C�̖ڕW�l

	
	
};

#endif