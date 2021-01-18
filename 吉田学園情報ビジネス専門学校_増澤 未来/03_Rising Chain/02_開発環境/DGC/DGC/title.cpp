////////////////////////////////////////////////////
//
//    �^�C�g���N���X�̏���[title.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "title_menu.h"
#include "title_camera.h"
#include "course.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;
CTitleCamera *CTitle::m_pCamera = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_PATH "./data/Textures/Title.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CTitle::CTitle():CScene(OBJTYPE_UI)
{
	m_pPolygon = NULL;
}

//=============================
// �f�X�g���N�^
//=============================
CTitle::~CTitle()
{
}

//=============================
// �N���G�C�g
//=============================
CTitle * CTitle::Create(void)
{
	// �������̊m��
	CTitle *pTitle = new CTitle;
	// ������
	pTitle->Init();
	return pTitle;
}

//=============================
// ����������
//=============================
HRESULT CTitle::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2)-100, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->SetTexture(m_pTexture);
	
	// �l���I�����j���[�̐���
	CTitleMenu::Create();

	// �J�����̐���
	m_pCamera = CTitleCamera::Create();
	// �R�[�X�̐���
	CCourse::Create(CCourse::COURSE_EASY);

	return S_OK;
}

//=============================
// �I������
//=============================
void CTitle::Uninit(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pPolygon != NULL)
	{
		// �|���S���̏I������
		m_pPolygon->Uninit();

		// �������̉��
		delete m_pPolygon;
		m_pPolygon = NULL;
	}
	
	if (m_pCamera != NULL)
	{
		// �J�����̏I������
		m_pCamera->Uninit();

		// �������̉��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CTitle::Update(void)
{
	// �|���S���̍X�V����
	m_pPolygon->Update();

	if (m_pCamera != NULL)
	{
		// �J�����̏I������
		m_pCamera->Update();
	}
}


//=============================
// �`�揈��
//=============================
void CTitle::Draw(void)
{
	// �J�����̃Z�b�g
	m_pCamera->SetCamera();
	// �|���S���̕`�揈��
	m_pPolygon->Draw();
}
