//======================================================================
//
// [file.cpp] ファイルクラス
// Author : AYANO KUDO
//
//======================================================================
#include "file.h"
#include "item.h"
#include "accelfloor.h"

//**********************************************************************
// マクロ定義
//**********************************************************************
#define READ_BUFFER (128)                   // 読み込み用バッファ

//**********************************************************************
// 静的メンバ変数
//**********************************************************************
CFile* CFile::pInstance = NULL;

//======================================================================
// [CFile] コンストラクタ
//======================================================================
CFile::CFile()
{

}

//======================================================================
// [~CFile] デストラクタ
//======================================================================
CFile::~CFile()
{

}

//======================================================================
// [Create] インスタンス生成
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
// [Init] 初期化処理
//======================================================================
void CFile::Init(void)
{
}

//======================================================================
// [UnInit] 終了処理
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
// [Read] 読み込み
// 引数
// pFileName : 読み込むファイル名
//======================================================================
void CFile::Read(const char* pFileName)
{
    // 変数宣言
    char aRead[READ_BUFFER];    // 文字列読み込み用
    char aHead[READ_BUFFER];    // 比較用
    char aDie[READ_BUFFER];     // 不必要な文字を読み込む
    FILE *pFile = fopen(pFileName, "r");
    D3DXVECTOR3 pos;            // 読み込んだ位置
    D3DXVECTOR3 rot;            // 読み込んだ角度
    int type;                   // オブジェクトの種類
    int ntype;                  // オブジェクトごとのタイプ

    if (pFile)
    {// ファイル読み込み
        while (strcmp(aHead, "SCRIPT"))
        {
            fgets(aRead, sizeof(aRead), pFile);
            sscanf(aRead, "%s", &aHead);
        }
        // SCREPTを読み込んだ時
        if (strcmp(aHead, "SCRIPT") == 0)
        {
            while (strcmp(aHead, "END_SCRIPT") != 0)//END_SCRIPTを読み込むまでループ
            {
                // 文字列の読み込み
                fgets(aRead, sizeof(aRead), pFile);//位置行単位で文字列を読み込み
                sscanf(aRead, "%s", &aHead);//読み込んだテキストを比較用変数に代入

                if (strcmp(aHead, "OBJ_SET") == 0)// OBJSETを読み込んだ時
                {
                    while (strcmp(aHead, "END_OBJ_SET") != 0)// END_OBJSETを読み込むまでループ
                    {
                        // 文字列の読み込み
                        fgets(aRead, sizeof(aRead), pFile);//位置行単位で文字列を読み込み
                        sscanf(aRead, "%s", &aHead);//読み込んだテキストを比較用変数に代入
                                                    // 位置の設定
                        if (strcmp(aHead, "POS") == 0)
                        {
                            sscanf(aRead, "%s %f %f %f", &aDie, &pos.x, &pos.y, &pos.z);//位置を格納
                        }
                        // 角度の設定
                        if (strcmp(aHead, "ROT") == 0)
                        {
                            sscanf(aRead, "%s %f %f %f", &aDie, &rot.x, &rot.y, &rot.z);//角度を格納
                        }
                        if (strcmp(aHead, "TYPE") == 0)
                        {
                            sscanf(aRead, "%s %d", &aDie, &type);//種類を格納
                        }
                        if (strcmp(aHead, "OBJ_TYPE") == 0)
                        {
                            sscanf(aRead, "%s %d", &aDie, &ntype);//オブジェクトのタイプをを格納
                        }
                    }
                    // 読み込んだ情報を使用
                    if (type == (int)CScene::OBJTYPE_ITEM)
                    {
                        CItem::Create(pos, (CItem::ITEM_TYPE)ntype);
                    }
                    // 加速板
                    if (type == CScene::OBJTYPE_ACCELFLOOR)
                    {
                        // 加速板生成
                        CAccelFloor::Create(pos, rot);

                    }
                }
            }
        }
    }
    fclose(pFile);
}
