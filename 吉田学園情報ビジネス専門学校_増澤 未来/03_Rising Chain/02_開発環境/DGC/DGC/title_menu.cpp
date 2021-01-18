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
#include "fade.h"
#include "joypad.h"
#include "game.h"
#include "mouse.h"
#include "sound.h"

//=======================================================================================
// �}�N����`
//=======================================================================================
#define BACK_SIZE D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // �w�ʃT�C�Y
#define BACK_POS  D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f)  // �w�ʍ��W

#define STRING_SIZE D3DXVECTOR3(140.0f*1.5f,11.0f*1.5f,0.0f)              // ������T�C�Y
#define TWO_POS     D3DXVECTOR3(SCREEN_WIDTH/2, 530.0f-10,0.0f)  // ��l
#define THREE_POS   D3DXVECTOR3(SCREEN_WIDTH/2, 600.0f-10,0.0f)  // �O�l
#define FOUR_POS    D3DXVECTOR3(SCREEN_WIDTH/2, 670.0f-10,0.0f)  // �l�l

#define MENU_ENTER_COL     D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)   // �I��ł郁�j���[�̐F
#define MENU_NOT_ENTER_COL D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)   // �I��łȂ����j���[�̐F

//=======================================================================================
// �O���錾
//=======================================================================================
LPDIRECT3DTEXTURE9 CTitleMenu::m_apTexture[CTitleMenu::PARTS_MAX] = {};

//=======================================================================================
// �R���X�g���N�^
//=======================================================================================
CTitleMenu::CTitleMenu() :CScene(OBJTYPE_UI)
{
	memset(&m_pPolygon, 0, sizeof(m_pPolygon));
	m_nMenu = 0;
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
	D3DXCreateTextureFromFile(pDevice, "data/Textures/2_PLAYER_GAME.png", &m_apTexture[TWO]);   // ��l
	D3DXCreateTextureFromFile(pDevice, "data/Textures/3_PLAYER_GAME.png", &m_apTexture[THREE]); // �O�l
	D3DXCreateTextureFromFile(pDevice, "data/Textures/4_PLAYER_GAME.png", &m_apTexture[FOUR]);  // �l�l

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
	//m_pPolygon[BACK] = CPolygon::Create(BACK_POS, BACK_SIZE);
	//m_pPolygon[BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	// ��l
	m_pPolygon[TWO] = CPolygon::Create(TWO_POS, STRING_SIZE);
	m_pPolygon[TWO]->SetTexture(m_apTexture[TWO]);
	// �O�l
	m_pPolygon[THREE] = CPolygon::Create(THREE_POS, STRING_SIZE);
	m_pPolygon[THREE]->SetTexture(m_apTexture[THREE]);
	// �l�l
	m_pPolygon[FOUR] = CPolygon::Create(FOUR_POS, STRING_SIZE);
	m_pPolygon[FOUR]->SetTexture(m_apTexture[FOUR]);

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

	// �f�o�b�O�p
	if(CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		// ��l
		CGame::SetPlayerNum(1);
		// �`���[�g���A����ʂɑJ��
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		

	}
#ifdef _DEBUG
#endif // _DEBUG

	// ���j���[�R�}���h
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		switch (m_nMenu)
		{
		case TWO:
			// ��l
			CGame::SetPlayerNum(2);
			break;

		case THREE:
			// �O�l
			CGame::SetPlayerNum(3);
			break;

		case FOUR:
			// �l�l
			CGame::SetPlayerNum(4);
			break;
		default:
			break;
		}
		// SE
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
		// �`���[�g���A����ʂɑJ��
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		ShowCursor(FALSE);
	}

	// �I��ł郁�j���[�ŐF����
	for (int nCntMenu = TWO; nCntMenu < PARTS_MAX; nCntMenu++)
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
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_W) || CManager::GetKeyboard()->GetKeyTrigger(DIK_UP) || m_bMove && jy.lY <= -100)
	{// ��
		m_nMenu--;
		m_bMove = false;
	}
	else if (CManager::GetKeyboard()->GetKeyTrigger(DIK_S) || CManager::GetKeyboard()->GetKeyTrigger(DIK_DOWN) || m_bMove && jy.lY >= 100)
	{// �E
		m_nMenu++;
		m_bMove = false;
	}

	// �X�e�B�b�N�p�t���O�̏�����
	if (jy.lY <= 30 && jy.lY >= -30)
	{
		m_bMove = true;
	}

	// �͈͊O�ɍs���Ȃ��悤��
	if (m_nMenu < TWO)
	{
		m_nMenu = TWO;
	}
	else if (m_nMenu > FOUR)
	{
		m_nMenu = FOUR;
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