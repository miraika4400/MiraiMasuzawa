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

	static CCamera *Create(int nNmPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void Shake(bool bRand = false);// ブレ

	D3DXVECTOR3 GetPos(void) { return m_posV; }
	// バックミラーフラグの取得
	bool GetBackMirror(void) { return m_bBackMirror; }

	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	D3DXMATRIX GetProjectionMtx(void) { return m_mtxProjection; }
	float GetAngle(void) { return m_fAngle; }

	void SetPhiDist(float fPhiDist) { m_fPhiDest = fPhiDist; }

	
	D3DXVECTOR3 GetposV(void) { return m_posV; }
	D3DXVECTOR3 GetposR(void) { return m_posR; }
private:
	void FovManage(void);// 視野角管理

	// メンバ変数
	D3DXVECTOR3 m_posV;            // カメラの座標
	D3DXVECTOR3 m_posR;            // 注視点
	D3DXVECTOR3 m_vecU;            // 上方向ベクトル
	D3DXVECTOR3 m_shake;           // ブレ
	D3DXVECTOR3 m_shakeDist;       // ブレ
	int m_nCntShake;               // ブレの方向転換カウント
	D3DXMATRIX  m_mtxProjection;   // プロジェクションマトリックス
	D3DXMATRIX  m_mtxView;         // ビューマトリックス
	int         m_nPlayerNum;      // どのプレイヤーのカメラか
	float       m_fAngle;
	bool        m_bBackMirror;     // バックミラー
	float       m_fViewFOV;        // 視野角
	float       m_fViewFOVDist;    // 視野角の目標値
	// 球面座標
	float m_fRad;         // 目標からの距離
	float m_fTheta;       // 角度シータ
	float m_fThetaDest;   // シータの目標値
	float m_fPhi;         // 角度ファイ
	float m_fPhiDest;     // ファイの目標値

	
	
};

#endif