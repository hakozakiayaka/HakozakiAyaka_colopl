//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"			// タイトル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "fade.h"			// フェード
#include "camera.h"			// カメラ
#include "block.h"			// ブロック
#include "3dobject.h"		// 3D背景オブジェクト
#include "UI.h"				// ui
#include "floor.h"			// 床
#include "wall.h"			// 壁
#include "effect.h"			// エフェクト
#include "sound.h"			// サウンド

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
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	for (int nCountb = 0; nCountb < OBJTYPE_MAX; nCountb++)
	{//	テキストのオブジェクトの種類
		m_textState[nCountb].nCnter = NULL;
		for (int nCount = 0; nCount < OBJ_MAX; nCount++)
		{//	テキストの中身の初期化
			m_textState[nCountb].nType[nCount] = NULL;		//	種類
			m_textState[nCountb].nColType[nCount] = NULL;	//	当たり判定の種類
			m_textState[nCountb].pos[nCount] = {};			//	位置
			m_textState[nCountb].rot[nCount] = {};			//	回転
			m_textState[nCountb].size[nCount] = {};			//	大きさ
			m_textState[nCountb].pVtxFloor[nCount] = {};	//	床の頂点の高さ
			m_textState[nCountb].col[nCount] = {};			//	色
			m_textState[nCountb].uv[nCount] = {};			//	UV
			m_textState[nCountb].nColType[nCount] = {};		//	当たり判定の種類
			m_textState[nCountb].fNoneSpeed[nCount] = {};	//	オブジェクトの通常の速さ
			m_textState[nCountb].fFomSpeed[nCount] = {};	//	オブジェクトの追従するときの速さ
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	//	アテキスト読み込み
	ObjectLoad();
	//	エフェクトのテキストの名前の読み込み
	CEffect::LoadNameEffect();	//	エフェクトのテキスト読み込み
	CEffect::LoadDataEffect();	//	エフェクトのデータ読み込み
	CEffect::Load();			//	エフェクトの読み込み
	CUI::Load(CManager::MODE_TITLE);	//  Uiの読み込み
	CBlock::Load();				//	ブロックの読み込み
	C3Dobject::Load();			//	3Dオブジェクトの読み込み
	CFloor::Load();				//	床の読み込み
	CWall::Load();				//	壁の読み込み

//	UIの生成
	CUI::CreateMode(CManager::MODE_TITLE);
	for (int nCount = 0; nCount < m_textState[OBJTYPE_BLOCK].nCnter; nCount++)
	{//	ブロックの設定　位置　回転　大きさ　種類　　当たり判定の種類
		CBlock::Create
		(m_textState[OBJTYPE_BLOCK].pos[nCount], D3DXVECTOR3(m_textState[OBJTYPE_BLOCK].rot[nCount].x, D3DX_PI*m_textState[OBJTYPE_BLOCK].rot[nCount].y, m_textState[OBJTYPE_BLOCK].rot[nCount].z),
			m_textState[OBJTYPE_BLOCK].size[nCount], (CBlock::MODELTYPE)m_textState[OBJTYPE_BLOCK].nType[nCount], (CBlock::COLTYPE)m_textState[OBJTYPE_BLOCK].nColType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_3DPORI].nCnter; nCount++)
	{//	3Dポリゴンの設定	位置　回転　大きさ　色　UV　種類
		C3Dobject::Create
		(m_textState[OBJTYPE_3DPORI].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[OBJTYPE_3DPORI].size[nCount],
			m_textState[OBJTYPE_3DPORI].col[nCount], m_textState[OBJTYPE_3DPORI].uv[nCount], m_textState[OBJTYPE_3DPORI].nType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_FLOOR].nCnter; nCount++)
	{//	床設定	位置　回転　大きさ　頂点の高さ　色　UV　種類	判定
		CFloor::Create
		(m_textState[OBJTYPE_FLOOR].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[OBJTYPE_FLOOR].size[nCount],
			m_textState[OBJTYPE_FLOOR].col[nCount], m_textState[OBJTYPE_FLOOR].uv[nCount], m_textState[OBJTYPE_FLOOR].nType[nCount], (CFloor::COLTYPE)m_textState[OBJTYPE_FLOOR].nColType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_WALL].nCnter; nCount++)
	{//	壁設定	位置　回転　大きさ　色　UV　種類
		CWall::Create
		(m_textState[OBJTYPE_WALL].pos[nCount], D3DXVECTOR3(0.0f, D3DX_PI*m_textState[OBJTYPE_WALL].rot[nCount].y, 0.0f), m_textState[OBJTYPE_WALL].size[nCount],
			m_textState[OBJTYPE_WALL].col[nCount], m_textState[OBJTYPE_WALL].uv[nCount], m_textState[OBJTYPE_WALL].nType[nCount]);
	}

	m_nFadeCnt = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CUI::Unload();				// Uiの読み込み

	CBlock::Unload();			//	ブロックの開放
	C3Dobject::Unload();		//	3Dポリゴンの開放
	CFloor::Unload();			//	床の開放
	CUI::Unload();				//  Uiの開放
	CWall::Unload();			//	壁の破棄
	CEffect::Unload();			//	エフェクトの開放
	CScene::ReleaseMode();		//	モード終了
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();			//	サウンドの取得
	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードしていない状態
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{// キーボード（ENTER）を入力したら
			CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT, 3);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PRESS);	//	決定音
		}
	}

	m_nFadeCnt++;
	if (m_nFadeCnt >= 500)
	{
		CFade::Create(CManager::MODE_RANKING, CFade::FADE_OUT, 3);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// テキスト読み込み
//=============================================================================
void CTitle::ObjectLoad(void)
{
	FILE *pFile;
	char read[128];										//	読み込む文字列
	char *txstName[OBJTYPE_MAX][128] = {};	//	読み込むテキストのファイル

	txstName[OBJTYPE_ITEM][0] = "data/TEXT/TITLE_MAP/ItemSet.txt";			//	ステージ1のアイテム
	txstName[OBJTYPE_BLOCK][0] = "data/TEXT/TITLE_MAP/BlockSet.txt";		//	ステージ1ブロック
	txstName[OBJTYPE_3DPORI][0] = "data/TEXT/TITLE_MAP/3DPorSet.txt";		//	ステージ1Dポリゴンビルボード
	txstName[OBJTYPE_FLOOR][0] = "data/TEXT/TITLE_MAP/FloorSet.txt";		//	ステージ1Dポリゴン床
	txstName[OBJTYPE_WALL][0] = "data/TEXT/TITLE_MAP/WallSet.txt";			//	ステージ1Dポリゴン壁
	txstName[OBJTYPE_ENEMY][0] = "data/TEXT/TITLE_MAP/EnemySet.txt";		//	ステージ1Dポリゴン敵


	//-------------------------------------アイテム--------------------------------------------
																			//	読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_ITEM][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "ITEM_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].y, &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	大きさの設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].y, &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].z);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ITEM].nType[m_textState[OBJTYPE_ITEM].nCnter]);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].r, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].g, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].b, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].uv[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].uv[m_textState[OBJTYPE_ITEM].nCnter].y);
				}
			} while (strcmp(&read[0], "ITEM_END") != 0);
			//	読み込んだアイテムの数加算
			m_textState[OBJTYPE_ITEM].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		//	テキストを占める
		fclose(pFile);
	}
	//-------------------------------------ブロック-------------------------------------------
	//	読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_BLOCK][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "BLOCK_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT") == 0)
				{//	回転の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	大きさの設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "MODELTYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_BLOCK].nType[m_textState[OBJTYPE_BLOCK].nCnter]);
				}
				else if (strcmp(&read[0], "COLTYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_BLOCK].nColType[m_textState[OBJTYPE_BLOCK].nCnter]);
				}
			} while (strcmp(&read[0], "BLOCK_END") != 0);
			//	読み込んだモデルの数加算
			m_textState[OBJTYPE_BLOCK].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------3Dポリゴン-------------------------------------------
	//	読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_3DPORI][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "3DPORIGON_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].y, &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].y, &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].z);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_3DPORI].nType[m_textState[OBJTYPE_3DPORI].nCnter]);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].r, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].g, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].b, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].uv[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].uv[m_textState[OBJTYPE_3DPORI].nCnter].y);
				}
			} while (strcmp(&read[0], "3DPORIGON_END") != 0);
			//	読み込んだモデルの数加算
			m_textState[OBJTYPE_3DPORI].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------床-------------------------------------------
	//読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_FLOOR][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "FLOOR_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].y, &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	大きさの設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].y, &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].z);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].r, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].g, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].b, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].uv[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].uv[m_textState[OBJTYPE_FLOOR].nCnter].y);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_FLOOR].nType[m_textState[OBJTYPE_FLOOR].nCnter]);
				}
				else if (strcmp(&read[0], "COLTYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_FLOOR].nColType[m_textState[OBJTYPE_FLOOR].nCnter]);
				}
			} while (strcmp(&read[0], "FLOOR_END") != 0);
			//	読み込んだモデルの数加算
			m_textState[OBJTYPE_FLOOR].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------壁-------------------------------------------
	//	読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_WALL][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "WALL_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].y, &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT_Y") == 0)
				{//	Y回転の設定4
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_WALL].rot[m_textState[OBJTYPE_WALL].nCnter].y);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	大きさの設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].y, &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].z);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].r, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].g, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].b, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	大きさの設定4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].uv[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].uv[m_textState[OBJTYPE_WALL].nCnter].y);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_WALL].nType[m_textState[OBJTYPE_WALL].nCnter]);
				}


			} while (strcmp(&read[0], "WALL_END") != 0);
			//	読み込んだモデルの数加算
			m_textState[OBJTYPE_WALL].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}

	//-------------------------------------敵-------------------------------------------
	//	読み込むテキスト
	pFile = fopen(txstName[OBJTYPE_ENEMY][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "ENEMY_SET") == 0)
		{//	文字列が読み込まれていた場合
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	位置の設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].x, &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].y, &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT") == 0)
				{//	向きの設定
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].x, &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].y, &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].z);
				}
				else if (strcmp(&read[0], "LIFE") == 0)
				{//	体力の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nLife[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nType[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "MOVELENGTH") == 0)
				{//	動く距離の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nMoveLength[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "NONESPEED") == 0)
				{//	通常の速さ
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].fNoneSpeed[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "FOMSPEED") == 0)
				{//	追従するときの速さ
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].fFomSpeed[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "AITYPE") == 0)
				{//	AIの種類の設定
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nAIType[m_textState[OBJTYPE_ENEMY].nCnter]);
				}

			} while (strcmp(&read[0], "ENEMY_END") != 0);
			//	読み込んだモデルの数加算
			m_textState[OBJTYPE_ENEMY].nCnter++;
			//	次の文字列読み込む
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
}

