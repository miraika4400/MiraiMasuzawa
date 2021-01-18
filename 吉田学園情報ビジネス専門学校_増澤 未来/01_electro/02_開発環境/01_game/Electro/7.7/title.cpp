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
#include "title_logo.h"
#include "sound.h"
#include "press_logo.h"
#include "debug_log.h"
#include "title_menu.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_PATH "./data/Textures/title000.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CTitle::CTitle()
{
	m_pPolygon = NULL;
	m_pPressLogo = NULL;
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

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �e�N�X�`���̊��蓖��
	m_pPolygon->BindTexture(m_pTexture);

	// ���S����
	m_pPressLogo = CPressLogo::Create();
	CTitleLogo::Create();
	
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

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || 
		CManager::GetMouse()->GetMouseTrigger(0) || 
		CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_ENTER);
		/*CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);*/
		if (m_pPressLogo != NULL)
		{
			m_pPressLogo->Uninit();
			m_pPressLogo = NULL;
		}

		CTitleMenu::Create();
	}
}


//=============================
// �`�揈��
//=============================
void CTitle::Draw(void)
{
	// �|���S���̕`�揈��
	m_pPolygon->Draw();
}
