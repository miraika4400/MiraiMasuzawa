//=============================================================================
//
// game�w�b�_ [game.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _GAME_H_
#define _GAME_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"


//=============================
// �O���錾
//=============================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CCamera;
class CPlayer;
class CLight;
class CCourse;
//*****************************
//�}�N����`
//*****************************
#define MAX_PLAYER_NUM 4

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CGame : public CScene
{
public:
	// �����o�֐�
	CGame();
	~CGame();
	static CGame *Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ���U���g�t���O�̎擾�E�Z�b�g
	static bool GetResultFlag(void) { return m_bResult; }
	static void SetResultFlag(bool bResult) { m_bResult = bResult; }

	static CCamera *GetCamera(int nIndex) { return m_pCamera[nIndex]; }  // �J�����̎擾����
	static CPlayer *GetPlayer(int nIndex) { return m_pPlayer[nIndex]; }  // �v���C���[�̎擾����
	static CCourse *GetCourse(void) { return m_pCourse; }                // �R�[�X�̎擾
	static int GetPlayerNum(void) { return m_nNumPlayer; }                 // �v���C���[�l���̎擾
	static void SetPlayerNum(int nNumPlayer){ m_nNumPlayer = nNumPlayer; } // �v���C���[�l���̃Z�b�g
private:
	// �����o�ϐ�
	static CCamera*m_pCamera[MAX_PLAYER_NUM]; // �J����
	static CPlayer*m_pPlayer[MAX_PLAYER_NUM]; // �v���C���[
	static CLight *m_pLight;                  // ���C�g
	static CCourse*m_pCourse;                 // �R�[�X
	static int m_nNumPlayer;                  // �v���C���[�l��
	static bool m_bResult;                           // ���U���g
	int m_nCntResult;                         // ���U���g�J�ڎ��̃J�E���g
};

#endif