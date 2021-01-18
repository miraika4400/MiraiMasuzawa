//=============================================================================
//
// evolution_effect�w�b�_ [evolution_effect.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _EVOLUTION_EFFECT_H_
#define _EVOLUTION_EFFECT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �O���錾
//*****************************


//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CEvoEffect : public CModel
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	//�����o�֐�
	//==========================
	CEvoEffect();
	~CEvoEffect();
	static CEvoEffect *Create(int nPlayerNum);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static Model m_model;   // ���f���\����
	int m_nPlayerNum;       // �v���C���[�ԍ�
	int m_nLife;            // ����
	float m_fAlfa;          // �A���t�@�l
	D3DXCOLOR m_col;        // �J���[
	D3DXCOLOR m_distCol;    // �J���[�ڕW�l
};
#endif