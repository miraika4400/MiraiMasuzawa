////////////////////////////////////////////////////
//
//    TitleLogo�̏���[TitleLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "press_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

//**********************************
// �}�N����`
//**********************************
#define LOGO_TEXTURE_PATH "./data/Textures/press_button.png" // �e�N�X�`��
#define LOGO_SIZE D3DXVECTOR3(685.0f*0.4f, 221.0f*0.4f, 0.0f)  // �T�C�Y�摜�̉𑜓x�ɍ��킹��
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH/2,600.0f,0.0f) // ���W

//=============================
// �R���X�g���N�^
//=============================
CPressLogo::CPressLogo()
{
	m_pTexture = NULL;
}

//=============================
// �f�X�g���N�^
//=============================
CPressLogo::~CPressLogo()
{
}

//=============================
// �N���G�C�g
//=============================
CPressLogo * CPressLogo::Create(void)
{
	// �������̊m��
	CPressLogo *pPressLogo = new CPressLogo;
	// ������
	pPressLogo->Init();
	return pPressLogo;
}

//=============================
// ����������
//=============================
HRESULT CPressLogo::Init(void)
{
	CTitleLogo::Init();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, LOGO_TEXTURE_PATH, &m_pTexture);

	// ���W�̐ݒ�
	SetPos(LOGO_POS);
	// �T�C�Y�̐ݒ�
	SetSize(LOGO_SIZE);
	// �e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================
// �I������
//=============================
void CPressLogo::Uninit(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CPressLogo::Update(void)
{
	CTitleLogo::Update();
}


//=============================
// �`�揈��
//=============================
void CPressLogo::Draw(void)
{
	CTitleLogo::Draw();
}
