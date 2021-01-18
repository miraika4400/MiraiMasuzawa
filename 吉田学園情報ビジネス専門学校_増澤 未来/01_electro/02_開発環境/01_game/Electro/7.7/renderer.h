//=============================================================================
//
// �����_���[�w�b�_ [renderer.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RENDERER_H_
#define _RENDERER_H_

//�C���N���[�h
#include "main.h"

//=============================
//�N���X��`
//=============================

//�����_���[�N���X
class CRenderer
{
public:
	//============
	// �����o�֐�
	//============
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);                     // ������
	void Uninit(void);                                         // �I��
	void Update(void);                                         // �X�V
	void Draw(void);                                           // �`��
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; } // �f�o�C�X�̎擾

	D3DXCOLOR GetBuffColor(void) { return m_buffCol; }         // �o�b�N�o�b�t�@�̃J���[�̎擾
	void      SetBuffColor(D3DXCOLOR col) { m_buffCol = col; } // �o�b�N�o�b�t�@�̃J���[�̐ݒ�
private:

#ifdef _DEBUG
	void DrawFPS(void); //FPS���̕\��
#endif
	
	//============
	// �����o�ϐ�
	//============
	PDIRECT3D9        m_pD3D;       // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice; // Device�I�u�W�F�N�g(�`��ɕK�v)
	D3DXCOLOR         m_buffCol;    // �o�b�N�o�b�t�@�J���[

#ifdef _DEBUG
	LPD3DXFONT        m_pFont;      // �t�H���g�ւ̃|�C���^
#endif
};

#endif