//===================================================
//
//    �J�[�\���N���X�̏���[cursor.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "cursor.h"
#include "manager.h"
#include "renderer.h"
#include "joypad.h"
#include "mouse.h"
#include "player.h"
#include "game.h"
#include "camera.h"

//*****************************
// �}�N����`
//*****************************
#define CURSOR_TEXTURE_PATH "./data/Textures/cursor001.png" // �e�N�X�`���̃p�X
#define CURSOR_RATE_SPEED 60.0f   // �W���C�p�b�h�̊��x�����p
#define CURSOR_SIZE 30            // �J�[�\���T�C�Y
//#define CURSOR_DISTANCE 100       // �J�[�\���T�C�Y
#define CURSOR_ANIM_SPEED 6       // �A�j���[�V�������x
#define CURSOR_MAX_ANIMATION_X 11 // �A�j���[�V������ ��
#define CURSOR_MAX_ANIMATION_Y 1  // �A�j���[�V������ �c

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CCursor::m_pTexture = NULL;

//==================================
// �R���X�g���N�^
//==================================
CCursor::CCursor():CScene2d(OBJTYPE_UI)
{
	m_fRotAngle = 0.0f;
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//==================================
// �f�X�g���N�^
//==================================
CCursor::~CCursor()
{
}

//==================================
// �N���G�C�g
//==================================
CCursor * CCursor::Create(const D3DXVECTOR3 pos)
{
	//�� �����̊m��
	CCursor *pCursor;
	pCursor = new CCursor;
	// ������
	pCursor->Init();

	pCursor->m_fRotAngle = 0.0f;

	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();

	// ���W�̐ݒ�
	CManager::GetMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f));
	pCursor->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f));

	// �T�C�Y�̐ݒ�
	pCursor->SetSize(D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f));
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pCursor->SetPriority(OBJTYPE_UI);

	return pCursor;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CCursor::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, CURSOR_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CCursor::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// ����������
//==================================
HRESULT CCursor::Init(void)
{
	ShowCursor(false);

	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}
	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// ��]�p�x�̏�����
	m_fRotAngle = 0.0f;

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / CURSOR_MAX_ANIMATION_X;
	float fv = 1.0f / CURSOR_MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	return S_OK;
}

//==================================
// �I������
//==================================
void CCursor::Uninit(void)
{
	CScene2d::Uninit();
	ShowCursor(true);
}

//==================================
// �X�V����
//==================================
void CCursor::Update(void)
{
	// �}�E�X�̎擾
	CInputMouse*pMouse = CManager::GetMouse();
	// �}�E�X�J�[�\�����W�̎擾
	D3DXVECTOR3 pos = pMouse->GetMousePos();
	// �ړ�����O�̍��W�̕ۑ�
	D3DXVECTOR3 posOld = pos;
	// �W���C�p�b�h�̎擾
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	//// �W���C�p�b�h�ŃJ�[�\���̑���
	//pos.x += cosf(45) * js.lZ / CURSOR_RATE_SPEED;
	//pos.y += sinf(45) * js.lRz / CURSOR_RATE_SPEED;

	//// ��ʒ[�̔���
	//if (pos.y < 0.0f + CURSOR_SIZE / 2)
	//{// ��
	//	pos.y = 0.0f + CURSOR_SIZE / 2;
	//}
	//if (pos.y > SCREEN_HEIGHT - CURSOR_SIZE / 2)
	//{ // ��
	//	pos.y = SCREEN_HEIGHT - CURSOR_SIZE / 2;
	//}
	//if (pos.x < 0.0f + CURSOR_SIZE / 2)
	//{// ��
	//	pos.x = 0.0f + CURSOR_SIZE / 2;
	//}
	//if(pos.x > SCREEN_WIDTH - CURSOR_SIZE / 2)
	//{ // ��
	//	pos.x = SCREEN_WIDTH - CURSOR_SIZE / 2;
	//}

	// �W���C�X�e�B�b�N�̊p�x�ŃJ�[�\���̈ʒu��ς���
	if (js.lRz >= 600 || js.lRz <= -600 || js.lZ >= 600 || js.lZ <= -600)
	{
		// �X�e�B�b�N�̃x�N�g���̌v�Z
		D3DXVECTOR3 stickVector = D3DXVECTOR3(cosf(45)*js.lZ, sinf(45)*js.lRz, 0.0f);
		// ���K��
		D3DXVec3Normalize(&stickVector, &stickVector);
		// ���W�̐ݒ�
		pos.x = SCREEN_WIDTH / 2 + stickVector.x * 200;
		pos.y = SCREEN_HEIGHT / 2 + stickVector.y * 200;
	}

	// ���W�̃Z�b�g
	SetPos(pos);
	pMouse->SetMousePos(pos);

	/*m_fRotAngle += 2.0f;
	SetAngle(m_fRotAngle);*/

	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	if (m_nCntAnim % CURSOR_ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX++;

		if (m_nAnimX >= CURSOR_MAX_ANIMATION_X)
		{
			// �A�j���[�V����X���̏�����
			m_nAnimX = 0;
		}

		// UV���W�̐ݒ�
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / CURSOR_MAX_ANIMATION_X;
		float fv = 1.0f / CURSOR_MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV���W�Z�b�g
		SetTextureUV(uv);
	}

}

//==================================
// �`�揈��
//==================================
void CCursor::Draw(void)
{
	CScene2d::Draw();
}

D3DXVECTOR3 CCursor::GetPos3d(void)
{
	D3DXVECTOR3 pos;

	CCamera*pCamera = CGame::GetCamera();

	// �e�s��̋t�s��
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &pCamera->GetViewMtx());
	D3DXMatrixInverse(&InvPrj, NULL, &pCamera->GetProjectionMtx());
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(GetPos().x, GetPos().y, 0), &tmp);

	return pos/2;
}
