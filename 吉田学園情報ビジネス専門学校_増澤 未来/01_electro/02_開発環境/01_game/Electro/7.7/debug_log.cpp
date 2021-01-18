////////////////////////////////////////////////////
//
//    TitleLogo�̏���[TitleLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "debug_log.h"
#include "manager.h"
#include "renderer.h"
char CDebugLog::m_aStr[DEBUG_LOG_MAX_STRING] = {};
LPD3DXFONT CDebugLog::m_pFont = NULL;

//=============================
// �R���X�g���N�^
//=============================
CDebugLog::CDebugLog()
{
	
}

//=============================
// �f�X�g���N�^
//=============================
CDebugLog::~CDebugLog()
{
}

//=============================
// ����������
//=============================
HRESULT CDebugLog::Init(void)
{
	memset(&m_aStr, 0, sizeof(m_aStr));
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	return E_NOTIMPL;
}

//=============================
// �I������
//=============================
void CDebugLog::Uninit(void)
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================
// ��������
//=============================
void CDebugLog::Print(const char * format, ...)
{
	va_list arg;
	va_start(arg, format);

	bool bVariable = false;// �ϐ����ǂ����̃t���O

	// �ψ����̔���
	for (const char* chC = format; *chC != '\0'; chC++)
	{
		// %���ǂݍ��܂ꂽ�玟�͕ϐ�������
		if (chC[0] == '%')
		{
			// �ϐ��t���O�𗧂Ă�
			bVariable = true;
		}

		if (bVariable)
		{
			if (chC[0] == 'd')
			{//%d�iint�j
				sprintf(m_aStr, "%s%d", m_aStr, va_arg(arg, int));
			}
			else if (chC[0] == 'f')
			{//%f�ifloat�j
				sprintf(m_aStr, "%s%f", m_aStr, va_arg(arg, float));
			}
			else if (chC[0] == 'c')
			{//%c�ichar�j
				sprintf(m_aStr, "%s%c", m_aStr, va_arg(arg, char));

			}
			else if (chC[0] == 's')
			{//%s�i*char�j
				sprintf(m_aStr, "%s%s", m_aStr, va_arg(arg, const char*));
			}
			else
			{// �ϐ�����Ȃ��Ƃ�
				sprintf(m_aStr, "%s%c", m_aStr, chC[0]);
			}
			// �ϐ��t���O�̏�����
			bVariable = false;
		}
		else
		{// �ϐ�����Ȃ��Ƃ�
			sprintf(m_aStr, "%s%c", m_aStr, chC[0]);
		}
	}
	va_end(arg);
}

//=============================
// �`�揈��
//=============================
void CDebugLog::Draw(void)
{
	RECT rect = { 20, 70, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}
