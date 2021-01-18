//===================================================
//
//    ���C�t�N���X�̏���[life.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
// �C���N���[�h
//**********************************
#include "bomb_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "boss.h"

//**********************************
// �}�N����`
//**********************************
#define BOMB_UI_POS D3DXVECTOR3(100.0f,632.0f,0.0f)
#define BOMB_UI_SIZE D3DXVECTOR3(25.0f,25.0f,0.0f)
#define BOMB_UI_TEXTURE_PATH "./data/Textures/Bomb.png" //�e�N�X�`��

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CBombUi::m_pTexture = NULL;
//int CBombUi::m_nBombUi = MAX_LIFE;

//==================================
// �R���X�g���N�^
//==================================
CBombUi::CBombUi() :CScene(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_pPolygon = NULL;
}

//==================================
// �f�X�g���N�^
//==================================
CBombUi::~CBombUi()
{
}

//==================================
// �N���G�C�g
//==================================
CBombUi * CBombUi::Create(void)
{
	// �������̊m��
	CBombUi *pBombUi;
	pBombUi = new CBombUi;
	// ������
	pBombUi->Init();
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	pBombUi->SetPriority(OBJTYPE_UI);

	return pBombUi;
}

//==================================
// ����������
//==================================
HRESULT CBombUi::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BOMB_UI_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(BOMB_UI_POS, BOMB_UI_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPolygon->BindTexture(m_pTexture);
	return S_OK;
}

//==================================
// �I������
//==================================
void CBombUi::Uninit(void)
{

	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	if (m_pPolygon != NULL)
	{
		// �I������
		m_pPolygon->Uninit();
		// �������̉��
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	// �J������
	Release();
}

//==================================
// �X�V����
//==================================
void CBombUi::Update(void)
{

}

//==================================
// �`�揈��
//==================================
void CBombUi::Draw(void)
{
	if (CGame::GetPlayer()->HaveBomb())
	{
		m_pPolygon->Draw();
	}
}
