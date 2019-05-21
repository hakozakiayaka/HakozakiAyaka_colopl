//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "block.h"			// ブロック
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "game.h"			// ゲーム
#include "input.h"			// 入力
#include "camera.h"			// カメラ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME		"data\\MODEL\\Object\\cylinder.x"			//　シリンダー
#define MODEL_NAME_0	"data\\MODEL\\Object\\Model\\000_Desk.x"	// 机
#define MODEL_NAME_1	"data\\MODEL\\Object\\Model\\001_mic.x"		// マイク
#define MODEL_NAME_2	"data\\MODEL\\Object\\Model\\002_Pipe.x"	// イス

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CBlock::m_pTexture[m_MaxModel] = {};		// テクスチャ情報へのポインタ
LPD3DXMESH				CBlock::m_pMesh[m_MaxModel] = {};			// メッシュ情報へのポインタ
LPD3DXBUFFER			CBlock::m_pBuffMat[m_MaxModel] = {};		// マテリアル情報へのポインタ
DWORD					CBlock::m_nNumMat[m_MaxModel] = {};			// マテリアル情報の数

//=============================================================================
// コンストラクタ
//=============================================================================
CBlock::CBlock() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_BLOCK)
{
	m_modelType = MODELTYPE_CYLINDER;								//　モデルの種類
	m_colType = COLTYPE_NONE;										//	当たり判定の種類
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//	過去の位置

	for (int nCount = 0; nCount < 6; nCount++)
	{//	当たった場所の設定
		m_bHit[nCount] = false;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CBlock::~CBlock()
{
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CBlock::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	モデルの名前を格納のする変数
	char cModelName[256];

	for (int nCntIndex = 0; nCntIndex < m_MaxModel; nCntIndex++)
	{//	モデルの最大数分回す
		switch (nCntIndex)
		{//	読み込むモデルの名前
		case MODELTYPE_CYLINDER:		//	シリンダー
			wsprintf(cModelName, MODEL_NAME);
			break;
		case MODELTYPE_DESK:			//	机
			wsprintf(cModelName, MODEL_NAME_0);
			break;
		case MODELTYPE_MIC:				//	マイク
			wsprintf(cModelName, MODEL_NAME_1);
			break;
		case MODELTYPE_CHAIR:			//	イス
			wsprintf(cModelName, MODEL_NAME_2);
			break;
		}

		// Xファイルの読み込み
		D3DXLoadMeshFromX(cModelName,
						D3DXMESH_SYSTEMMEM,
						pDevice, NULL,
						&m_pBuffMat[nCntIndex],
						NULL,
						&m_nNumMat[nCntIndex],
						&m_pMesh[nCntIndex]);

		D3DXMATERIAL *pmat;									// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;								// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntIndex]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntIndex]];						//	マテリアルのメモリの確保
		m_pTexture[nCntIndex] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntIndex]];	//	テクスチャのメモリの確保

		for (DWORD tex = 0; tex < m_nNumMat[nCntIndex]; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;					//　マテリアルのプロパティを記述する D3DMATERIAL9 構造体
			matDef[tex].Ambient = matDef[tex].Diffuse;		//　光の直接当たる部分の色を光を当たらない部分と同期させる
			m_pTexture[nCntIndex][tex] = NULL;				//	テクスチャ情報を空にする
			if (pmat[tex].pTextureFilename != NULL &&lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している & 文字列の長さが0以上だったら
				if (FAILED(D3DXCreateTextureFromFile(pDevice,pmat[tex].pTextureFilename,&m_pTexture[nCntIndex][tex])))
				{
					//MessageBox(NULL, "モデルのテクスチャ/マテリアルの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
		//	マテリアルの開放
		delete[] matDef;
		matDef = NULL;
	}

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CBlock::Unload(void)
{
	for (int nCntIndex = 0; nCntIndex < m_MaxModel; nCntIndex++)
	{//	モデルの最大数分回す
		// テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntIndex]; tex++)
		{// カウント
			if (m_pTexture[nCntIndex][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntIndex][tex]->Release();		// メモリの解放
				m_pTexture[nCntIndex][tex] = NULL;			// NULLへ
			}
		}
		//	テクスチャの開放
		delete[] m_pTexture[nCntIndex];
		m_pTexture[nCntIndex] = NULL;

		// マテリアルの開放
		if (m_pBuffMat[nCntIndex] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntIndex]->Release();				// メモリの解放
			m_pBuffMat[nCntIndex] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntIndex] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntIndex]->Release();					// メモリの解放
			m_pMesh[nCntIndex] = NULL;						// NULLへ
		}
		// マテリアルの初期化
		m_nNumMat[nCntIndex] = 0;
	}
}

//=============================================================================
// 確保処理
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTYPE modelType, COLTYPE coltype)
{
	CBlock *pBlock = NULL;	// ポインタ
	if (pBlock == NULL)
	{// NULLの場合
		pBlock = new CBlock;	// メモリ確保

		if (pBlock != NULL)
		{// NULL以外の場合
			pBlock->Init(pos,rot, scale,modelType, coltype);		// 初期化処理
		}
	}
	// 値を返す
	return pBlock;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTYPE modelType, COLTYPE coltype)
{
	//	モデルの変数の初期化
	CSceneX::SetInitAll(m_pTexture[modelType], m_pMesh[modelType], m_pBuffMat[modelType], m_nNumMat[modelType], pos, rot, scale);
	CSceneX::Init();				//	モデルの初期化
	CSceneX::Setpos(pos);			//	位置の設定
	CSceneX::Setrot(rot);			//	回転の設定
	CSceneX::SetScale(scale);		//	大きさの設定

	m_modelType = modelType;		//	モデルの種類
	m_colType = coltype;			//	モデルの判定の種類

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// 終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBlock::Update(void)
{
	for (int nCount = 0; nCount < 6; nCount++)
	{//	当たった場所の設定
		m_bHit[nCount] = false;
	}
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);	//	モデルの大きさ最大値
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);	//	モデルの大きさ最小値

	D3DXVECTOR3 pos = CSceneX::Getpos();			//	位置の取得
	D3DXVECTOR3 rot = CSceneX::Getrot();			//	回転の取得
	D3DXVECTOR3 scale = CSceneX::GetScale();		//	拡大縮小の取得

	CSceneX::Setpos(pos);							//	位置の設定
	CSceneX::Setrot(rot);							//	回転の設定
	CSceneX::SetScale(scale);						//	大きさの設定
}

//=============================================================================
// 描画処理
//=============================================================================
void CBlock::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	ライトの無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 頂点法線の自動正規化		開始
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// 描画処理
	CSceneX::Draw();

	// 頂点法線の自動正規化		終了
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//	ライトを戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CBlock::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	for (int nCount = 0; nCount < 6; nCount++)
	{//	当たった場所の設定
		m_bHit[nCount] = false;
	}

	bool bHit = false;

	D3DXVECTOR3 BlockPos = CSceneX::Getpos();			//	位置の取得
	D3DXVECTOR3 BlockRot = CSceneX::Getrot();			//	回転の取得
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);		//	大きさの最大値
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);		//	大きさの最小値

	if (BlockRot.y == 0.0f || BlockRot.y == D3DX_PI * 1.0f)
	{//	手前か後ろ向きだった場合
		if (m_colType == COLTYPE_BOX)
		{//	ボックスコリジョンだった場合
			if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) &&
				(pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x) &&
				(BlockPos.z + BlockSizeMin.z) < (pos->z + (sizeMax.z)) &&
				(pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.z))
			{// X/Z範囲確認
				if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
				{// 上からの当たり判定
					m_bHit[0] = true;
					bHit = true;
					pos->y = posOld->y;
					move->y = 0.0f;
				}
				else if ((BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (BlockPos.y + BlockSizeMin.y))
				{// 下からの当たり判定
					m_bHit[1] = true;
					bHit = true;
					pos->y = posOld->y;
					move->y = 0.0f;
				}
			}
			if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
			{// Y範囲確認
				if ((BlockPos.z + BlockSizeMin.z) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.z))
				{// Z範囲確認
					if ((BlockPos.x + BlockSizeMin.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.x))
					{// 左からの当たり判定

						m_bHit[2] = true;
						bHit = true;
						pos->x = posOld->x;
						move->x = 0;
					}
					else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.x) && (BlockPos.x + BlockSizeMax.x) <= (posOld->x + sizeMin.x))
					{// 右からの当たり判定
						m_bHit[3] = true;
						bHit = true;
						pos->x = posOld->x;
						move->x = 0;
					}
				}
			}
			if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
			{// Y範囲確認
				if ((BlockPos.x + BlockSizeMin.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.x))
				{// X範囲確認
					if ((BlockPos.z + BlockSizeMin.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (BlockPos.z + BlockSizeMin.z))
					{// 手前からの当たり判定
						m_bHit[4] = true;
						bHit = true;
						pos->z = posOld->z;
						move->z = 0;
					}
					else if ((pos->z + sizeMin.z) <= (BlockPos.z + BlockSizeMax.z) && (BlockPos.z + BlockSizeMax.z) <= (posOld->z + sizeMin.z))
					{// 後ろからの当たり判定
						m_bHit[5] = true;
						bHit = true;
						pos->z = posOld->z;
						move->z = 0;
					}
				}
			}
		}
	}
	else if (BlockRot.y == D3DX_PI * 0.5f || BlockRot.y == -D3DX_PI * 0.5f)
	{//	右か後ろ向きだった場合
		if (m_colType == COLTYPE_BOX)
		{
			if ((BlockPos.x + BlockSizeMin.z) < (pos->x + (sizeMax.x)) &&
				(pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.z) &&
				(BlockPos.z + BlockSizeMin.x) < (pos->z + (sizeMax.z)) &&
				(pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.x))
			{// X/Z範囲確認
				if ((pos->y + sizeMin.y) <= (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMax.y) <= (posOld->y + sizeMin.y))
				{// 上からの当たり判定
					m_bHit[0] = true;
					bHit = true;
					pos->y = posOld->y;
					move->y = 0.0f;

				}
				else if ((BlockPos.y + BlockSizeMin.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (BlockPos.y + BlockSizeMin.y))
				{// 下からの当たり判定
					m_bHit[1] = true;
					bHit = true;
					pos->y = posOld->y;
					move->y = 0.0f;
				}
			}
			if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
			{// Y範囲確認
				if ((BlockPos.z + BlockSizeMin.x) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.x))
				{// Z範囲確認
					if ((BlockPos.x + BlockSizeMin.z) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (BlockPos.x + BlockSizeMin.z))
					{// 左からの当たり判定

						m_bHit[2] = true;
						bHit = true;
						pos->x = posOld->x;
						move->x = 0;
					}
					else if ((pos->x + sizeMin.x) <= (BlockPos.x + BlockSizeMax.z) && (BlockPos.x + BlockSizeMax.z) <= (posOld->x + sizeMin.x))
					{// 右からの当たり判定
						m_bHit[3] = true;
						bHit = true;
						pos->x = posOld->x;
						move->x = 0;
					}
				}
			}
			if ((pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) && (BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
			{// Y範囲確認
				if ((BlockPos.x + BlockSizeMin.z) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.z))
				{// X範囲確認
					if ((BlockPos.z + BlockSizeMin.x) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (BlockPos.z + BlockSizeMin.x))
					{// 手前からの当たり判定
						m_bHit[4] = true;
						bHit = true;
						pos->z = posOld->z;
						move->z = 0;
					}
					else if ((pos->z + sizeMin.z) <= (BlockPos.z + BlockSizeMax.x) && (BlockPos.z + BlockSizeMax.x) <= (posOld->z + sizeMin.z))
					{// 後ろからの当たり判定
						m_bHit[5] = true;
						bHit = true;
						pos->z = posOld->z;
						move->z = 0;
					}
				}
			}
		}
	}
	return bHit;
}

//=============================================================================
// ステルスブロックの当たり判定処理
//=============================================================================
bool CBlock::StealthCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	D3DXVECTOR3 BlockPos = CSceneX::Getpos();			//	位置の取得
	D3DXVECTOR3 BlockRot = CSceneX::Getrot();			//	回転の取得
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);		//	大きさの最大値
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);		//	大きさの最小値
	if (m_colType == COLTYPE_STEALTH)
	{//	ステルスコリジョン
		if ((BlockPos.x + BlockSizeMin.z) < (pos->x + (sizeMax.x)) &&
			(pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.x) &&
			(pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.z) &&
			(BlockPos.z + BlockSizeMin.x) < (pos->z + (sizeMax.z)) &&
			(pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) &&
			(BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
		{// X/Z/Y範囲確認
			m_bStealthIN = true;
		}
		else
		{
			m_bStealthIN = false;
		}
	}
	return m_bStealthIN;
}

//=============================================================================
// 次のステージいく判定
//=============================================================================
bool CBlock::StageNxstCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bIn = false;		//	中に入ったかどうか

	D3DXVECTOR3 BlockPos = CSceneX::Getpos();			//	位置の取得
	D3DXVECTOR3 BlockRot = CSceneX::Getrot();			//	回転の取得
	D3DXVECTOR3 BlockSizeMax = CSceneX::Getsize(0);		//	大きさの最大値
	D3DXVECTOR3 BlockSizeMin = CSceneX::Getsize(1);		//	大きさの最小値
	if (m_colType == COLTYPE_STAGENXST)
	{//	ステルスコリジョン
		if ((BlockPos.x + BlockSizeMin.z) < (pos->x + (sizeMax.x)) &&
			(pos->z + (sizeMin.z)) < (BlockPos.z + BlockSizeMax.x) &&
			(pos->x + (sizeMin.x)) < (BlockPos.x + BlockSizeMax.z) &&
			(BlockPos.z + BlockSizeMin.x) < (pos->z + (sizeMax.z)) &&
			(pos->y + sizeMin.y) < (BlockPos.y + BlockSizeMax.y) &&
			(BlockPos.y + BlockSizeMin.y) < (pos->y + sizeMax.y))
		{// X/Z/Y範囲確認
			bIn = true;										//	中に入った場合
			CGame::SetGameState(CGame::GAMESTATE_CLEAR);	//	ゲームクリアにする
		}
	}
	return bIn;
}