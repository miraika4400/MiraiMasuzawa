//=============================================================================
//
// �����L���O�w�b�_ [ranking.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
#include "score.h"

//*****************************
//�O���錾
//*****************************
class CNumber;

//*****************************
//�}�N����`
//*****************************
#define MAX_RANK 8 // �����L���O�̏��ʐ�

//*****************************
//�N���X��`
//*****************************

//�����L���O�N���X
class CRanking : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CRanking();
	~CRanking();

	static CRanking *Create(void);      // �N���X����
	static void GetRanking(void);       // �����L���O�擾
	static int SetRanking(void);        // �����L���O�Z�b�g(�T�[�o�[�ʐM)
	static void SetRanking(int nScore); // �����L���O�Z�b�g(�Q�[����)

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
	
private:
	//============
	// �����o�ϐ�
	//============
	static int m_nNowScore;                              // ���̃v���C�̃X�R�A
	static CNumber* m_pNowScore[MAX_SCORE_DIGIT];        // �X�R�A�|���S��
	static int m_nRanking[MAX_RANK];                     // �X�R�A�f�[�^
	CNumber* m_apRankNumber[MAX_RANK];                   // �����L���O
	CNumber* m_apScoreNumber[MAX_RANK][MAX_SCORE_DIGIT]; // �X�R�A�|���S��
	int m_nCntFlash;                                     // �_�ŃJ�E���g

};

#endif