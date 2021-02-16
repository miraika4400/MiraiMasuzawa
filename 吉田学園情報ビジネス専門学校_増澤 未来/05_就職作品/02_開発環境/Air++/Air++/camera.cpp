////////////////////////////////////////////////////
//
//    �J�����N���X�̏���[camera.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "mouse.h"
#include "player.h"
#include "cpu.h"

//******************************
// �}�N����`
//******************************
#define CAMERA_DISTANCE 850.0f                               // �J�����ƑΏۂ̋���
#define CAMERA_VIEW_MAX 100000.0f                            // �J�����̕`�拗��(��)
#define CAMERA_VIEW_MIN 10.0f                                // �J�����̕`�拗��(��)
#define CAMERA_FOV D3DXToRadian(45.0f)                       // ����p
#define CAMERA_BACK_BUFFE D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f) // �J�����̔w�i�F
#define CAMERA_MOVE_SPEED_MAX 0.04f                          // �J�������슴�x�ő�l

// CPU�ɒǏ]�ւ���ɐ؂�ւ���}�N��
#define CPU_CAMERA
#undef  CPU_CAMERA

//******************************
// �ÓI�����o�ϐ��錾
//******************************
CCamera*CCamera::m_pCamera = NULL;

//******************************
// �R���X�g���N�^
//******************************
CCamera::CCamera()
{
	// �ϐ��̃N���A
	m_posV = { 0.0f,0.0f,0.0f };
	m_posR = { 0.0f,0.0f,0.0f };
	m_vecU = { 0.0f,0.0f,0.0f };
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_fRad   = 0.0f;              // �ڕW����̋���
	m_fTheta = 0.0f;              // �p�x�V�[�^
	m_fPhi   = 0.0f;              // �p�x�t�@�C
}

//******************************
// �f�X�g���N�^
//******************************
CCamera::~CCamera()
{
}

//******************************
// �N���G�C�g
//******************************
CCamera * CCamera::Create()
{

	m_pCamera = new CCamera;
	m_pCamera->Init();

	return m_pCamera;
}

//******************************
// �������
//******************************
void CCamera::Release(void)
{
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//******************************
// ����������
//******************************
HRESULT CCamera::Init(void)
{
	// �ϐ��̏�����
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // ������x�N�g��

	m_fRad = CAMERA_DISTANCE;     // ����
	m_fTheta = D3DXToRadian(90); // �p�x�V�[�^
	m_fPhi = D3DXToRadian(70);   // �p�x�t�@�C

	// ���ʍ��W�̐ݒ�
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

	return S_OK;
}

//******************************
// �I������
//******************************
void CCamera::Uninit(void)
{
}

//******************************
// �X�V����
//******************************
void CCamera::Update(void)
{
#ifdef CPU_CAMERA

	// CPU�ɒǏ]����J�����i�f�o�b�O�p�j
	CCpu* pCpu = (CCpu*)CScene::GetTop(CScene::OBJTYPE_CPU);

	// �����_��CPU�ɂ���
	if (pCpu != NULL)
	{
		m_posR = pCpu->GetPos();
	}

	// �p�x�V�[�^�̖ڕW�l
	float fThetaDist = (-pCpu->GetRot().y - D3DXToRadian(90));

	// �O����Ɖ�]���Ȃ��悤����
	while (m_fTheta - fThetaDist > D3DXToRadian(180))
	{
		fThetaDist += D3DXToRadian(360);
	}
	while (m_fTheta - fThetaDist < D3DXToRadian(-180))
	{
		fThetaDist -= D3DXToRadian(360);
	}

	// �V�[�^��ڕW�l�ɋ߂Â���
	m_fTheta += (fThetaDist - m_fTheta)*0.05f;
	
	// �J�����̏c�ړ�
	m_fPhi -= CManager::GetMouse()->GetMouseMove().y / 100;

#else
	// �v���C���[�ɒǏ]����J����
	CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

	if (pPlayer != NULL)
	{
		m_posR = pPlayer->GetPos();
	}

	m_fTheta -= min(max(CManager::GetMouse()->GetMouseMove().x / 100, -CAMERA_MOVE_SPEED_MAX), CAMERA_MOVE_SPEED_MAX);
	//m_fPhi   -= CManager::GetMouse()->GetMouseMove().y / 100;

#endif // CPU_CAMERA
	
	// ���ʍ��W�̐ݒ�
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

}

//******************************
// �J�����̃Z�b�g
//******************************
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera->m_mtxView);
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_pCamera->m_mtxView, &m_pCamera->m_posV, &m_pCamera->m_posR, &m_pCamera->m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera->m_mtxView);

	//�v���W�F�N�g�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera->m_mtxProjection);

	D3DXMatrixPerspectiveFovLH(&m_pCamera->m_mtxProjection,
		CAMERA_FOV, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_VIEW_MIN, CAMERA_VIEW_MAX);
	
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CAMERA_BACK_BUFFE, 1.0f, 0);
}