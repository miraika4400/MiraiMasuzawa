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
#include "game.h"

//=============================
//�N���X��`
//=============================

//�����_���[�N���X
class CRenderer
{
public:
	//�����o�֐�
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	bool SetUpViewport(int nNumber);
	D3DVIEWPORT9 GetViewPort(int nCount) { return m_view_port[nCount]; };

private:
#ifdef _DEBUG
	//�����o�֐�
	void DrawFPS(void);
#endif
	//�����o�ϐ�
	PDIRECT3D9        m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;	    // Device�I�u�W�F�N�g(�`��ɕK�v)
	D3DVIEWPORT9 m_view_port[MAX_PLAYER_NUM];			// �r���[�|�[�g
#ifdef _DEBUG
	LPD3DXFONT				m_pFont;	// �t�H���g�ւ̃|�C���^
#endif
};

#endif