////////////////////////////////////////////////////
//
//    ���U���g�N���X�̏���[result.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "warning_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "stage.h"
#include "sound.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CWarningLogo::m_pTexture = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TEXTURE_PATH "./data/Textures/logo_warning3.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CWarningLogo::CWarningLogo():CScene3d(OBJTYPE_WARNING)
{
}

//=============================
// �f�X�g���N�^
//=============================
CWarningLogo::~CWarningLogo()
{
}

//=============================
// �N���G�C�g
//=============================
CWarningLogo * CWarningLogo::Create(void)
{
	// �������̊m��
	CWarningLogo *pWarning = new CWarningLogo;
	// ������
	pWarning->Init();
	return pWarning;
}
//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CWarningLogo::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CWarningLogo::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}
//=============================
// ����������
//=============================
HRESULT CWarningLogo::Init(void)
{
	// ������
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// ���W�̐ݒ�
	SetPos(D3DXVECTOR3(STAGE_POS.x, STAGE_POS.x , 0.0f));
	// �T�C�Y�̐ݒ�
	SetSize(D3DXVECTOR3(STAGE_SIZE.x + 100, STAGE_SIZE.y + 100, 0.0f));
	//�@�e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);
	// �I�u�W�F���^�C�v�̐ݒ�
	SetPriority(OBJTYPE_WARNING);
	// SE�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_WARNING);
	return S_OK;
}

//=============================
// �I������
//=============================
void CWarningLogo::Uninit(void)
{
	// SE�̒�~
	CManager::GetSound()->Stop(CSound::LABEL_SE_WARNING);
	CScene3d::Uninit();
}


//=============================
// �X�V����
//=============================
void CWarningLogo::Update(void)
{
	SetAngle(GetAngle() + 0.1f);
}


//=============================
// �`�揈��
//=============================
void CWarningLogo::Draw(void)
{
	CScene3d::Draw();
}
