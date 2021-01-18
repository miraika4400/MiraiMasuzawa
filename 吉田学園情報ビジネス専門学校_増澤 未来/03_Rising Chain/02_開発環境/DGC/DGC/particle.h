//=============================================================================
//
// particle�w�b�_ [particle.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CParticle : public CScene2d
{
public:
	typedef enum
	{
		PARTICLE_BOX = 0,
		PARTICLE_MAX,
	}PARTICLE_TYPE;

	//�����o�֐�
	CParticle();
	~CParticle();
	static CParticle *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const D3DCOLOR col, const PARTICLE_TYPE type = PARTICLE_BOX);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTICLE_MAX]; // �e�N�X�`��
	D3DXVECTOR3 m_move; // �ړ���
	int m_nLife;        // ����
	PARTICLE_TYPE m_type; // �p�[�e�B�N���̎��
};

#endif