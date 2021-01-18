//======================================================================
//
// [file.cpp] �t�@�C���N���X
// Author : AYANO KUDO
//
//======================================================================
#include "file.h"
#include "item.h"
#include "accelfloor.h"

//**********************************************************************
// �}�N����`
//**********************************************************************
#define READ_BUFFER (128)                   // �ǂݍ��ݗp�o�b�t�@

//**********************************************************************
// �ÓI�����o�ϐ�
//**********************************************************************
CFile* CFile::pInstance = NULL;

//======================================================================
// [CFile] �R���X�g���N�^
//======================================================================
CFile::CFile()
{

}

//======================================================================
// [~CFile] �f�X�g���N�^
//======================================================================
CFile::~CFile()
{

}

//======================================================================
// [Create] �C���X�^���X����
//======================================================================
CFile *CFile::Create(void)
{
    if (!pInstance)
    {
    pInstance = new CFile;
    pInstance->Init();
    }
    return  pInstance;;
}

//======================================================================
// [Init] ����������
//======================================================================
void CFile::Init(void)
{
}

//======================================================================
// [UnInit] �I������
//======================================================================
void CFile::Uninit(void)
{
    if (pInstance)
    {
        delete pInstance;
        pInstance = NULL;
    }
}

//======================================================================
// [Read] �ǂݍ���
// ����
// pFileName : �ǂݍ��ރt�@�C����
//======================================================================
void CFile::Read(const char* pFileName)
{
    // �ϐ��錾
    char aRead[READ_BUFFER];    // ������ǂݍ��ݗp
    char aHead[READ_BUFFER];    // ��r�p
    char aDie[READ_BUFFER];     // �s�K�v�ȕ�����ǂݍ���
    FILE *pFile = fopen(pFileName, "r");
    D3DXVECTOR3 pos;            // �ǂݍ��񂾈ʒu
    D3DXVECTOR3 rot;            // �ǂݍ��񂾊p�x
    int type;                   // �I�u�W�F�N�g�̎��
    int ntype;                  // �I�u�W�F�N�g���Ƃ̃^�C�v

    if (pFile)
    {// �t�@�C���ǂݍ���
        while (strcmp(aHead, "SCRIPT"))
        {
            fgets(aRead, sizeof(aRead), pFile);
            sscanf(aRead, "%s", &aHead);
        }
        // SCREPT��ǂݍ��񂾎�
        if (strcmp(aHead, "SCRIPT") == 0)
        {
            while (strcmp(aHead, "END_SCRIPT") != 0)//END_SCRIPT��ǂݍ��ނ܂Ń��[�v
            {
                // ������̓ǂݍ���
                fgets(aRead, sizeof(aRead), pFile);//�ʒu�s�P�ʂŕ������ǂݍ���
                sscanf(aRead, "%s", &aHead);//�ǂݍ��񂾃e�L�X�g���r�p�ϐ��ɑ��

                if (strcmp(aHead, "OBJ_SET") == 0)// OBJSET��ǂݍ��񂾎�
                {
                    while (strcmp(aHead, "END_OBJ_SET") != 0)// END_OBJSET��ǂݍ��ނ܂Ń��[�v
                    {
                        // ������̓ǂݍ���
                        fgets(aRead, sizeof(aRead), pFile);//�ʒu�s�P�ʂŕ������ǂݍ���
                        sscanf(aRead, "%s", &aHead);//�ǂݍ��񂾃e�L�X�g���r�p�ϐ��ɑ��
                                                    // �ʒu�̐ݒ�
                        if (strcmp(aHead, "POS") == 0)
                        {
                            sscanf(aRead, "%s %f %f %f", &aDie, &pos.x, &pos.y, &pos.z);//�ʒu���i�[
                        }
                        // �p�x�̐ݒ�
                        if (strcmp(aHead, "ROT") == 0)
                        {
                            sscanf(aRead, "%s %f %f %f", &aDie, &rot.x, &rot.y, &rot.z);//�p�x���i�[
                        }
                        if (strcmp(aHead, "TYPE") == 0)
                        {
                            sscanf(aRead, "%s %d", &aDie, &type);//��ނ��i�[
                        }
                        if (strcmp(aHead, "OBJ_TYPE") == 0)
                        {
                            sscanf(aRead, "%s %d", &aDie, &ntype);//�I�u�W�F�N�g�̃^�C�v�����i�[
                        }
                    }
                    // �ǂݍ��񂾏����g�p
                    if (type == (int)CScene::OBJTYPE_ITEM)
                    {
                        CItem::Create(pos, (CItem::ITEM_TYPE)ntype);
                    }
                    // ������
                    if (type == CScene::OBJTYPE_ACCELFLOOR)
                    {
                        // ��������
                        CAccelFloor::Create(pos, rot);

                    }
                }
            }
        }
    }
    fclose(pFile);
}
