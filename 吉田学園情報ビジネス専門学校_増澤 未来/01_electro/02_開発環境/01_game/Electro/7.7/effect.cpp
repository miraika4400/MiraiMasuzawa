//===================================================
//
//    �G�t�F�N�g�N���X�̏���[effect.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//*****************************
// �}�N����`
//*****************************
#define EFFECT_TEXTURE_PATH "./data/Textures/bullet000.png" // �e�N�X�`���̃p�X
#define EFFECT_LIFE 6 // �G�t�F�N�g�̎���
#define EFFECT_SUB_SIZE 0.8f   // �T�C�Y�̌�����
#define EFFECT_SUB_ALPHA 0.1f  // a�l�̌�����

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL; // �e�N�X�`��

//*****************************
// �R���X�g���N�^
//*****************************
CEffect::CEffect() : CScene3d(OBJTYPE_EFFECT)
{
	m_nLife = EFFECT_LIFE;
}

//*****************************
// �f�X�g���N�^
//*****************************
CEffect::~CEffect()
{
}

//*****************************
// �N���G�C�g
//*****************************
CEffect * CEffect::Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,const D3DXCOLOR col, const LPDIRECT3DTEXTURE9 pTexture)
{
	// �������̊m��
	CEffect *pEffect;
	pEffect = new CEffect;

	// ������
	pEffect->Init();

	// �e�l�̃Z�b�g
	pEffect->m_nLife = EFFECT_LIFE;      // ����
	pEffect->SetPos(pos);                // ���W
	pEffect->SetSize(size);              // �T�C�Y
	pEffect->SetColor(col);              // �J���[
	pEffect->SetPriority(OBJTYPE_EFFECT); // �I�u�W�F�N�g�^�C�v 
	pEffect->BindTexture(pTexture);      // �e�N�X�`�����蓖��
	pEffect->SetAddMode(true);           // ���Z����
	return pEffect;
}

//*****************************
// �e�N�X�`���̃��[�h
//*****************************
HRESULT CEffect::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_PATH, &m_pTexture);
	//D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_PATH_BIRD, &m_pTexture[TEX_BIRD]);
	return S_OK;
}

//*****************************
// �e�N�X�`���̃A�����[�h
//*****************************
void CEffect::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************
// ����������
//*****************************
HRESULT CEffect::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	//// �e�N�X�`�����蓖��
	//BindTexture(m_pTexture);
	
	return S_OK;
}

//*****************************
// �I������
//*****************************
void CEffect::Uninit(void)
{
	CScene3d::Uninit();
}

//*****************************
// �X�V����
//*****************************
void CEffect::Update(void)
{

	//�T�C�Y�̎擾
	D3DXVECTOR3 size = GetSize();
	// �T�C�Y�̏k��
	size.x -= EFFECT_SUB_SIZE;
	size.y -= EFFECT_SUB_SIZE;
	// �T�C�X�̃Z�b�g
	SetSize(size);

	// �J���[�̎擾
	D3DXCOLOR col= GetColor();
	// A�l�̌��Z
	col.a -= EFFECT_SUB_ALPHA;
	// �J���[�̃Z�b�g
	SetColor(col);

	// ���������炷
	m_nLife--;

	// ���Ŕ���
	if (m_nLife <= 0 || size.x <= 0 /*|| col.a <= 0*/)
	{
		Uninit();
	}
}

//*****************************
// �`�揈��
//*****************************
void CEffect::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3d::Draw();
}
