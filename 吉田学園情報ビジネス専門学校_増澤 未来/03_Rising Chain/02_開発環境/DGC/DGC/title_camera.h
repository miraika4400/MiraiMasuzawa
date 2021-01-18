//=============================================================================
//
// title_cameraヘッダ [title_camera.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TITLE_CAMERA_H_
#define _TITLE_CAMERA_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// クラス定義
//*****************************

//カメラクラス
class CTitleCamera
{
public:
	//メンバ関数
	CTitleCamera();
	~CTitleCamera();

	static CTitleCamera *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);


	D3DXVECTOR3 GetposV(void) { return m_posV; }
	D3DXVECTOR3 GetposR(void) { return m_posR; }
private:
	void Direction(void);
	void LoadText(void);
	// メンバ変数
	D3DXVECTOR3 m_posV;            // カメラの座標
	D3DXVECTOR3 m_posR;            // 注視点
	D3DXVECTOR3 m_vecU;            // 上方向ベクトル
	D3DXMATRIX  m_mtxProjection;   // プロジェクションマトリックス
	D3DXMATRIX  m_mtxView;         // ビューマトリックス
	int m_nNumPoint;               // チェックポイントの数
	D3DXVECTOR3 m_pointPos[256];   // チェックポイントの座標
	int m_nNum;                    // 座標番号
	int m_nNumNext;                // 次の座標番号
	float m_fAngle;                // 向き
	float m_fAngleDist;            // 向きの目標値

};

#endif