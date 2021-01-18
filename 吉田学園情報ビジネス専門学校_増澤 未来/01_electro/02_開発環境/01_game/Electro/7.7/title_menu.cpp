//=======================================================================================
//
//�^�C�g�����j���[���� [titlemenu.cpp]
// Author : Masuzawa Mirai
//
//=======================================================================================

//=======================================================================================
// �C���N���[�h
//=======================================================================================
#include "title_menu.h"
#include "polygon.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "tcp_client.h"
#include "fade.h"
#include "joypad.h"
#include "game.h"
#include "mouse.h"
//=======================================================================================
// �}�N����`
//=======================================================================================
#define BACK_SIZE D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // �w�ʃT�C�Y
#define BACK_POS  D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // �w�ʍ��W

#define STRING_SIZE D3DXVECTOR3(150.0f,50.0f,0.0f)             // ������
#define NORMAL_POS  D3DXVECTOR3(SCREEN_WIDTH/2-150, 600,0.0f)  // �m�[�}��
#define HARD_POS    D3DXVECTOR3(SCREEN_WIDTH/2+150, 600,0.0f)  // �n�[�h

#define MENU_ENTER_COL     D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)     // �I��ł郁�j���[�̐F
#define MENU_NOT_ENTER_COL D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f)     // �I��łȂ����j���[�̐F

//=======================================================================================
// �O���錾
//=======================================================================================
LPDIRECT3DTEXTURE9 CTitleMenu::m_apTexture[CTitleMenu::PARTS_MAX] = {};

//=======================================================================================
// �R���X�g���N�^
//=======================================================================================
CTitleMenu::CTitleMenu():CScene(OBJTYPE_UI)
{
	memset(&m_pPolygon, 0, sizeof(m_pPolygon));
	m_nMenu = BACK;
	m_bMove = true;
}

//=======================================================================================
// �f�X�g���N�^
//=======================================================================================
CTitleMenu::~CTitleMenu()
{

}

//=======================================================================================
// �N���G�C�g
//=======================================================================================
CTitleMenu* CTitleMenu::Create(void)
{
	// �������̐���
	CTitleMenu* pTitleMenu = new CTitleMenu();
	// ����������
	pTitleMenu->Init();

	return pTitleMenu;
}

//=======================================================================================
// �e�N�X�`���̃��[�h
//=======================================================================================
HRESULT CTitleMenu::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Normal.png", &m_apTexture[NORMAL]); // �m�[�}��
	D3DXCreateTextureFromFile(pDevice, "data/Textures/LOGO_Hard.png"  , &m_apTexture[HARD]);   // �n�[�h

	return S_OK;
}
//=======================================================================================
// �e�N�X�`���̃A�����[�h
//=======================================================================================
void CTitleMenu::Unload(void)
{

	for (int nCntTex = 0; nCntTex < PARTS_MAX; nCntTex++)
	{
		// �e�N�X�`���̊J��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=======================================================================================
// ������
//=======================================================================================
HRESULT CTitleMenu::Init(void)
{
	// �|���S������
	m_pPolygon[BACK] = CPolygon::Create(BACK_POS, BACK_SIZE);
	m_pPolygon[BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	// �m�[�}��
	m_pPolygon[NORMAL] = CPolygon::Create(NORMAL_POS, STRING_SIZE);
	m_pPolygon[NORMAL]->BindTexture(m_apTexture[NORMAL]);
	// �n�[�h
	m_pPolygon[HARD] = CPolygon::Create(HARD_POS, STRING_SIZE);
	m_pPolygon[HARD]->BindTexture(m_apTexture[HARD]);

	return S_OK;
}

//=======================================================================================
// �I��
//=======================================================================================
void CTitleMenu::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < PARTS_MAX; nCntTex++)
	{
		if (m_pPolygon[nCntTex] != NULL)
		{
			//�I������
			m_pPolygon[nCntTex]->Uninit();
			// �������̉��
			delete m_pPolygon[nCntTex];
			m_pPolygon[nCntTex] = NULL;
		}
	}
}

//=======================================================================================
// �X�V
//=======================================================================================
void CTitleMenu::Update(void)
{

	// ���j���[�R�}���h
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		switch (m_nMenu)
		{
		case NORMAL:
			// �m�[�}��
			CGame::SetDefficult(CGame::DEFFICULT_NORMAL);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			break;

		case HARD:
			// �n�[�h
			CGame::SetDefficult(CGame::DEFFICULT_HARD);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			break;

		default:
			break;
		}
	}

	// �I��ł郁�j���[�ŐF����
	for (int nCntMenu = NORMAL; nCntMenu < PARTS_MAX; nCntMenu++)
	{
		if (m_nMenu == nCntMenu)
		{// �I��ł�Ƃ�
			m_pPolygon[nCntMenu]->SetColor(MENU_ENTER_COL);
		}
		else
		{// �I��łȂ��Ƃ�
			m_pPolygon[nCntMenu]->SetColor(MENU_NOT_ENTER_COL);
		}
	}
	DIJOYSTATE jy = CManager::GetJoypad()->GetStick(0);

	// ���j���[����
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_A) || CManager::GetKeyboard()->GetKeyTrigger(DIK_LEFT) || m_bMove && jy.lX <= -600)
	{// ��
		m_nMenu--;
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyTrigger(DIK_D) || CManager::GetKeyboard()->GetKeyTrigger(DIK_RIGHT) || m_bMove && jy.lX >= 600)
	{// �E
		m_nMenu++;
		m_bMove = false;
	}

	// �X�e�B�b�N�p�t���O�̏�����
	if (jy.lY <= 500 && jy.lY >= -500)
	{
		m_bMove = true;
	}

	// �͈͊O�ɍs���Ȃ��悤��
	if (m_nMenu < NORMAL)
	{
		m_nMenu = NORMAL;
	}
	else if (m_nMenu > HARD)
	{
		m_nMenu = HARD;
	}
}

//=======================================================================================
// �`��
//=======================================================================================
void CTitleMenu::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < PARTS_MAX; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Draw();
		}
	}
}