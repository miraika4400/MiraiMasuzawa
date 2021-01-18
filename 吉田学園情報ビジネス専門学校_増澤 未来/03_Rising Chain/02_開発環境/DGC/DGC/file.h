//======================================================================
//
// [file.h] �t�@�C���N���X
// Author : AYANO KUDO
//
//======================================================================
#ifndef _FILE_H_
#define _FILE_H_
#include "main.h"

// �t�@�C���f�[�^�\����
typedef struct
{
    D3DXVECTOR3 pos;            // �ǂݍ��񂾈ʒu
    D3DXVECTOR3 rot;            // �ǂݍ��񂾊p�x
    int type;                   // �I�u�W�F�N�g�̎��
    int ntype;                  // �I�u�W�F�N�g���Ƃ̃^�C�v

}FILE_DATA;

// �N���X��`
// �t�@�C���N���X
class CFile
{
    CFile();
    static CFile* pInstance;
public:
    ~CFile();
    static CFile* Create(void);

    void Init(void);
    void Uninit(void);

    void Read(const char* pFileName);

    static CFile *GetInstance(void) { return pInstance;}
};
#endif // !_FILE_H_
