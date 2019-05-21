//=============================================================================
//
// 床処理 [Floor.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "floor.h"			// 床

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
LPDIRECT3DTEXTURE9 CFloor::m_pTexture[MAX_FLOOR_TEX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CFloor::CFloor() : CScene3D(CScene::PRIORITY_2, CScene::OBJTEPE_FLOOR)
{
	m_bHit = false;			//	当たり判定に入ったかどうか
	m_colType = COLTYPE_NONE;										//	当たり判定の種類
}

//=============================================================================
// デストラクタ
//=============================================================================
CFloor::~CFloor()
{
}

//=============================================================================
//	アイテムの生成
//=============================================================================
CFloor *CFloor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType, COLTYPE coltype)
{
	CFloor *pFloor = {};

	if (pFloor == NULL)
	{//	アイテムの生成
		pFloor = new CFloor;
		if (pFloor != NULL)
		{//アイテムの初期化
			pFloor->Init(pos, rot, size, col, TexUV, nType, coltype);
		}
	}
	return pFloor;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFloor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType, COLTYPE coltype)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y, 0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_NORMAL);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);

	m_colType = coltype;			//	モデルの判定の種類
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFloor::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFloor::Update(void)
{
	D3DXVECTOR3 FloorPos = CScene3D::Getpos();		//	壁の位置
	D3DXVECTOR3 FloorRot = CScene3D::Getrot();		//	壁の回転
	D3DXVECTOR3 FloorSize = CScene3D::Getsize();		//	壁の幅
	CScene3D::Setpos(FloorPos);
	CScene3D::Setrot(FloorRot);
	CScene3D::Setsize(FloorSize);

	//CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFloor::Draw(void)
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
HRESULT CFloor::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\FLOOR\\floor000.jpg", &m_pTexture[0]);	//	草

	return S_OK;
}
//=============================================================================
// アンロード処理
//=============================================================================
void CFloor::Unload(void)
{
	for (int nCount = 0; nCount < MAX_FLOOR_TEX; nCount++)
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
//	床の当たり判定
//=============================================================================
bool CFloor::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	m_bHit = false;								//	当たったか当たっていないか

	D3DXVECTOR3 FloorPos = CScene3D::Getpos();			//	床の位置
	D3DXVECTOR3 FloorSize = CScene3D::Getsize();		//	壁床の幅

	if (m_colType == COLTYPE_BOX)
	{//	コリジョンBOXだった場合
		if ((FloorPos.x - FloorSize.x) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (FloorPos.x + FloorSize.x) &&
			(FloorPos.z - FloorSize.z) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (FloorPos.z + FloorSize.z))
		{// X/Z範囲確認
			if ((pos->y + sizeMin.y) <= (FloorPos.y + FloorSize.y) && (FloorPos.y + FloorSize.y) <= (posOld->y + sizeMin.y))
			{// 上からの当たり判定
				m_bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((FloorPos.y + FloorSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (FloorPos.y + FloorSize.y))
			{// 下からの当たり判定
				m_bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (FloorPos.y + FloorSize.y) && (FloorPos.y - FloorSize.y) < (pos->y + sizeMax.y))
		{// Y範囲確認
			if ((FloorPos.z - FloorSize.z) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (FloorPos.z + FloorSize.z))
			{// Z範囲確認
				if ((FloorPos.x - FloorSize.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (FloorPos.x - FloorSize.x))
				{// 左からの当たり判
					m_bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (FloorPos.x + FloorSize.x) && (FloorPos.x + FloorSize.x) <= (posOld->x + sizeMin.x))
				{// 右からの当たり判定
					m_bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
			}
			if ((FloorPos.x - FloorSize.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (FloorPos.x + FloorSize.x))
			{// X範囲確認
				if ((FloorPos.z - FloorSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (FloorPos.z - FloorSize.z))
				{// 手前からの当たり判定
					m_bHit = true;
					pos->z = posOld->z;
					move->z = 0;
				}
				else if ((pos->z + sizeMin.z) <= (FloorPos.z + FloorSize.z) && (FloorPos.z + FloorSize.z) <= (posOld->z + sizeMin.z))
				{// 後ろからの当たり判定
					m_bHit = true;
					pos->z = posOld->z;
					move->z = 0;
				}
			}
		}
	}
	return m_bHit;
}