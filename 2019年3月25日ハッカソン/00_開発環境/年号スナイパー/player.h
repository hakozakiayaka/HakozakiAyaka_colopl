//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 箱﨑彩花
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// ファイル読み込みで必要
#include <stdio.h>			//インクルードファイル

#include "main.h"			// メイン
#include "scene.h"			// scene

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY					(2)				// プレイヤーの優先順位
#define MODEL_SPEED						(0.05f)			//モデルの移動の速さ
#define MAX_CHAR						(256)			// 文字の最大数
#define MAX_KEY							(11)			// キーの総数
#define JUMP							(10.0f)			// ジャンプ
#define PLAYER_MOVE						(0.2f)			// 基準の移動量
#define MAX_PLAYER_PARTS				(12)			//プレイヤーのパーツ数
#define PLAYER_LIFE						(3)				// プレイヤーのライフ
#define PLAYER_FILENAME			"data/TEXT/motion_souri.txt"	//読み込むファイル名

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;		// モデル

//*****************************************************************************
// プレイヤークラス
//*****************************************************************************
class CPlayer : public CScene
{
	//*****************************************************************************
	// 構造体の定義
	//*****************************************************************************
	typedef struct
	{	// キー要素
		D3DXVECTOR3 pos[MAX_PLAYER_PARTS];		//位置情報
		D3DXVECTOR3 rot[MAX_PLAYER_PARTS];		//角度情報
		int nFrame;		// フレーム数
	} KEY;

	typedef struct
	{	// キー情報
		int nLoop;		// ループするかしないか
		int nNumKey;	// キー数
		KEY aKey[MAX_KEY];
	} KEY_INFO;

public:
	typedef enum
	{	// プレイヤーの状態
		PLAYERSTATE_NORMAL = 0,		// 通常状態
		PLAYERSTATE_MOVE,					// 移動状態
		PLAYERSTATE_TURNUP,				// 攻撃状態
		PLAYERSTATE_DEATH,					// ダメージ状態
		PLAYERSTATE_MAX						// ステートの最大数
	} PLAYERSTATE;

	typedef enum
	{	// モーションの種類
		MOTIONSTATE_NEUTRAL = 0,		// ニュートラル
		MOTIONSTATE_MOVE,					// 移動
		MOTIONSTATE_TURNUP,				// 攻撃
		MOTIONSTATE_DEATH,					// ジャンプ
		MOTIONSTATE_MAX						// ステートの最大数
	} MOTIONSTATE;

	typedef enum
	{	// フラグの種類
		PLAYERBOOLTYPE_JUMP = 0,		// ジャンプ
		PLAYERBOOLTYPE_MOVE,			// 移動
		PLAYERBOOLTYPE_TURNUP,			// 発表
		PLAYERBOOLTYPE_DEATH,			// 死んだ
		PLAYERBOOLTYPE_MAX				// タイプの最大数
	} PLAYERBOOLTYPE;

	CPlayer(int nPriority = 3, OBJTYPE type = OBJTYPE_PLAYER);			// コンストラクタ
	~CPlayer();															// デストラクタ
	HRESULT Init(void);													// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
	void PlayerMotionState(bool *bAction);								// モーションステートの更新
	void PlayerMotion(void);											// モーションの設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置の取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向きの取得
	D3DXVECTOR3 GetMove(void) { return m_move; }		// 移動量の取得
	PLAYERSTATE GetState(void) { return m_state; }		// ステートの取得
	char * ReadLine(FILE *pFile, char *pDst);			// 1行分読み込むための関数
	char *GetLineTop(char *pSrc);						// 文字列の先頭を設定
	int PopString(char *pStr, char *pDst);				// 文字列を抜き出す
	static bool GetNengouUp(void) { return m_bNengouUp; }		//	年号を上げた取得
	static bool GetDisp(void) { return m_bDisp; }				//	死亡取得

	// 静的メンバ関数
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);			// プレイヤーの生成

private:
	// 基本使用する変数-------------------------------------------------------------------------------------------------
	LPD3DXMESH			m_pMesh;													// メッシュ情報（頂点情報）へのポインタ
	LPD3DXBUFFER		m_pBuffMat;												// マテリアル情報へのポインタ
	DWORD				m_nNumMat;											// マテリアル情報の数
	D3DXMATRIX			m_mtxWorld;												// ワールドマトリックス
	D3DXVECTOR3			m_pos;														// 現在の位置
	D3DXVECTOR3			m_posOld;													// 前回の位置
	D3DXVECTOR3			m_rot;														// 向き
	D3DXVECTOR3			m_scale;													// 向き
	D3DXVECTOR3			m_move;														// 移動量
	float				m_fAngle;									// 角度を求める
	float				m_fDestAngle;
	int					m_StateCnt;										//状態変化のカウンター
	PLAYERSTATE			m_state;													//プレイヤーの状態
	bool				m_bDraw;									//描画するか
	int					m_nLife;										// プレイヤーのライフ
	float				m_nCntSpeed;									//	プレイヤーの動き
	// メッシュフィールドの判定で使用する変数------------------------------------------------------------------------
	D3DXVECTOR3			m_vecA;														//壁
	D3DXVECTOR3			m_vecC;														//aPosからposまでのベクトル
	float				m_fAnswer;									// ベクトルの答え
	// モデルの読み込みで使用する変数----------------------------------------------------------------------------------
	CModel							*m_apModel[MAX_PLAYER_PARTS];						// モデルへのポインタ
	char							m_aFilenameModel[MAX_PLAYER_PARTS][256];	// 読み込んだモデル名を保存しておく変数
	int								m_nNumModel;									// 読み込むモデルの数
	int								m_nNumParts;									// 読み込むモデルのパーツ数
	int								m_aIndexParent[MAX_PLAYER_PARTS];				// パーツのインデックス番号
	KEY								m_aKeyOffset[MAX_PLAYER_PARTS];					// キーのオフセット
	static D3DXVECTOR3				m_PartsPos[MAX_PLAYER_PARTS];								// パーツの位置
	static D3DXVECTOR3				m_PartsRot[MAX_PLAYER_PARTS];								// パーツの向き
	static int						m_aParent[MAX_PLAYER_PARTS];					//パーツの親
	static int						m_aIndex[MAX_PLAYER_PARTS];						//パーツの番号
	// モーションで使用する変数----------------------------------------------------------------------------------
	static D3DXVECTOR3	m_PartsDefaultPos[MAX_PLAYER_PARTS];						// デフォルトのパーツの位置
	static D3DXVECTOR3	m_PartsDefaultRot[MAX_PLAYER_PARTS];						// デフォルトのパーツの向き
	static D3DXVECTOR3	m_motionPos[MAX_PLAYER_PARTS];								// モーション差分代入用位置
	static D3DXVECTOR3	m_motionRot[MAX_PLAYER_PARTS];								// モーション差分用代入用向き
	int					m_nNumKey;											// キーの総数
	KEY_INFO			*m_pKeyInfo;										// キー情報のポインタ
	KEY_INFO			m_aKeyInfo[MOTIONSTATE_MAX];						// キー情報
	MOTIONSTATE			m_motionState;												// モーションの状態
	MOTIONSTATE			m_motionStateOld;											// 前回のモーション状態
	bool				m_bAction[PLAYERBOOLTYPE_MAX];					// 行動中のフラグ
	bool				m_motionBlend;									// ブレンドするかしないか
	int					m_nKey;											// 現在のキー
	int					m_nAllFrame;									// 全フレーム数
	int					m_nCntMotion;									// モーションカウンター
	int					m_nCntFrame;									// フレームカウンター
	int					m_nCntAllFrame;									// 全フレームのカウンター
	int					m_nCntKey;										// キーのカウンター
	int					m_nFrame;										// 現在のフレーム
	float				m_fRadius;										// 当たり判定の範囲
	int					m_nCountFream;									//	カウンターフレーム
	static bool			m_bNengouUp;									//	ランダム
	static bool			m_bDisp;										//	死亡状態

};
#endif