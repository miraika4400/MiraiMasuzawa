//=============================================================================
//
//  [titlemenu.h]
// Author : Masuzawa Mirai
//
//=============================================================================

#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_
//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

class CPolygon;

//***************************************************************************************
// クラス定義
//***************************************************************************************

// ポーズクラス
class CPause
{
public:
	typedef enum
	{
		BACK = 0,
		RESUME,
		RESTART,
		EXIT,
		PARTS_MAX
	}MENU_PARTS;
	
	CPause();
	~CPause();
	static CPause* Create(void);
	static HRESULT Load(void);
	static void CPause::UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTS_MAX]; // テクスチャ
	CPolygon* m_pPolygon[PARTS_MAX];                  // ポリゴン
	int m_nMenu;                                      // メニュー選択用
	bool m_bMove;                                     // 移動フラグ
};

#endif // !_TITEL_H_