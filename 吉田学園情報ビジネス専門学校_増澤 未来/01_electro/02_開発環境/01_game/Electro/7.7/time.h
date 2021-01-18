//=============================================================================
//
// time�w�b�_ [time.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _TIME_H_
#define _TIME_H_

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
#define MAX_TIME_NUM 4 // �^�C���ɕK�v�ȃi���o�[�̐�

//*****************************
//�N���X��`
//*****************************

//�^�C���N���X
class CTime : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CTime();
	~CTime();

	//static
	static CTime *Create(void);                                             // �N���X����
	static int GetTime(void) { return m_nTime; }                            // �^�C���̎擾
	static void SetPauseTime(void) { m_nPauseCurrentTime = timeGetTime(); } // �|�[�Y��ʂɓ��������Ԃ̃Z�b�g

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	
private:
	//============
	// �����o�ϐ�
	//============
	static int m_nTime;                // �o�ߎ���
	static int m_nPauseCurrentTime;    // �|�[�Y�ɓ���������
	CNumber* m_apNumber[MAX_TIME_NUM]; // �^�C���`��p�̃|���S��
	int m_nPauseTime;                  // �|�[�Y��ʒ��̃^�C��
	int m_nCurrentTime;                // �J�n����
};

#endif