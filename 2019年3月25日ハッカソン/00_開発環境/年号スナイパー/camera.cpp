//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : YUTARO ABE
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_POSV_X			(44.0f)		// カメラの初期視点[]
#define CAMERA_POSV_Y			(120.0f)	// カメラの初期視点[高さ]
#define CAMERA_POSV_Z			(-400.0f)	// カメラの初期視点[奥行]

#define CAMERA_POSR_X			(44.0f)		// カメラの初期注視点[]
#define CAMERA_POSR_Y			(128.0f)	// カメラの初期注視点[高さ]
#define CAMERA_POSR_Z			(0.0f)		// カメラの初期注視点[奥行]
#define CAMERA_m_rot_SPEED		(0.025f)	// カメラの回転速度
#define CAMERA_ZOOMIN_MAX		(50.0f)		// カメラの最大拡大距離
#define CAMERA_ZOOMOUT_MAX		(500.0f)	// カメラの最大縮小距離

#define CAMERA_AXIS_MIN			(100.0f)		//	傾きの値
#define CAMERA_SENSITEIVTY_X	(0.00004f)	//	傾きの値X
#define CAMERA_SENSITEIVTY_Y	(0.00004f)	//	傾きの値Y

#define VIEW_Y_MIN				(-0.8f)					//	カメラの角度の最小値
#define VIEW_Y_MAX				(D3DX_PI * 0.05f)		//	カメラの角度の最大値

//*****************************************************************************
//	静的メンバ変数の初期化
//*****************************************************************************
D3DXVECTOR3 CCamera::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
//	コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = m_rot;
	m_fLength = 0;
	m_fCameraSpeed = 0.0f;
	m_fContCamSpeed = 0.0f;
	m_fCameraRotSpeed = 0.0f;
	m_CameraType = CAMERATYPE_NONE;
	for (int nCount = 0; nCount < 3; nCount++)
	{//確保分回す
		m_nCameraMove[nCount] = 0;		//	カメラを加算する変数
	}
	m_nGameCounter =0;					//	ゲームのカウント
	m_bCameraMode = false;				//	カメラのモード切替
	m_AddCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
//	コンストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(CAMERA_MODE CameraMode)
{
	m_CameraType = CameraMode;	//	カメラの種類の設定
	switch (m_CameraType)
	{
	case CAMERATYPE_NONE:
		break;
	case CAMERATYPE_TITLE:
		m_posR = D3DXVECTOR3(0.0f, 25, 615);
		m_posV = D3DXVECTOR3(0.0f, 70, -385);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, atan2f((m_posV.x - m_posR.x), (m_posV.z - m_posR.z)), 0.0f);
		m_rotDest = m_rot;
		m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));

		m_fContCamSpeed = 5.0f;				// カメラの速さ上限の初期設定
		m_fCameraSpeed = 5.0f;				// カメラの速さの初期設定
		m_fCameraRotSpeed = 0.025f;			// カメラの回転動く速さ
		for (int nCount = 0; nCount < 3; nCount++)
		{//確保分回す
			m_nCameraMove[nCount] = 0;					//	カメラを加算する変数
		}
		m_nGameCounter = 0;					//	ゲームのカウント
		break;
	case CAMERATYPE_GAME:
		m_AddCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posR = D3DXVECTOR3(0.0f, 65, -180);
		m_posV = D3DXVECTOR3(0.0f, 75, -305);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, atan2f((m_posV.x - m_posR.x), (m_posV.z - m_posR.z)), 0.0f);
		m_rotDest = m_rot;
		m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));

		m_fContCamSpeed = 5.0f;				// カメラの速さ上限の初期設定
		m_fCameraSpeed = 5.0f;				// カメラの速さの初期設定
		m_fCameraRotSpeed = 0.025f;			// カメラの回転動く速さ
		break;
	case CAMERATYPE_RANKING:
		m_posR = D3DXVECTOR3(0.0f, 65, -180);
		m_posV = D3DXVECTOR3(0.0f, 75, -305);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, atan2f((m_posV.x - m_posR.x), (m_posV.z - m_posR.z)), 0.0f);
		m_rotDest = m_rot;
		m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
		break;
	}
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	CInputKeyboard *pInputKeyboad = CManager::GetInputKeyboard();
	m_nGameCounter++;
	switch (m_CameraType)
	{//	カメラの種類
	case CAMERATYPE_NONE:
		break;
	case CAMERATYPE_TITLE:

		if (m_nGameCounter >= 200)
		{//	ゲームのカウンターが180以上
			m_nCameraMove[0]++;
			m_posR = D3DXVECTOR3(0.0f, 25.0f, 615 + (float)m_nCameraMove[0]);
			m_posV = D3DXVECTOR3(0.0f, 70.0f, -385 + (float)m_nCameraMove[0]);
		}
		if (m_nGameCounter >= 600)
		{
			m_nCameraMove[1]++;
			m_posR = D3DXVECTOR3(-219.3f + (float)m_nCameraMove[1], 25.0f, -128.0f + (float)m_nCameraMove[1]);
			m_posV = D3DXVECTOR3(-479.5f + (float)m_nCameraMove[1], 250.0f, -431.0f + (float)m_nCameraMove[1]);
		}
		if (m_nGameCounter >= 1000)
		{
			m_nCameraMove[2]++;
			m_posR = D3DXVECTOR3(219.3f - (float)m_nCameraMove[2], 25.0f, -128.0f + (float)m_nCameraMove[2]);
			m_posV = D3DXVECTOR3(479.5f - (float)m_nCameraMove[2], 250.0f, -431.0f + (float)m_nCameraMove[2]);
		}
		if (m_nGameCounter >= 1400)
		{
			m_nGameCounter = 0;
			m_nCameraMove[0] = 0;
			m_nCameraMove[1] = 0;
			m_nCameraMove[2] = 0;
			m_posR = D3DXVECTOR3(0.0f, 25, 315);
			m_posV = D3DXVECTOR3(0.0f, 70, -85);
		}


		break;
	case CAMERATYPE_GAME:

		if (m_nGameCounter > 60)
		{
			m_AddCamera.z++;
		}
		if (m_posR.z >= 10 && m_posV.z >= -115) {
		}
		else {
			m_posR = D3DXVECTOR3(0.0f, 65, -180 + (m_AddCamera.z * 10));
			m_posV = D3DXVECTOR3(0.0f, 75, -305 + (m_AddCamera.z * 10));
		}
		break;
	case CAMERATYPE_RANKING:

		m_rot.y -= 0.025f;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posV.x = sinf(m_rot.y) * m_fLength + m_posR.x;
		m_posV.z = cosf(m_rot.y) * m_fLength + m_posR.z;
		break;
	}


#ifdef _DEBUG
	CManager::m_CDebugProcRight->Print("\nカメラの視点  ( %.1f : %.1f  : %.1f)", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_CDebugProcRight->Print("\nカメラの注視点( %.1f : %.1f  : %.1f)", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_CDebugProcRight->Print("\nカメラの回転( %.2f : %.2f  : %.2f)", m_rot.x, m_rot.y, m_rot.z);
	CManager::m_CDebugProcRight->Print("\nカメラの速さ上限　(%.2f)", m_fContCamSpeed);
	CManager::m_CDebugProcRight->Print("\nカメラの速さ      (%.2f)", m_fCameraSpeed);
	CManager::m_CDebugProcRight->Print("\n");
	CManager::m_CDebugProcRight->Print("\n[ N_8 ] 前方移動　　");
	CManager::m_CDebugProcRight->Print("\n[ N_5 ] 後方移動　　");
	CManager::m_CDebugProcRight->Print("\n[ N_4 ] 左移動　　　");
	CManager::m_CDebugProcRight->Print("\n[ N_6 ] 右移動　　　");
	CManager::m_CDebugProcRight->Print("\n");
	CManager::m_CDebugProcRight->Print("\n[ N_7 ] 注視点左旋回");
	CManager::m_CDebugProcRight->Print("\n[ N_9 ] 注視点右旋回");
	CManager::m_CDebugProcRight->Print("\n[ N_1 ] 視点左旋回　");
	CManager::m_CDebugProcRight->Print("\n[ N_3 ] 視点右旋回　");
	CManager::m_CDebugProcRight->Print("\n");
	CManager::m_CDebugProcRight->Print("\n[ N_/ ] 視点の上昇  ");
	CManager::m_CDebugProcRight->Print("\n[ N_0 ] 視点の下降  ");
	CManager::m_CDebugProcRight->Print("\n[ N_* ] 注視点の上昇");
	CManager::m_CDebugProcRight->Print("\n[ N_. ] 注視点の下降");
	CManager::m_CDebugProcRight->Print("\n");
	CManager::m_CDebugProcRight->Print("\n[ N_ENTER ] 初期位置に戻す");
	CManager::m_CDebugProcRight->Print("\n[ N_+ ] カメラの速さ加算");
	CManager::m_CDebugProcRight->Print("\n[ N_- ] カメラの速さ減算");
	CManager::m_CDebugProcRight->Print("\n[ Pre N_2 & N_+ ] カメラの速さ制御加算");
	CManager::m_CDebugProcRight->Print("\n[ Pre N_2 & N_- ] カメラの速さ制御減算");

#endif
}
//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得

														// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection
		, D3DXToRadian(45.0f)							// 画角
		, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT	// 比率
		, 10.0f											// 描画範囲[手前]
		, 16000);										// 描画範囲[奥行]

														// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView
		, &m_posV
		, &m_posR
		, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}
//=============================================================================
//	カメラの回転の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetCameraRotate(void)
{
	return m_rot;
}

