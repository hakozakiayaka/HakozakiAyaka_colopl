//=============================================================================
//
// 入力処理 [input.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX			(256)			// キーの最大数
#define NUM_BUTTON_MAX		(32)			// ボタンの最大数

#define MAX_CONTROLLERS (4)
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
#define XINPUT_STICK_MAX (32767)			// スティック最大値
#define XINPUT_STICK_MIN (-32768)			// スティック最小値

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInput
{// 入力
public:
	CInput();								// コンストラクタ
	virtual ~CInput();						// デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	virtual void Uninit(void);				// 終了処理
	virtual void Update(void) = 0;			// 更新処理

protected:
	static LPDIRECTINPUT8 m_pInput;			// 入力Obj
	LPDIRECTINPUTDEVICE8 m_pDevice;			// 入力デバイス

};

class CInputKeyboard : public CInput
{// キーボード（親：CInput）
public:
	CInputKeyboard();						// コンストラクタ
	virtual ~CInputKeyboard();				// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);				// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理

	bool GetALL(int nType);					// オール
	bool GetPress(int nKey);				// プレス
	bool GetTrigger(int nKey);				// トリガー
	bool GetRelease(int nKey);				// リリース

private:
	BYTE m_aKeyStatePress[NUM_KEY_MAX];		// プレス
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// トリガー
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// リリース

};

class CInputDIPad : public CInput
{// DIパッド（親：CInput）
public:
	typedef enum
	{// ボタン以外のタイプ
		DIPADOTHER_AXES_UP = NUM_BUTTON_MAX,	// 十字キー上
		DIPADOTHER_AXES_LEFT,					// 十字キー左
		DIPADOTHER_AXES_RIGHT,					// 十字キー右
		DIPADOTHER_AXES_DOWN,					// 十字キー下
		DIPADOTHER_STICK_L_UP,					// Lスティック上
		DIPADOTHER_STICK_L_LEFT,				// Lスティック左
		DIPADOTHER_STICK_L_RIGHT,				// Lスティック右
		DIPADOTHER_STICK_L_DOWN,				// Lスティック下
		DIPADOTHER_STICK_R_UP,					// Rスティック上
		DIPADOTHER_STICK_R_LEFT,				// Rスティック左
		DIPADOTHER_STICK_R_RIGHT,				// Rスティック右
		DIPADOTHER_STICK_R_DOWN,				// Rスティック下
		DIPADOTHER_MAX
	}DIPADOTHER;

	CInputDIPad();								// コンストラクタ
	virtual ~CInputDIPad();						// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理

	bool GetPress(int nButton);					// プレス
	bool GetTrigger(int nButton);				// トリガー
	bool GetRelease(int nButton);				// リリース
	bool GetStick(int nLR);						// スティック
	float GetStickRot(int nLR);					// スティック向き

	float GetDRightXAxis(void);							// スティックの軸
	float GetDRightYAxis(void);							// スティックの軸
	float GetDRightAxis(void);							// スティックの軸
private:
	BYTE m_aJoyStatePress[DIPADOTHER_MAX];		// プレス
	BYTE m_aJoyStateTrigger[DIPADOTHER_MAX];	// トリガー
	BYTE m_aJoyStateRelease[DIPADOTHER_MAX];	// リリース

	float m_LStickRot;							// Lスティック向き
	float m_RStickRot;							// Rスティック向き

	float m_RStickRotX;							// Rスティック向きX
	float m_RStickRotY;							// Rスティック向きY
};

class CInputXPad
{// Xパッド
public:
	typedef enum
	{// ボタン以外のタイプ
		XPADOTHER_BUTTON = 0,					// ボタン
		XPADOTHER_TRIGGER_LEFT,					// Lトリガー
		XPADOTHER_TRIGGER_RIGHT,				// Rトリガー
		XPADOTHER_STICK_L_UP,					// Lスティック上
		XPADOTHER_STICK_L_LEFT,					// Lスティック左
		XPADOTHER_STICK_L_RIGHT,				// Lスティック右
		XPADOTHER_STICK_L_DOWN,					// Lスティック下
		XPADOTHER_STICK_R_UP,					// Rスティック上
		XPADOTHER_STICK_R_LEFT,					// Rスティック左
		XPADOTHER_STICK_R_RIGHT,				// Rスティック右
		XPADOTHER_STICK_R_DOWN,					// Rスティック下
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUTコントローラー
		XINPUT_STATE state;		// 情報
		bool bConnected;		// 入力有無
	}CONTROLER_STATE;

	CInputXPad();								// コンストラクタ
	virtual ~CInputXPad();						// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理

	static int GetInputNum(void);						// 入力数取得

	bool GetALL(int nType, int nIdxPad);				// オール
	bool GetPress(int nButton, int nIdxPad);			// プレス
	bool GetPress(XPADOTHER nButton, int nIdxPad);		// プレス
	bool GetTrigger(int nButton, int nIdxPad);			// トリガー
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);	// トリガー
	bool GetRelease(int nButton, int nIdxPad);			// リリース
	bool GetRelease(XPADOTHER nButton, int nIdxPad);	// リリース

	bool GetStick(int nLR, int nIdxPad);				// スティック
	float GetStickRot(int nLR, int nIdxPad);			// スティック向き


	float GetRightXAxis(void);							// スティックの軸
	float GetRightYAxis(void);							// スティックの軸
	float GetRightAxis(void);							// スティックの軸

private:
	HRESULT UpdateControllerState(void);		// コントローラー入力数更新

	CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];		// XINPUTコントローラー
	bool			m_bDeadZoneOn;						// スティック

	static int		m_nInputNum;						// 入力数

	WORD m_aJoyStatePress[MAX_CONTROLLERS][XPADOTHER_MAX];			// プレス
	WORD m_aJoyStateTrigger[MAX_CONTROLLERS][XPADOTHER_MAX];		// トリガー
	WORD m_aJoyStateRelease[MAX_CONTROLLERS][XPADOTHER_MAX];		// リリース

	float m_LStickRot[MAX_CONTROLLERS];					// Lスティック向き
	float m_RStickRot[MAX_CONTROLLERS];					// Rスティック向き

	float m_RStickRotX;					// Rスティック向きX
	float m_RStickRotY;					// Rスティック向きY

};

#endif