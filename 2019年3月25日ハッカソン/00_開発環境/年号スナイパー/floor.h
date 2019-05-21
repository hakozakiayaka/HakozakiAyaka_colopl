//=============================================================================
//
// 床処理 [floor.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _FLOOR_H_
#define _FLOOR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"			// メイン
#include "scene3d.h"		// シーン
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FLOOR_TEX (6)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFloor : public CScene3D
{// シーン3D（親：CScene）
public:

	typedef enum
	{//	ブロックの判定の種類
		COLTYPE_NONE = 0,	//	当たり判定なし
		COLTYPE_BOX,		//	ボックスコリジョン
		COLTYPE_MAX
	}COLTYPE;

	CFloor();															// コンストラクタ
	~CFloor();															// デストラクタ
																		// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV,int nType,COLTYPE coltype);
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		// 生成
	static CFloor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType,COLTYPE coltype);
	static HRESULT Load(void);											// テクスチャの読み込み
	static void Unload(void);											// 破棄
																		// 当たり判定
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);
	bool GetHit(void) { return m_bHit; }								//	当たったかの取得
private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_FLOOR_TEX];			// テクスチャ情報へのポインタ
	bool							m_bHit;								// 当たっているかどうか
	COLTYPE						m_colType;					// 種類

protected:

};

#endif