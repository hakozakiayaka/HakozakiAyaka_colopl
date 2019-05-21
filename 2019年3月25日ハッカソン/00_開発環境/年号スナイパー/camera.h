//=============================================================================
//
// カメラ処理 [camera.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================================================================
//	前方宣言
//=============================================================================
class CPlayer;

//*****************************************************************************
// カメラのクラス
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERATYPE_NONE = 0,
		CAMERATYPE_TITLE,
		CAMERATYPE_GAME,
		CAMERATYPE_RANKING,
		CAMERATYPE_MAX

	}CAMERA_MODE;
	CCamera();									//	コンストラクタ
	~CCamera();									//	デストラクタ
	HRESULT Init(CAMERA_MODE CameraMode);		//	カメラの初期化
	void Uninit(void);							//	カメラの終了処理
	void Update(void);							//	カメラの更新処理
	void SetCamera(void);						//	カメラの設定処理
	static D3DXVECTOR3 GetCameraRotate(void);	//	カメラの回転の取得
private:

	CAMERA_MODE m_CameraType;					// カメラの種類

	D3DXVECTOR3	m_posV;							// 現在の視点
	D3DXVECTOR3	m_posR;							// 現在の注視点
	D3DXVECTOR3	m_posVDest;						// 目的の視点
	D3DXVECTOR3	m_posRDest;						// 目的の注視点
	D3DXVECTOR3	m_vecU;							// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;				// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;						// ビューマトリックス
	static D3DXVECTOR3	m_rot;					// 現在の向き　
	D3DXVECTOR3	m_rotDest;						// 目的の向き
	float		m_fLength;						// 注視点と視点の距離
	float		m_fCameraSpeed;					// カメラの動く速さ
	float		m_fContCamSpeed;				// カメラの動く速さ上限
	float		m_fCameraRotSpeed;				// カメラの回転動く速さ
	int			m_nCameraMove[3];				//	カメラを動かす
	int			m_nGameCounter;					//	ゲームのカウント
	bool		m_bCameraMode;					//	カメラのモード切替
	D3DXVECTOR3 m_AddCamera;					//	カメラの加算
};
#endif
