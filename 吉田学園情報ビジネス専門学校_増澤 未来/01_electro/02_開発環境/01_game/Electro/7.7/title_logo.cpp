////////////////////////////////////////////////////
//
//    TitleLogo�̏���[TitleLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "title_logo.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTitleLogo::m_pTexture = NULL;

//**********************************
// �}�N����`
//**********************************
#define TITLE_LOGO_TEXTURE_PATH "./data/Textures/title_logo2.png" // �e�N�X�`��
#define ANIM_SPEED 5          // �A�j���[�V�������x
#define MAX_ANIMATION_X 4      // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 6      // �A�j���[�V������ �c
#define LOGO_SIZE D3DXVECTOR3(685.0f/2, 221.0f/2, 0.0f)  // �T�C�Y�摜�̉𑜓x�ɍ��킹��
#define LOGO_POS D3DXVECTOR3(SCREEN_WIDTH/2,300.0f,0.0f) // ���W
#define NEON_MAX_X 4                                     // �l�I���_������U���W
#define NEON_MAX_Y 5                                     // �l�I���_������V���W
#define NEON_INTERVAL 60                                 // �A�j���[�V�����p�x

//=============================
// �R���X�g���N�^
//=============================
CTitleLogo::CTitleLogo():CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0;       // �A�j���[�V�����J�E���g
	m_nAnimX = 0;         // �A�j���[�V����X��
	m_nAnimY = 0;         // �A�j���[�V����Y��
	m_nCntSetAnim = 0;
	m_bAnim = false;
}

//=============================
// �f�X�g���N�^
//=============================
CTitleLogo::~CTitleLogo()
{
}

//=============================
// �N���G�C�g
//=============================
CTitleLogo * CTitleLogo::Create(void)
{
	// �������̊m��
	CTitleLogo *pTitleLogo = new CTitleLogo;
	// ������
	pTitleLogo->Init();
	return pTitleLogo;
}

//=============================
// ����������
//=============================
HRESULT CTitleLogo::Init(void)
{
	CScene2d::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURE_PATH, &m_pTexture);
	
	m_bAnim = false;
	m_nAnimX = NEON_MAX_X;
	m_nAnimY = NEON_MAX_Y;

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

	// �����_���ŏ�����
	m_nCntSetAnim = rand() % 20;
	return S_OK;
}

//=============================
// �I������
//=============================
void CTitleLogo::Uninit(void)
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
void CTitleLogo::Update(void)
{
	
	if (m_bAnim)
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

			uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

			// UV���W�Z�b�g
			SetTextureUV(uv);
		}

		if (m_nAnimX == NEON_MAX_X && m_nAnimY == NEON_MAX_Y)
		{
			m_nCntSetAnim = rand() % 20;
			m_bAnim = false;
		}
	}
	else
	{
		m_nCntSetAnim++;
		if (m_nCntSetAnim >= NEON_INTERVAL && rand() % 20 == 0)
		{
			m_nCntAnim = ANIM_SPEED - 1;
			CManager::GetSound()->Play(CSound::LABEL_SE_NEON);
			m_bAnim = true;
		}

	}
	
}


//=============================
// �`�揈��
//=============================
void CTitleLogo::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2d::Draw();

	// �ʏ퍇���ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
