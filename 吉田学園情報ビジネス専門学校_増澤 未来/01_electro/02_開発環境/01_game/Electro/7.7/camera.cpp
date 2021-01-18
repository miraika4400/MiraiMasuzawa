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
#include "player.h"
#include "keyboard.h"
#include "game.h"
#include "boss.h"

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
	m_fViewAriaX = 0.0f;
	m_fViewAriaY = 0.0f;
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
	
	CPlayer*pPlayer = CGame::GetPlayer();
	if (pPlayer != NULL)
	{
		if (CPlayer::IsAlive())
		{
			D3DXVECTOR3 playerPos = pPlayer->GetPos();
			m_posV = D3DXVECTOR3(playerPos.x, playerPos.y, -500.0f);
			m_posR = playerPos;
			m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}
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
	if (CPlayer::IsAlive())
	{
		// �v���C���[�̎擾
		CPlayer*pPlayer = CGame::GetPlayer();
		if (pPlayer != NULL)
		{
			// �v���C���[���W
			D3DXVECTOR3 playerPos = pPlayer->GetPos();

			// �J�������v���C���[�ɂ��Ă����悤��
			m_posV = pPlayer->GetPos() * 2;
			m_posV.z = -500;
			m_posR = pPlayer->GetPos() * 2;

		}
	}

	
	float fDistViewAreaX = 0.0f;
	float fDistViewAreaY = 0.0f;
	// �{�X��Ŏ�����L����
	if (CGame::GetWave()==CGame::WAVE_BOSS)
	{
		fDistViewAreaX = 1.6f * 500.0f;
		fDistViewAreaY = 0.9f * 500.0f;
	}

	m_fViewAriaX += (fDistViewAreaX - m_fViewAriaX)*0.05f;
	m_fViewAriaY += (fDistViewAreaY - m_fViewAriaY)*0.05f;
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

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬*���s���e
	D3DXMatrixOrthoLH(&m_pCamera->m_mtxProjection, ((float)SCREEN_WIDTH * 2) + m_fViewAriaX, ((float)SCREEN_HEIGHT * 2) + m_fViewAriaY, 1, 1000);

	/*D3DXMatrixPerspectiveFovLH(&m_pCamera->mtxProjection,
		D3DXToRadian(100.0f), 
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.0f, 1000.0f);*/
	
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CManager::GetRenderer()->GetBuffColor(), 1.0f, 0);

	
}
