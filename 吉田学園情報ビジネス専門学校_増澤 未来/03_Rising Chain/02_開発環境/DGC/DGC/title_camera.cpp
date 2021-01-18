////////////////////////////////////////////////////
//
//    �J�����N���X�̏���[camera.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "title_camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "player.h"
#include "mouse.h"
#include "joypad.h"
#include "destination.h"
#include "scene.h"

//******************************
// �}�N����`
//******************************
#define SPEED 50.0f // �ړ����x
#define DIRECTION_RATE 0.05f

//******************************
// �ÓI�����o�ϐ��錾
//******************************

//******************************
// �R���X�g���N�^
//******************************
CTitleCamera::CTitleCamera()
{
	// �ϐ��̃N���A
	m_posV = VEC3_ZERO;
	m_posR = VEC3_ZERO;
	m_vecU = VEC3_ZERO;
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_nNumPoint = 0;
	memset(&m_pointPos, 0, sizeof(m_pointPos));
	m_nNum = 0;               // ���W�ԍ�
	m_nNumNext = 1;           // ���̍��W�ԍ�
	m_fAngle = 0.0f;
	m_fAngleDist = 0.0f;
}

//******************************
// �f�X�g���N�^
//******************************
CTitleCamera::~CTitleCamera()
{
}

//******************************
// �N���G�C�g
//******************************
CTitleCamera * CTitleCamera::Create(void)
{

	CTitleCamera *pTitleCamera = new CTitleCamera;

	pTitleCamera->Init();

	return pTitleCamera;
}

//******************************
// ����������
//******************************
HRESULT CTitleCamera::Init(void)
{
	// �ϐ��̏�����
	m_posV = D3DXVECTOR3(80000.0f, 30000.0f, -500.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_nNum = 0;               // ���W�ԍ�
	m_nNumNext = 1;           // ���̍��W�ԍ�
	// �e�L�X�g�̓ǂ݂���
	LoadText();
	m_posV = m_pointPos[m_nNum];
	m_posR = m_pointPos[m_nNumNext];

	m_fAngle = atan2f(m_pointPos[m_nNumNext].z - m_pointPos[m_nNum].z, m_pointPos[m_nNumNext].x - m_pointPos[m_nNum].x);
	m_fAngleDist = m_fAngle;
	return S_OK;
}

//******************************
// �I������
//******************************
void CTitleCamera::Uninit(void)
{
}

//******************************
// �X�V����
//******************************
void CTitleCamera::Update(void)
{
	D3DXVECTOR3 move = m_pointPos[m_nNumNext] - m_pointPos[m_nNum];
	D3DXVec3Normalize(&move, &move);
	move *= SPEED;
	// �ړ��ʂ𑫂�
	m_posV += move;

	// �����_�̊Ǘ�
	m_fAngleDist = atan2f(move.z, move.x);
	Direction();
	m_posR.x = m_posV.x + cosf(m_fAngle)*SPEED;
	m_posR.y = m_posV.y;
	m_posR.z = m_posV.z + sinf(m_fAngle)*SPEED;

	float fDistance = sqrtf(powf(m_pointPos[m_nNumNext].x - m_posV.x, 2) + powf(m_pointPos[m_nNumNext].y - m_posV.y, 2) + powf(m_pointPos[m_nNumNext].z - m_posV.z, 2));
	if (fDistance <= 500)
	{
		m_nNum++;
		m_nNumNext = m_nNum + 1;

		m_nNum %= m_nNumPoint;
		m_nNumNext %= m_nNumPoint;

		//m_posR = m_pointPos[m_nNumNext];
	}
}

//******************************
// �J�����̃Z�b�g
//******************************
void CTitleCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�g�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	float fWidth = CManager::GetRenderer()->GetViewPort(0).Width;
	float fHeight = CManager::GetRenderer()->GetViewPort(0).Height;

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(60.0f),
		fWidth / fHeight, 10.0f, 1000000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

void CTitleCamera::Direction(void)
{
	// �O����Ɖ�]���Ȃ��悤����
	while (m_fAngleDist - m_fAngle > D3DXToRadian(180))
	{
		m_fAngleDist -= D3DXToRadian(360);
	}
	while (m_fAngleDist - m_fAngle < D3DXToRadian(-180))
	{
		m_fAngleDist += D3DXToRadian(360);
	}

	m_fAngle += (m_fAngleDist - m_fAngle)*DIRECTION_RATE;
}

//******************************
// �e�L�X�g�̓ǂݍ���
//******************************
void CTitleCamera::LoadText(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;
	pFile = fopen("data/Texts/Title_data.txt", "r");
	
	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fscanf(pFile, "%d", &m_nNumPoint);

		// �`�F�b�N�|�C���g�������[�v
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// ���W�̓ǂݍ���
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);

		}

		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}
