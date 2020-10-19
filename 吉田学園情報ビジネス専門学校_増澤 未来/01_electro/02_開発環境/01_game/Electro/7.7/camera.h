//=============================================================================
//
// cameraヘッダ [camera.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// クラス定義
//*****************************

//カメラクラス
class CCamera
{
public:
	//メンバ関数
	CCamera();
	~CCamera();

	static CCamera *Create();
	static void Release(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	D3DXVECTOR3 GetPos(void) { return m_posV; }
	
	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	D3DXMATRIX GetProjectionMtx(void) { return m_mtxProjection; }

	float GetCameraWidth(void) { return (float)(SCREEN_WIDTH * 2) + m_fViewAriaX; }
	float GetCameraHeight(void) { return (float)(SCREEN_HEIGHT * 2) + m_fViewAriaY; }
private:
	// メンバ変数
	static CCamera *m_pCamera;
	D3DXVECTOR3 m_posV;//カメラの座標
	D3DXVECTOR3 m_posR;//注視点
	D3DXVECTOR3 m_vecU;//上方向ベクトル
	D3DXMATRIX  m_mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX  m_mtxView;//ビューマトリックス
	float       m_fViewAriaX; // 視野の広さ操作用
	float       m_fViewAriaY; // 視野の広さ操作用
};

#endif