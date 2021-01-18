//=============================================================================
//
// ���f������ [model.h]
// Author : ���V����
//
//=============================================================================
#ifndef _MODEL_HIERARCHY_H_
#define _MODEL_HIERARCHY_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"
#include "model.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_PARTS_NUM 15

//=============================================================================
//���f���N���X
//=============================================================================
class CModelHierarchy : public CScene
{
public:
	//=========================================================================
	//�����o�֐��錾
	//=========================================================================
	CModelHierarchy(int nPliority = OBJTYPE_NONE);
	~CModelHierarchy();

	static void LoadModels(char * pPath, CModel::Model *model, int * pNumModel);
	void LoadHierarchy(CModel::Model *model, char*pPath);
	HRESULT Init(void);
	HRESULT Init(int nNumParts, CModel::Model *model, char*pPath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModelData(CModel::Model *pModel);
	CModel::Model * GetModelData(void) { return &m_model[0]; }
	void SetPartsNum(int nNumParts) { m_nNumParts = nNumParts; }
	int GetPartsNum(void) { return m_nNumParts; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void)const { return m_pos; }
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void)const { return m_size; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void)const { return m_rot; }
private:
	//=========================================================================
	//�����o�ϐ��錾
	//=========================================================================
	CModel::Model m_model[MAX_PARTS_NUM]; // ���f�����
	D3DXVECTOR3 m_pos;     // ���W
	D3DXVECTOR3 m_rot;     // ��]
	D3DXVECTOR3 m_size;    // �T�C�Y
	D3DXMATERIAL m_defMat[MAX_PARTS_NUM][128]; // �}�e���A���ۊǗp
	int m_nNumParts;    // �p�[�c��
};
#endif 