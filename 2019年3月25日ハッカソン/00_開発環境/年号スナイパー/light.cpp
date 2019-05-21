//=============================================================================
//
// ライト処理 [light.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "light.h"			// ライト
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ									(public)	*** CLight ***
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// デストラクタ										(public)	*** CLight ***
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// 初期化処理										(public)	*** CLight ***
//=============================================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// 設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.45f, -0.38f, 0.35f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.27f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_Light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CLight ***
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// 更新処理											(public)	*** CLight ***
//=============================================================================
void CLight::Update(void)
{

}