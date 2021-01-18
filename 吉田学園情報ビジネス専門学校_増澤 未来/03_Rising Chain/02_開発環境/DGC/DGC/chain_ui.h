//=============================================================================
//
// chain_ui�w�b�_ [chain_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CHAIN_UI_H_
#define _CHAIN_UI_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// �O���錾
//*****************************
class CNumber;
class CPolygon;

//*****************************
// �}�N����`
//*****************************
#define MAX_CHAIN_DIGIT 3

//*****************************
// �N���X��`
//*****************************

// �`�F�C����UI���S�N���X
class CChainUi : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CChainUi();
	~CChainUi();

	static CChainUi *Create(int nPlayerNum); // �N���X����
	static HRESULT Load(void);               // �e�N�X�`�����[�h
	static void Unload(void);                // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;       // �e�N�X�`��
	CNumber*m_apNumber[MAX_CHAIN_DIGIT]; // �`�F�C������
	int m_nPlayerNum;                    // �v���C���[�ԍ�
	int m_nDegit;                        // �\������
	D3DXVECTOR3 m_pos;                   // ���S���W
	int m_nChain;                        // �`�F�C����
	CPolygon*m_pPolygon;                 // �����摜
	D3DXVECTOR3 m_resultPos;       // ���U���g���̍��W
	D3DXVECTOR3 m_resultSize;      // ���U���g���̃T�C�Y
};

#endif