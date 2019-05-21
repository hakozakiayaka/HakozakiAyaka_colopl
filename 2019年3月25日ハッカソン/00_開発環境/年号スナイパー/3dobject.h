//=============================================================================
//
// 3Dobject処理 [3dobject.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _3DOBJECT_H_
#define _3DOBJECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene3d.h"		// シーン
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class C3Dobject : public CScene3D
{// シーン3D（親：CScene）
public:
	typedef enum
	{
		TYPE_001 = 0,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_005,
		TYPE_006,
		TYPE_007,
		TYPE_008,
		TYPE_009,
		TYPE_MAX
	}OBJECTTYPE;

	C3Dobject();														// コンストラクタ
	~C3Dobject();														// デストラクタ
																		// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static C3Dobject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);											// 破棄

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[TYPE_MAX];					// テクスチャ情報へのポインタ
	OBJECTTYPE				m_Type;											// アイテムの種類

protected:

};

#endif