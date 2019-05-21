//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "wall.h"

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
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CWall::CWall() : CScene3D(CScene::PRIORITY_3, CScene::OBJTYPE_WALL)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CWall::~CWall()
{
}

//=============================================================================
//	アイテムの生成
//=============================================================================
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CWall *pWall = {};

	if (pWall == NULL)
	{//	アイテムの生成
		pWall = new CWall;
		if (pWall != NULL)
		{//アイテムの初期化
			pWall->Init(pos, rot, size, col, TexUV, nType);
		}
	}
	return pWall;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y, 0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_NORMAL);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWall::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWall::Update(void)
{
	D3DXVECTOR3 WallPos = CScene3D::Getpos();		//	位置の取得
	D3DXVECTOR3 WallRot = CScene3D::Getrot();		//	回転の取得
	D3DXVECTOR3 WallSize = CScene3D::Getsize();		//	幅の取得

	CScene3D::Setpos(WallPos);						//	位置の設定
	CScene3D::Setrot(WallRot);						//	回転の設定
	CScene3D::Setsize(WallSize);					//	大きさの設定

	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CWall::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングなくす

	CScene3D::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリングに戻す

}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CWall::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture[0]);		//	壁ノーマル
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\WALL\\backBlock.jpg", &m_pTexture[1]);	//	壁網
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\WALL\\wall000.jpg", &m_pTexture[2]);		//	壁網

	return S_OK;
}
//=============================================================================
// アンロード処理
//=============================================================================
void CWall::Unload(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//	壁の当たり判定
//=============================================================================
bool CWall::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bHit = false;								//	当たったか当たっていないか

	D3DXVECTOR3 WallPos = CScene3D::Getpos();		//	位置の取得
	D3DXVECTOR3 WallSize = CScene3D::Getsize();		//	幅の取得
	D3DXVECTOR3 WallRot = CScene3D::Getrot();		//	回転の取得
	if (WallRot.y == 0.0f || WallRot.y == D3DX_PI * 1.0f)
	{//	手前か後ろ向きだった場合
		WallSize.x = WallSize.x;
		WallSize.z = WallSize.z;

		if ((WallPos.x - WallSize.x) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (WallPos.x + WallSize.x) &&
			(WallPos.z - WallSize.z) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (WallPos.z + WallSize.z))
		{// X/Z範囲確認
			if ((pos->y + sizeMin.y) <= (WallPos.y + WallSize.y) && (WallPos.y + WallSize.y) <= (posOld->y + sizeMin.y))
			{// 上からの当たり判定
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((WallPos.y + WallSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (WallPos.y + WallSize.y))
			{// 下からの当たり判定
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (WallPos.y + WallSize.y) && (WallPos.y - WallSize.y) < (pos->y + sizeMax.y))
		{// Y範囲確認
			if ((WallPos.z - WallSize.z) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (WallPos.z + WallSize.z))
			{// Z範囲確認
				if ((WallPos.x - WallSize.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (WallPos.x - WallSize.x))
				{// 左からの当たり判
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (WallPos.x + WallSize.x) && (WallPos.x + WallSize.x) <= (posOld->x + sizeMin.x))
				{// 右からの当たり判定
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
			}
			if ((WallPos.x - WallSize.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (WallPos.x + WallSize.x))
			{// X範囲確認
				if ((WallPos.z - WallSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (WallPos.z - WallSize.z))
				{// 手前からの当たり判定
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;

				}
				else if ((pos->z + sizeMin.z) <= (WallPos.z + WallSize.z) && (WallPos.z + WallSize.z) <= (posOld->z + sizeMin.z))
				{// 後ろからの当たり判定
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;
				}
			}
		}
	}
	else if (WallRot.y == D3DX_PI * 0.5f || WallRot.y == D3DX_PI * -0.5f)
	{//	右か後ろ向き向きだった場合
		//WallSize.x = WallSize.z;
		//WallSize.z = WallSize.x;

		if ((WallPos.x - WallSize.z) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (WallPos.x + WallSize.z) &&
			(WallPos.z - WallSize.x) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (WallPos.z + WallSize.x))
		{// X/Z範囲確認
			if ((pos->y + sizeMin.y) <= (WallPos.y + WallSize.y) && (WallPos.y + WallSize.y) <= (posOld->y + sizeMin.y))
			{// 上からの当たり判定
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((WallPos.y + WallSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (WallPos.y + WallSize.y))
			{// 下からの当たり判定
				bHit = true;
				pos->y = posOld->y;
				move->z = 0;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (WallPos.y + WallSize.y) && (WallPos.y - WallSize.y) < (pos->y + sizeMax.y))
		{// Y範囲確認
			if ((WallPos.z - WallSize.x) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (WallPos.z + WallSize.x))
			{// Z範囲確認
				if ((WallPos.x - WallSize.z) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (WallPos.x - WallSize.z))
				{// 左からの当たり判
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (WallPos.x + WallSize.z) && (WallPos.x + WallSize.z) <= (posOld->x + sizeMin.x))
				{// 右からの当たり判定
					bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
			}
			if ((WallPos.x - WallSize.z) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (WallPos.x + WallSize.z))
			{// X範囲確認
				if ((WallPos.z - WallSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (WallPos.z - WallSize.x))
				{// 手前からの当たり判定
					bHit = true;
					pos->z = posOld->z;
					move->x = 0;
					//move->z = 0;
				}
				else if ((pos->z + sizeMin.z) <= (WallPos.z + WallSize.x) && (WallPos.z + WallSize.x) <= (posOld->z + sizeMin.z))
				{// 後ろからの当たり判定
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;
				}
			}
		}
	}
	//	値を返す
	return bHit;

}