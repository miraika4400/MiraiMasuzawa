//=============================================================================
//
// polygon�w�b�_ [polygon.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _POLYGON_H_
#define _POLYGON_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"

//*****************************
//�N���X��`
//*****************************

//�i���o�[�N���X
class CPolygon
{
public:
	// �����o�֐�
	CPolygon();
	~CPolygon();
	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetVertexPos(D3DXVECTOR3 pos[NUM_VERTEX]);
	void SetColor(const D3DXCOLOR col);
	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
private:

	// �����o�ϐ�
	LPDIRECT3DTEXTURE9        m_pTexture;	 // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;	 // ���_�o�b�t�@�ւ̃|�C��
	D3DXVECTOR3 m_pos;  // ���W
	D3DXVECTOR3 m_size;  // �T�C�Y
};

#endif