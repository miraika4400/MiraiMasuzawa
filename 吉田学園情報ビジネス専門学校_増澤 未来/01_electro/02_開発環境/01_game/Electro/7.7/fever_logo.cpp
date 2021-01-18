////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "fever_logo.h"
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
#define TEXTURE_PATH "./data/Textures/fever.png" // �e�N�X�`���̃p�X
#define ANIM_SPEED 3           // �A�j���[�V�������x
#define MAX_ANIMATION_X 12     // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1      // �A�j���[�V������ �c
#define COLOR_COUNT 20         // �ڕW�J���[�ύX�̃C���^�[�o��
#define COLOR_RATE  0.05f      // �F��ς���Ƃ��W��
#define LOGO_SIZE D3DXVECTOR3(600.0f/2.0f, 200.0f / 2.0f, 0.0f)   // �T�C�Y�摜�̉𑜓x�ɍ��킹��
#define LOGO_POS  D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f,0.0f)  // ���W
//D3DXCOLOR(((float)(rand() % 50) / 100.0f)+ 0.5f, ((float)(rand() % 50) / 100.0f)+0.5f, ((float)(rand() % 50) / 100.0f) + 0.5f, 1.0f);
//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CFeverLogo::m_pTexture = NULL;

//=============================
// �R���X�g���N�^
//=============================
CFeverLogo::CFeverLogo() :CScene2d(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nCntAnim  = 0;                                 // �A�j���[�V�����J�E���g
	m_nAnimX    = 0;                                 // �A�j���[�V����X��
	m_nAnimY    = 0;                                 // �A�j���[�V����Y��
	m_nCntColor = 0;                                 // �F��ς���J�E���g
	m_distColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // �J���[�ύX���̖ڕW�J���[
}

//=============================
// �f�X�g���N�^
//=============================
CFeverLogo::~CFeverLogo()
{
}

//=============================
// �N���G�C�g
//=============================
CFeverLogo * CFeverLogo::Create(void)
{
	// �������̊m��
	CFeverLogo *pFeverLogo = new CFeverLogo;
	// ������
	pFeverLogo->Init();
	return pFeverLogo;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CFeverLogo::Load(void)
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
void CFeverLogo::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// ����������
//=============================
HRESULT CFeverLogo::Init(void)
{
	CScene2d::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	// ���W�̐ݒ�
	SetPos(LOGO_POS);
	// �T�C�Y�̐ݒ�
	SetSize(LOGO_SIZE);
	// �e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);
	//SetAddMode(true);
	// �����_���ŏ�����
	m_distColor = D3DXCOLOR((float)(rand() % 80) / 100.0f, (float)(rand() % 80) / 100.0f, (float)(rand() % 80) / 100.0f, 1.0f);
	return S_OK;
}

//=============================
// �I������
//=============================
void CFeverLogo::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CFeverLogo::Update(void)
{
	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX++;

		if (m_nAnimX > MAX_ANIMATION_X)
		{
			// �A�j���[�V����X���̏�����
			m_nAnimX = 0;
			m_nAnimY++;
			if (m_nAnimY > MAX_ANIMATION_Y)
			{
				m_nAnimY = 0;
			}
		}

		// UV���W�̐ݒ�
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV���W�Z�b�g
		SetTextureUV(uv);
	}

	// �J�E���g��i�߂�
	m_nCntColor++;
	// ���̃J�E���g��
	if (m_nCntColor % COLOR_COUNT == 0)
	{
		m_distColor = D3DXCOLOR((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1.0f); // �J���[�ύX���̖ڕW�J���[
	}
	else
	{
		// ���݃J���[�̎擾
		D3DXCOLOR col = GetColor();
		
		// ���X�ɐF��ς���
		col.r += (m_distColor.r - col.r)*COLOR_RATE;
		col.g += (m_distColor.g - col.g)*COLOR_RATE;
		col.b += (m_distColor.b - col.b)*COLOR_RATE;

		// �F�̃Z�b�g
		SetColor(col);
	}
}


//=============================
// �`�揈��
//=============================
void CFeverLogo::Draw(void)
{
	CScene2d::Draw();
}
