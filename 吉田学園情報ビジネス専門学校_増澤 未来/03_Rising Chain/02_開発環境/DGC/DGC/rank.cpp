////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "rank.h"
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
#define TEXTURE_PATH "./data/Textures/Ranking�@number.png"    // �e�N�X�`���̃p�X
#define MAX_ANIMATION_X 4                                     // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1                                     // �A�j���[�V������ �c
#define SIZE D3DXVECTOR3( 192.25f / 2.5f,110.0f/2.5f, 0.0f)   // �T�C�Y*�摜�̉𑜓x�ɍ��킹��

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 70 ,  SCREEN_HEIGHT    - 70 , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT/2) - 70 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 70 , (SCREEN_HEIGHT/2) - 50 , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 70 ,  SCREEN_HEIGHT    - 70 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT/2) - 50 , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 70 , (SCREEN_HEIGHT)   - 50 , 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 70 , (SCREEN_HEIGHT)   - 50 , 0.0f)  // �v���C���[4

// ���U���g���̃T�C�Y
#define RESULT_SIZE D3DXVECTOR3( 192.25f / 1.5f,110.0f / 1.5f, 0.0f)   // �摜�̉𑜓x�ɍ��킹��
// ���U���g���̈ʒu
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   - 60.0f, 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // �v���C���[3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // �v���C���[4

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CRank::m_pTexture = NULL;

//=============================
// �R���X�g���N�^
//=============================
CRank::CRank() :CScene2d(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nCntAnim = 0;           // �A�j���[�V�����J�E���g
	m_nAnimX = 0;             // �A�j���[�V����X��
	m_nAnimY = 0;             // �A�j���[�V����Y��
	m_nPlayerNum = 0;         // �v���C���[�ԍ�
	m_resultPos = VEC3_ZERO;  // ���U���g���̃|�X
	m_resultSize = VEC3_ZERO; // ���U���g���̃T�C�Y
}

//=============================
// �f�X�g���N�^
//=============================
CRank::~CRank()
{
}

//=============================
// �N���G�C�g
//=============================
CRank * CRank::Create(int nPlayerNum)
{
	// �������̊m��
	CRank *pRank = new CRank;
	// �v���C���[�ԍ��̑��
	pRank->m_nPlayerNum = nPlayerNum;
	// ������
	pRank->Init();
	return pRank;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CRank::Load(void)
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
void CRank::Unload(void)
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
void CRank::RankManage(void)
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
			for (int nCnt2 = 0; nCnt2 < nNumPlayer; nCnt2++)
			{
				if (pPlayer[nCnt2] != NULL)
				{
					if (!pPlayer[nCnt]->GetGoalFlag() && !pPlayer[nCnt2]->GetGoalFlag())
					{
						if (pPlayer[nCnt]->GetCheckNum() > pPlayer[nCnt2]->GetCheckNum())
						{// �`�F�b�N�|�C���g�̐��Ń\�[�g
							CPlayer*pSave = pPlayer[nCnt];
							pPlayer[nCnt] = pPlayer[nCnt2];
							pPlayer[nCnt2] = pSave;
							continue;
						}
						else if (pPlayer[nCnt]->GetCheckNum() == pPlayer[nCnt2]->GetCheckNum())
						{// �`�F�b�N�|�C���g�����ꏏ��������
							D3DXVECTOR3 checkPos = CCheckPoint::GetCheckPoint()->GetCollision(pPlayer[nCnt]->GetCheckNum())->GetPos();

							// �`�F�b�N�|�C���g�Ƃ̋����Ń\�[�g
							float fDistance = sqrtf(powf(checkPos.x - pPlayer[nCnt]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt]->GetPos().z, 2));
							if (fDistance < sqrtf(powf(checkPos.x - pPlayer[nCnt2]->GetPos().x, 2) + powf(checkPos.y - pPlayer[nCnt2]->GetPos().y, 2) + powf(checkPos.z - pPlayer[nCnt2]->GetPos().z, 2)))
							{
								CPlayer*pSave = pPlayer[nCnt];
								pPlayer[nCnt] = pPlayer[nCnt2];
								pPlayer[nCnt2] = pSave;
								continue;
							}
						}
					}
				}
			}
		}
	}

	// �z��̏��Ԃ����̂܂܏���
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
HRESULT CRank::Init(void)
{
	CScene2d::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nAnimX = 0;     // �A�j���[�V����X��
	m_nAnimY = 0;     // �A�j���[�V����Y��

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX     , fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);
	// UV���W�Z�b�g
	SetTextureUV(uv);

	// �T�C�Y�̐ݒ�
	SetSize(SIZE);
	
	m_resultSize = RESULT_SIZE;      // ���U���g���̃T�C�Y
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
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if(nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER1_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// �l����
			SetPos(POS_PLAYER1_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER2_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// �l����
			SetPos(POS_PLAYER2_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// �v���C���[3
		SetPos(POS_PLAYER3);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// �v���C���[4
		SetPos(POS_PLAYER4);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}
	

	return S_OK;
}

//=============================
// �I������
//=============================
void CRank::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CRank::Update(void)
{
	if (!CGame::GetResultFlag())
	{
		m_nAnimX = CGame::GetPlayer(m_nPlayerNum)->GetRank() - 1;     // �A�j���[�V����X��

		// UV���W�̐ݒ�
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);
		// UV���W�Z�b�g
		SetTextureUV(uv);
	}
	else
	{// ���U���g��Ԃ̎�
		SetPos(m_resultPos);
		SetSize(m_resultSize);
	}
}


//=============================
// �`�揈��
//=============================
void CRank::Draw(void)
{
	CScene2d::Draw();
}