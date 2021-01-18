//=============================================================================
//
// score�w�b�_ [score.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//�O���錾
//*****************************
class CNumber;

//*****************************
//�}�N����`
//*****************************
#define MAX_SCORE_DIGIT 6  // �X�R�A�̍ő包��

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CScore : public CScene
{
public:
	// �����o�֐�
	CScore();
	~CScore();
	static CScore *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddScore(int nPoint) { m_nScore += nPoint; }
	static int GetScore(void) { return m_nScore; }
private:
	// �����o�ϐ�
	CNumber* m_apNumber[MAX_SCORE_DIGIT];
	static int m_nScore;

};

#endif