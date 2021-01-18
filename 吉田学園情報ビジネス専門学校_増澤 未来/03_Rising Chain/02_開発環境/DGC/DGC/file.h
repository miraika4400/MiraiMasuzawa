//======================================================================
//
// [file.h] ファイルクラス
// Author : AYANO KUDO
//
//======================================================================
#ifndef _FILE_H_
#define _FILE_H_
#include "main.h"

// ファイルデータ構造体
typedef struct
{
    D3DXVECTOR3 pos;            // 読み込んだ位置
    D3DXVECTOR3 rot;            // 読み込んだ角度
    int type;                   // オブジェクトの種類
    int ntype;                  // オブジェクトごとのタイプ

}FILE_DATA;

// クラス定義
// ファイルクラス
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
