//=============================================================================
//
// タイトル処理 [title.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// タイトル
#include "manager.h"	// マネージャー
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEnemy;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle
{// タイトル
public:
	typedef enum
	{//	オブジェクトの種類
		OBJTYPE_NONE = 0,	//	ノーマル
		OBJTYPE_ITEM,		//	アイテム
		OBJTYPE_BLOCK,		//	ブロック
		OBJTYPE_3DPORI,		//	3Dポリゴン
		OBJTYPE_FLOOR,		//	床
		OBJTYPE_WALL,		//	壁
		OBJTYPE_ENEMY,		//	敵
		OBJTYPE_MAX			//	最大数
	}OBJECTTYPE;

	typedef struct
	{
		int				nCnter;					//	オブジェクトの数を数える
		D3DXVECTOR3		pos[OBJ_MAX];			//	オブジェクトポジションの設定
		D3DXVECTOR3		rot[OBJ_MAX];			//	オブジェクトポジションの設定
		int				nType[OBJ_MAX];			//	オブジェクト種類の設定
		int				nColType[OBJ_MAX];		//	オブジェクト判定の設定
		D3DXVECTOR3		size[OBJ_MAX];			//	オブジェクトサイズの設定
		D3DXCOLOR		col[OBJ_MAX];			//	オブジェクトの色
		D3DXVECTOR2		uv[OBJ_MAX];			//	オブジェクトのテクスチャのUV
		D3DXVECTOR4		pVtxFloor[OBJ_MAX];		//	床の頂点の位置
		int				nAIType[OBJ_MAX];		//	オブジェクトAIの種類
		float			fNoneSpeed[OBJ_MAX];	//	オブジェクトの通常の速さ
		float			fFomSpeed[OBJ_MAX];		//	オブジェクトの追従するときの速さ
		int				nLife[OBJ_MAX];			//	オブジェクト寿命の設定
		int				nMoveLength[OBJ_MAX];	//	オブジェクト判動く距離
	}TEXTSTATE;

	CTitle();						// コンストラクタ
	~CTitle();						// デストラクタ

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	void ObjectLoad(void);			// オブジェクトの読み込み

private:
	TEXTSTATE	    m_textState[OBJTYPE_MAX];	//	テキスト読み込む変数[読み込むオブジェクトの種類]

	int m_nFadeCnt;//自動遷移用

protected:
};

#endif