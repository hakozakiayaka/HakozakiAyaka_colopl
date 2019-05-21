//=============================================================================
//
// ゲーム処理 [game.h]
// Author :  YUTARO ABE
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJ_MAX (2400)			//	テキストで読み込めるオブジェクトの最大数
#define MAX_FLOOR (3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFloor;
class CPause;
class CSound;
class CNumber;
class CTime;
class CPlayer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{// ゲーム
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,	//何もしていない
		GAMESTATE0_NORMAL,	//通常状態
		GAMESTATE_END,		//オーバー状態
		GAMESTATE_CLEAR,	//クリア状態
		GAMESTATE_MAX
	}GAMESTATE;

	typedef enum
	{//	ステージの種類
		STAGE_NONE = 0,		//	何もない
		STAGE_001,			//	STAGE1
		STAGE_MAX
	}STAGETYPE;

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
		int				nLife[OBJ_MAX];			//	オブジェクト寿命の設定
		int				nColType[OBJ_MAX];		//	オブジェクト判定の設定
		int				nAIType[OBJ_MAX];		//	オブジェクトAIの種類
		int				nMoveLength[OBJ_MAX];	//	オブジェクト判動く距離
		float			fNoneSpeed[OBJ_MAX];	//	オブジェクトの通常の速さ
		float			fFomSpeed[OBJ_MAX];		//	オブジェクトの追従するときの速さ
		D3DXVECTOR3		size[OBJ_MAX];			//	オブジェクトサイズの設定
		D3DXCOLOR		col[OBJ_MAX];			//	オブジェクトの色
		D3DXVECTOR2		uv[OBJ_MAX];			//	オブジェクトのテクスチャのUV
		D3DXVECTOR4		pVtxFloor[OBJ_MAX];		//	床の頂点の位置
	}TEXTSTATE;

	CGame();									//	コンストラクタ
	~CGame();									//	デストラクタ
	HRESULT Init(STAGETYPE StageType,int nPlayerLife);							//	初期化処理
	void Uninit(void);							//	終了処理
	void Update(void);							//	更新処理
	void Draw(void);							//	描画処理
	void ObjectLoad(STAGETYPE stage);			//	オブジェクトの読み込み
	void SoundSE(int nNum);					//	効果音
	static CFloor *GetFloor(int nNum) { return m_pFloor[nNum]; }
	static bool GetPause(void) { return m_bPause; }				//	ポーズの取得
	static STAGETYPE GetStage(void) { return m_StageType; }		//	ゲームのステージの種類
	static GAMESTATE GetGameState(void) { return m_GameState; }	//	ゲームの状態の取得
	static void SetGameState(GAMESTATE GameState) { m_GameState = GameState; }	//	ゲームステートの設定

	//ランキング関連
	static CTime *GetTime(void) { return m_pTime; }

private:
	static CFloor  *m_pFloor[MAX_FLOOR];					//	床
	TEXTSTATE	    m_textState[OBJTYPE_MAX];				//	テキスト読み込む変数[読み込むオブジェクトの種類]
	static CPause  *m_pPause;								//	Pause
	static bool     m_bPause;								//	Pausewpしているかどうか
	static STAGETYPE m_StageType;							//	ステージの種類
	static GAMESTATE m_GameState;							//	ゲームの状態
	static int		m_nCounter;								//	ゲームの状態のカウンター
	static CPlayer *m_pPlayer;								//プレイヤークラスに値を返す
	int	m_nPlayerDispCut;									//	プレイヤーがなくなってからのカウント
	//ランキング関連
	static CTime * m_pTime;

protected:

};

#endif