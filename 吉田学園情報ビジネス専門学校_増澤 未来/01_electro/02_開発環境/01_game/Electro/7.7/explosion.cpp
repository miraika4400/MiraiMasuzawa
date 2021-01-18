////////////////////////////////////////////////////
//
//    explosion�N���X�̏���[explosion.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "explosion.h"
#include "manager.h"
#include "renderer.h"

//**********************************
//�ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//*****************************
// �}�N����`
//*****************************
#define EXPLOSION_ANIM_SPEED 6       // �A�j���[�V�������x
#define EXPLOSION_MAX_ANIMATION_X 7  // �A�j���[�V�������@��
#define EXPLOSION_MAX_ANIMATION_Y 1  // �A�j���[�V�������@�c

#define EXPLOSION_TEXTURE_PATH "./data/Textures/explosion003.png" // �e�N�X�`���̃p�X

//******************************
// �R���X�g���N�^
//******************************
CExplosion::CExplosion() :CScene3d(OBJTYPE_EXPLOSION)
{
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
}

//******************************
// �f�X�g���N�^
//******************************
CExplosion::~CExplosion()
{
}

//******************************
// �N���G�C�g
//******************************
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �������̊m��
	CExplosion *pExplosion;
	pExplosion = new CExplosion;

	// ������
	pExplosion->Init();

	// �e�l�̃Z�b�g
	pExplosion->SetPos(pos);   // ���W
	pExplosion->SetSize(size); // �T�C�Y
	pExplosion->SetColor(col); // �F
	pExplosion->SetPriority(OBJTYPE_EXPLOSION); // �I�u�W�F�N�g�^�C�v

	return pExplosion;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CExplosion::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CExplosion::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// ����������
//******************************
HRESULT CExplosion::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// UV���W�̏�����
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV���W�̐ݒ�

	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / EXPLOSION_MAX_ANIMATION_X;
	float fv = 1.0f / EXPLOSION_MAX_ANIMATION_Y;
	
	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY - fv);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	
	// UV���W�Z�b�g
	SetTextureUV(uv);

	return S_OK;
}

//******************************
// �I������
//******************************
void CExplosion::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CExplosion::Update(void)
{
	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	if (m_nCntAnim % EXPLOSION_ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX++;

		if (m_nAnimX >= EXPLOSION_MAX_ANIMATION_X)
		{
			// �A�j���[�V����X���̏�����
			Uninit();
		}
		else
		{
			// UV���W�̐ݒ�
			D3DXVECTOR2 uv[NUM_VERTEX];

			float fu = 1.0f / EXPLOSION_MAX_ANIMATION_X;
			float fv = 1.0f / EXPLOSION_MAX_ANIMATION_Y;

			uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY - fv);
			uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY - fv);
			uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
			uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
			// UV���W�Z�b�g
			SetTextureUV(uv);
		}


	}
}

//******************************
// �`�揈��
//******************************
void CExplosion::Draw(void)
{
	CScene3d::Draw();
}
