//=============================================================================
//
//  [pause.h]
// Author : Masuzawa Mirai
//
//=============================================================================

#ifndef _PAUSE_H_
#define _PAUSE_H_
//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

class CPolygon;

//***************************************************************************************
// 
//***************************************************************************************
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
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTS_MAX];
	CPolygon* m_pPolygon[PARTS_MAX];
	int m_nMenu;
};

#endif // !_TITEL_H_