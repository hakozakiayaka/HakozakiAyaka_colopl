//=============================================================================
//
// モデル処理 [model.h]
// Author : 箱﨑彩花
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"			// メイン
#include "scene.h"			// scene

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PRIORITY				(3)
#define MAX_MODEL					(20)		// 読み込む数分だけ！
#define MAX_CHAR					(256)
#define MODEL_FILENAME		"data/TEXT/model_data.txt"

//*****************************************************************************
// モデルクラス
//*****************************************************************************
class CModel
{
public:
	typedef enum
	{
		MODELTYPE_PLAYER = 0,		//プレイヤー
		MODELTYPE_ENEMY,			//敵
		MODELTYPE_MAX				//モデルの種類の最大数
	}MODELTYPE;

	CModel();																					// コンストラクタ
	~CModel();																					// デストラクタ
	HRESULT Init(void);																			// 初期化処理
	void Uninit(void);																			// 終了処理
	void Update(void);																			// 更新処理
	void Draw(void);																			// 描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 tex);						// テクスチャの割り当て
	void SetPostion(D3DXVECTOR3 pos) { m_rot = pos; }				// 位置の設定
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }				// 向きの設定
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }			// マトリックスの設定
	void SetParent(CModel * pModel) { m_pParent = pModel; }			// モデルの設定
	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }				// ワールドマトリックスの取得
	D3DXVECTOR3 GetPosition(void) { return m_pos; }					// 位置の取得
	D3DXVECTOR3 GetRotation(void) { return m_rot; }					// 向きの取得
	HRESULT Load(LPCSTR Memory);									// 使用するパーツの読み込み
	void Unload(void);												// 使用するパーツの破棄

	// 静的メンバ関数
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type);	//生成

private:
	D3DXVECTOR3		m_pos;											// 位置
	D3DXVECTOR3		m_rot;											// 向き
	D3DXMATRIX		m_mtxWorld;										// ワールドマトリックス
	CModel			*m_pParent;										// 親モデルへのポインタ
	int				m_nNumParts;
	int				m_nMaxParts;
	MODELTYPE		m_type;											//モデルの種類

	// 静的メンバ変数
	static char						m_acFileName[MAX_MODEL][MAX_CHAR];		// 読み込む
	LPD3DXMESH						m_apMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER					m_apBuffMat;		//マテリアル情報へのポインタ
	DWORD							m_aNumMat;			//マテリアルの情報の数
	static LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
};
#endif