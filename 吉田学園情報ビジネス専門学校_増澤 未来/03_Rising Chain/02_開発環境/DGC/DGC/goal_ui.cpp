////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "goal_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// �}�N����`
//**********************************
#define TEXTURE_PATH "./data/Textures/GOAL.png"    // �e�N�X�`���̃p�X
#define SIZE D3DXVECTOR3( 960.0f / 2.5f, 276.0f/2.5f, 0.0f)   // �T�C�Y*�摜�̉𑜓x�ɍ��킹��

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // �v���C���[4

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CGoalUi::m_pTexture = NULL;

//=============================
// �R���X�g���N�^
//=============================
CGoalUi::CGoalUi() :CScene2d(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nPlayerNum = 0; // �v���C���[�ԍ�
}

//=============================
// �f�X�g���N�^
//=============================
CGoalUi::~CGoalUi()
{
}

//=============================
// �N���G�C�g
//=============================
CGoalUi * CGoalUi::Create(int nPlayerNum)
{
	// �������̊m��
	CGoalUi *pRank = new CGoalUi;
	// �v���C���[�ԍ��̑��
	pRank->m_nPlayerNum = nPlayerNum;
	// ������
	pRank->Init();
	return pRank;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CGoalUi::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CGoalUi::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// ���ʂ̊Ǘ�
//==================================
void CGoalUi::RankManage(void)
{
	// �v���C���[�z��
	CPlayer*pPlayer[MAX_PLAYER_NUM] = {};

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();

	// �z��Ƀv���C���[���i�[
	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (CGame::GetPlayer(nCnt) != NULL)
		{
			pPlayer[nCnt] = CGame::GetPlayer(nCnt);
		}
	}

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (pPlayer[nCnt] != NULL)
		{
			for (int nCnt2 = nCnt + 1; nCnt2 < nNumPlayer; nCnt2++)
			{
				if (pPlayer[nCnt2] != NULL)
				{
					if (!pPlayer[nCnt]->GetGoalFlag() && !pPlayer[nCnt2]->GetGoalFlag())
					{
						if (pPlayer[nCnt]->GetCheckNum() < pPlayer[nCnt2]->GetCheckNum())
						{// �`�F�b�N�|�C���g�̐��Ń\�[�g
							CPlayer*pSave = pPlayer[nCnt];
							pPlayer[nCnt] = pPlayer[nCnt2];
							pPlayer[nCnt2] = pSave;
						}
						else if (pPlayer[nCnt]->GetCheckNum() == pPlayer[nCnt2]->GetCheckNum())
						{// �`�F�b�N�|�C���g�����ꏏ��������
							D3DXVECTOR3 checkPos = CCheckPoint::GetCheckPoint()->GetCollision(pPlayer[nCnt]->GetCheckNum())->GetPos();

							// �`�F�b�N�|�C���g�Ƃ̋����Ń\�[�g
							float fDistance = sqrtf(powf(checkPos.x - pPlayer[nCnt]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt]->GetPos().z, 2));
							if (fDistance > sqrtf(powf(checkPos.x - pPlayer[nCnt2]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt2]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt2]->GetPos().z, 2)))
							{
								CPlayer*pSave = pPlayer[nCnt];
								pPlayer[nCnt] = pPlayer[nCnt2];
								pPlayer[nCnt2] = pSave;
							}
						}
					}
				}
			}
		}
	}

	// �z��̏��Ԃ�����
	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (pPlayer[nCnt] != NULL)
		{
			pPlayer[nCnt]->SetRank(nCnt + 1);
		}
	}
}

//=============================
// ����������
//=============================
HRESULT CGoalUi::Init(void)
{
	CScene2d::Init();
	// �e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

	// ���W�̐ݒ�

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();
	// �v���C���[�ԍ��ɂ���ĕ���
	switch (m_nPlayerNum)
	{
	case 0:
		// �v���C���[1

		if (nNumPlayer == 1)
		{// �����Ȃ�
			SetPos(POS_PLAYER1_1);
		}
		else if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER1_2);
		}
		else
		{// �l����
			SetPos(POS_PLAYER1_4);
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER2_2);
		}
		else
		{// �l����
			SetPos(POS_PLAYER2_4);
		}
		break;
	case 2:
		// �v���C���[3
		SetPos(POS_PLAYER3);
		break;
	case 3:
		// �v���C���[4
		SetPos(POS_PLAYER4);
		break;
	default:
		break;
	}

	if (nNumPlayer<=2)
	{
		// �T�C�Y�̐ݒ�
		SetSize(SIZE);
	}
	else
	{
		// �T�C�Y�̐ݒ�
		SetSize(SIZE/2);
	}

	return S_OK;
}

//=============================
// �I������
//=============================
void CGoalUi::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CGoalUi::Update(void)
{
	if (CGame::GetResultFlag())
	{// ���U���g��Ԃ̎�
		// UI������
		Uninit();
	}
}


//=============================
// �`�揈��
//=============================
void CGoalUi::Draw(void)
{
	CScene2d::Draw();
}