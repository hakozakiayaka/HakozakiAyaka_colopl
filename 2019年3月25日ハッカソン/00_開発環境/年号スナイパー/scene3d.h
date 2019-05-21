//=============================================================================
//
// シーン3D処理 [scene3d.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3D : public CScene
{// シーン3D（親：CScene）
public:
	CScene3D(int nPriority, CScene::OBJTYPE objType);		// コンストラクタ
	~CScene3D();								// デストラクタ

	typedef enum
	{// シーン3Dタイプ
		SCENE3DTYPE_NORMAL = 0,				// 通常
		SCENE3DTYPE_BILLBOARD,				// ビルボード
		SCENE3DTYPE_BILLEFFECT,				// ビルボードエフェクト用加算合成あり
		SCENE3DTYPE_SUBSYNTHESIS,			// 減算合成のみの3Dポリゴン
		SCENE3DTYPE_MAX
	}SCENE3DTYPE;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// テクスチャ反映

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{// 初期値設定
		m_pos			= pos;					// 位置
		m_rot			= rot;					// 向き
		m_size			= size;					// 大きさ
		m_col			= col;					// 色
		m_TexUV			= TexUV;				// テクスチャUV
		m_scene3dType	= scene3dType;			// シーン3Dタイプ
	}

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	D3DXVECTOR3 Getpos(void) { return m_pos; }			// 位置取得
	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	D3DXVECTOR3 Getrot(void) { return m_rot; }			// 向き取得
	void Setsize(D3DXVECTOR3 size) { m_size = size; }	// 大きさ設定
	D3DXVECTOR3 Getsize(void) { return m_size; }		// 大きさ取得
	void Setcol(D3DXCOLOR col) { m_col = col; }			// 色設定
	D3DXCOLOR Getcol(void) { return m_col; }			// 色取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファ情報へのポインタ
	D3DXMATRIX	m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXCOLOR m_col;							// 色
	D3DXVECTOR2 m_TexUV;						// UV
	SCENE3DTYPE m_scene3dType;					// シーン3Dタイプ

protected:

};

#endif