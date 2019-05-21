//=============================================================================
//
// シーン2D処理 [scene2d.h]
// Author :  YUTARO ABE
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

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
class CScene2D : public CScene
{// シーン3D（親：CScene）
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);		// コンストラクタ
	~CScene2D();								// デストラクタ

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// テクスチャ反映

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{
		m_pos = pos;
		m_size = size;
		m_col = col;
		m_TexUV = TexUV;
	}

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	D3DXVECTOR3 Getpos(void) { return m_pos; }			// 位置取得
	void Setsize(D3DXVECTOR3 size) { m_size = size; }	// 大きさ設定
	D3DXVECTOR3 Getsize(void) { return m_size; }		// 大きさ取得
	void Setcol(D3DXCOLOR col) { m_col = col; }			// 色設定
	D3DXCOLOR Getcol(void) { return m_col; }			// 色取得

	void SetTexture(D3DXVECTOR2 *pTex);			//	アニメーションの設定

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファ情報へのポインタ

	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXCOLOR m_col;							// 色
	D3DXVECTOR2 m_TexUV;						// UV

protected:

};

#endif