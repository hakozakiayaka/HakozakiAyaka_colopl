//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 箱﨑彩花
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダリング
#include "input.h"				// 入力
#include "DebugProc.h"			// デバッグ表示
#include "game.h"				// ゲーム
#include "player.h"				// プレイヤー
#include "camera.h"				// カメラ
#include "scene3D.h"			// scene3D
#include "fade.h"				// フェード
#include "model.h"				//	モデル
#include "sound.h"				//	音

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
D3DXVECTOR3 CPlayer::m_PartsPos[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3 CPlayer::m_PartsRot[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3 CPlayer::m_PartsDefaultPos[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ位置
D3DXVECTOR3 CPlayer::m_PartsDefaultRot[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ角度
D3DXVECTOR3 CPlayer::m_motionPos[MAX_PLAYER_PARTS] = {};		//モーション差分代入用位置
D3DXVECTOR3 CPlayer::m_motionRot[MAX_PLAYER_PARTS] = {};		//モーション差分代入用角度
int CPlayer::m_aParent[MAX_PLAYER_PARTS] = {};					//パーツの親
int CPlayer::m_aIndex[MAX_PLAYER_PARTS] = {};					//パーツ番号
bool CPlayer::m_bNengouUp = false;								//	年号を上げたかどうか
bool CPlayer::m_bDisp = false;									//	死亡したかどうか

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_bDraw = true;			//描画するか
	m_pMesh = NULL;							// メッシュのポインタ
	m_pBuffMat = NULL;						// バッファのポインタ
	m_nNumMat = 0;						// マテリアル情報
	m_nCountFream = 0;					//
	m_bNengouUp = false;
	m_bDisp = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// プレイヤーの生成
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer * pPlayer = NULL;

	if (pPlayer == NULL)
	{	// プレイヤーの動的確保
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{
			pPlayer->m_nLife = PLAYER_LIFE;		// ライフを設定
			pPlayer->m_pos = pos;
			pPlayer->m_rot = rot;
			pPlayer->m_bAction[PLAYERBOOLTYPE_JUMP] = false;
			pPlayer->m_bAction[PLAYERBOOLTYPE_MOVE] = false;
			pPlayer->m_bAction[PLAYERBOOLTYPE_TURNUP] = false;
			pPlayer->Init();
		}
	}


	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// 値の初期化
	m_fAnswer = 0.0f;		// ベクトルの答え

	FILE *pFile = fopen(PLAYER_FILENAME, "r");	// 読み込むテキストファイルのアドレス

	int nCntMotion = 0;							// モーションのカウンタ
	int nCntKey = 0;							// キーのカウンタ
	int nCntParts = 0;							// パーツのカウンタ
	int nCntModel = 0;							// 読み込むモデルのカウンタ
	int nCntTexture = 0;						// 読み込むテクスチャのカウンタ
	int nIndex = 0;								// 使用するモデルのタイプ
	int nParent = 0;							// 親の数字
	int nMaxParts = 0;							// パーツ数の最大数
	D3DXVECTOR3 pos;							// 位置を入れる変数
	D3DXVECTOR3 rot;							// 向きを入れる変数
	m_nCntSpeed = 1.00f;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	char * pStuCur;					// 文字列の先頭へのポインタ
	char aLine[MAX_CHAR];			// 文字列の読み込み用（1行分）
	char aStr[MAX_CHAR];			// 文字列の抜き出し用
	int nCntWord = 0;				// 文字のカウント
	char *pTop = NULL;				// 一文字目を保存

	if (pFile != NULL)
	{
		pStuCur = ReadLine(pFile, &aLine[0]);		// 1行分読み込み
		strcpy(aStr, pStuCur);									// 文字列を抜き出す

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{	// SCRIPTの文字を見つけた
			pStuCur = ReadLine(pFile, &aLine[0]);		// 1行分読み込み
			strcpy(aStr, pStuCur);									// 文字列を抜き出す

			//*****************************************************************************
			// モデルの読み込み
			//*****************************************************************************
			if (memcmp(aStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{	// NUM_MODEL = の文字を見つけた
				pStuCur += strlen("NUM_MODEL = ");			// 文字の先に、”モデルの数”がある
				strcpy(aStr, pStuCur);					// 文字列を抜き出し
				m_nNumModel = atoi(aStr);		// 文字列（モデルの数）を値に変換

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{	// 読み込んだモデルの数分だけfor文まわす
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1行分読み込み
					strcpy(aStr, pStuCur);									// 文字列を抜き出す

					// モデルの数分for文まわしている中で、"MODEL_FILENAME = "を見つける
					if (memcmp(aStr, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{	// MODEL_FILENAME = の文字を見つけた
						pStuCur += strlen("MODEL_FILENAME = ");			// 文字の先に、"読み込むモデルのデータ"がある
						strcpy(aStr, pStuCur);													// 文字列を抜き出す
						strcpy(m_aFilenameModel[nCntModel], pStuCur);		// 文字列を抜き出し

						//*****************************************************************************
						// 余計な文字を取り除く処理
						//*****************************************************************************
						int nWord = PopString(pStuCur, aStr);						// 文字列を抜き出す処理
						strcpy(aStr, pStuCur);													// 文字列を抜き出す
						aStr[nWord - 1] = '\0';
						strcpy(&m_aFilenameModel[nCntModel][0], aStr);			// 文字列を抜き出し
					}	// MODEL_FILENAME　終了
				}	// パーツ数分のfor文　終了
			}	// NUM_MODEL　終了

				// 続けて読み込んでいく↓
			pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
			strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

			//*****************************************************************************
			// キャラクター情報
			//*****************************************************************************
			if (memcmp(aStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{	// CHARACTERSETの文字を見つけた
				while (1)
				{
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
					strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

					if (memcmp(aStr, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	// NUM_PARTS = の文字を見つけた
						pStuCur += strlen("NUM_PARTS = ");			// 文字の先に、”モデルのパーツ数”がある
						strcpy(aStr, pStuCur);					// 文字列を抜き出し
						m_nNumParts = atoi(aStr);			// 文字列（モデルのパーツ数）を値に変換
					}	// NUM_PARTS　終了
					if (memcmp(aStr, "PARTSSET", strlen("PARTSSET")) == 0)
					{	// PARTSSETの文字を見つけた
						while (1)
						{	// PARTSSETの中身を繰り返す
							pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
							strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

							if (memcmp(aStr, "INDEX = ", strlen("INDEX = ")) == 0)
							{	// INDEX = の文字を見つけた
								pStuCur += strlen("INDEX = ");	// 文字の先に、”モデルの番号”がある
								strcpy(aStr, pStuCur);					// 文字列を抜き出し
								nIndex = atoi(aStr);		// 文字列（モデルの番号）を値に変換
							}	// INDEX　終了
							if (memcmp(aStr, "PARENT = ", strlen("PARENT = ")) == 0)
							{	// PARENT = の文字を見つけた
								pStuCur += strlen("PARENT = ");	// 文字の先に、”モデルの番号”がある
								strcpy(aStr, pStuCur);						// 文字列を抜き出し
								m_aIndexParent[nIndex] = atoi(aStr);		// 文字列（モデルの番号）を値に変換
							}	// PARENT　終了
							else if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
							{	// POS = の文字を見つけた
								pStuCur += strlen("POS = ");		// 文字の先に、”モデルの位置”がある
								strcpy(aStr, pStuCur);					// 文字列を抜き出し

								// posXの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								pos.x = (float)atof(pStuCur);		// Xの値を取得
								m_PartsPos[nIndex].x = pos.x;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める

								// posYの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								pos.y = (float)atof(pStuCur);		// Yの値を取得
								m_PartsPos[nIndex].y = pos.y;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める

								// posZの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								pos.z = (float)atof(pStuCur);		// Zの値を取得
								m_PartsPos[nIndex].z = pos.z;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める
							}	// POS　終了
							else if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
							{	// ROT = の文字を見つけた
								pStuCur += strlen("ROT = ");				// 文字の先に、”モデルの向き”がある
								strcpy(aStr, pStuCur);						// 文字列を抜き出し

								// rotXの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								rot.x = (float)atof(pStuCur);				// Xの値を取得
								m_PartsRot[nIndex].x = rot.x;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める

								// rotYの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								rot.y = (float)atof(pStuCur);				// Yの値を取得
								m_PartsRot[nIndex].y = rot.y;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める

								// rotZの取得----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
								rot.z = (float)atof(pStuCur);				// Zの値を取得
								m_PartsRot[nIndex].z = rot.z;
								pStuCur += nCntWord;								// 取得した文字数分、文字を進める
							}	// ROT　終了
							else if (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{	// END_PARTSSETの文字を見つけた
								if (m_apModel[nIndex] == NULL)
								{	// ポインタの中身が何も入ってなかった時
									m_apModel[nIndex] = CModel::Create(m_PartsPos[nIndex], m_PartsRot[nIndex], m_nNumParts, CModel::MODELTYPE_PLAYER);	// モデルの生成
									m_apModel[nIndex]->Load(m_aFilenameModel[nIndex]);		// モデルの読み込み
								}
								if (m_apModel[nIndex] != NULL)
								{
									// モデルの親を指定する
									if (m_aIndexParent[nIndex] == -1)
									{	// 0（からだ）の時はNULL
										m_apModel[nIndex]->SetParent(NULL);
									}
									else
									{	// それ以外
										m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);
									}
								}
								break;
							}	// END_PARTSSET　終了
						}	// PARTSSET while文　終了
					}	// PARTSSET　終了
					else if (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{	// END_CHARACTERSETの文字を見つけた
						break;
					}	// END_CHARACTERSET　終了
				}	// CHARACTERSET while文　終了
			}	// CHARACTERSET　終了

			if (nCntMotion < MOTIONSTATE_MAX)
			{// モーションの総数以上は設定しない
				while (1)
				{
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
					strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

					if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
					{
						nCntKey = 0;
					}
					if (memcmp(aStr, "LOOP = ", strlen("LOOP = ")) == 0)
					{// LOOPがあったら
						pStuCur += strlen("LOOP = ");		// 文字の先に、”ループするかしないか”がある
						strcpy(aStr, pStuCur);						// 文字列を抜き出し
						m_aKeyInfo[nCntMotion].nLoop = atoi(aStr);		// 文字列（ループするかしないか数）を値に変換
					}
					if (memcmp(aStr, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{// NUM_KEYがあったら
						pStuCur += strlen("NUM_KEY = ");		// 文字の先に、”キー数”がある
						strcpy(aStr, pStuCur);								// 文字列を抜き出し
						m_aKeyInfo[nCntMotion].nNumKey = atoi(aStr);		// 文字列（キー数）を値に変換
					}
					if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
					{
						nCntParts = 0;		// パーツのカウント変数を初期化

						while (1)
						{
							pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
							strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

							if (memcmp(aStr, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								pStuCur += strlen("FRAME = ");			// 文字の先に、”フレーム数”がある
								strcpy(aStr, pStuCur);								// 文字列を抜き出し
								m_aKeyInfo[nCntMotion].aKey[nCntKey].nFrame = atoi(aStr);		// 文字列（フレーム数）を値に変換
							}
							if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
							{
								while (1)
								{
									pStuCur = ReadLine(pFile, &aLine[0]);		// 1行読み込み
									strcpy(aStr, pStuCur);									// 対象の文字列に抜き出し

									if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
									{// POSがあったら
										pStuCur += strlen("POS = ");		// 文字の先に、”モデルの位置”がある
										strcpy(aStr, pStuCur);					// 文字列を抜き出し

										// posXの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].x = (float)atof(pStuCur);						// Xの値を取
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める

										// posYの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].y = (float)atof(pStuCur);		// Yの値を取得
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める

										// posZの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].z = (float)atof(pStuCur);		// Zの値を取得
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める
									}
									if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
									{// ROTがあったら
										pStuCur += strlen("ROT = ");		// 文字の先に、”モデルの向き”がある
										strcpy(aStr, pStuCur);					// 文字列を抜き出し

										// rotXの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].x = (float)atof(pStuCur);		// Xの値を取得
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める

										// rotYの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].y = (float)atof(pStuCur);		// Yの値を取得
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める

										// rotZの取得----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// 何文字目に空白があったかどうかを調べる
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].z = (float)atof(pStuCur);		// Zの値を取得
										pStuCur += nCntWord;								// 取得した文字数分、文字を進める
									}//ROT
									if (memcmp(aStr, "END_KEY", strlen("END_KEY")) == 0)
									{
										nCntParts++;
										break;
									}
								}
							}
							if (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								nCntKey++;
								break;
							}
						}	// KEY_SET
					}
					if (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						nCntMotion++;
					}
					if (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) == 0)
					{
						break;
					}
				}
			}
		}

		fclose(pFile);
	}
	else
	{
		printf(" ファイル読み込み失敗！\n");
	}

	//*****************************************************************************
	// モーション関係
	//*****************************************************************************
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCntMotion = 0;
	m_motionState = MOTIONSTATE_NEUTRAL;

	//プレイヤー情報
	m_fRadius = 5.0f; // プレイヤーの当たり判定の半径
	m_StateCnt = 20;

	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_motionState].nNumKey; nCntKey++)
	{//総フレーム数の計算
		m_nAllFrame += m_aKeyInfo[m_motionState].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// パーツ数分ループ
	 // モデルの位置をニュートラルモーションに初期化
		m_PartsDefaultPos[nCntParts] = m_PartsPos[nCntParts] + m_aKeyInfo[m_motionState].aKey[0].pos[nCntParts];
		m_PartsDefaultRot[nCntParts] = m_aKeyInfo[m_motionState].aKey[0].rot[nCntParts];
	}

	//2Dシーンのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		//モデルの終了処理
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}
	}

	// フラグを立てる
	CScene::SetDeath();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();		// キーボードの取得
	CCamera * pCamera = CManager::GetCamera();											// マネージャーの情報をもってくる

	// 前回の位置に、現在の位置を保存
	m_posOld = m_pos;

	//=========================================
	// モーションの更新
	//=========================================
	PlayerMotionState(m_bAction);
	PlayerMotion();

	//*****************************************************************************
	// プレイヤー横移動
	//*****************************************************************************
	if (m_pos.x == 0.0f)
	{
		m_nCountFream++;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//=========================================
		// 攻撃処理
		//=========================================
		if (m_motionState != MOTIONSTATE_DEATH)
		{			// 死亡状態じゃなかったら
			if (m_nCountFream > rand() % 360 + 240)
			{
				if (m_bAction[PLAYERBOOLTYPE_TURNUP] == false)
				{
					m_bNengouUp = true;					//	年号を上げた
					m_bAction[PLAYERBOOLTYPE_TURNUP] = true;
				}
			}
		}
		if (m_bAction[PLAYERBOOLTYPE_TURNUP] == true)
		{
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{	// エンターキー押した
				m_bAction[PLAYERBOOLTYPE_TURNUP] = false;
				m_motionState = MOTIONSTATE_DEATH;		// 死ぬ状態
				m_bNengouUp = false;					//	年号を上げた
				m_bDisp = true;
				CSound *pSound = CManager::GetSound();			//	サウンドの取得
				pSound->PlaySound(CSound::SOUND_LABEL_SE_BULLET);	//	決定音
			}
		}

	}
	else
	{
		m_move.x = m_nCntSpeed;
	}

	// 移動量を位置の値に加算
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	if (m_pos.x != 0.0f)
	{
		if (m_move.x < 0.3f && m_move.x > -0.3f && m_move.z < 0.3f && m_move.z > -0.3f)
		{
			if (m_motionState != MOTIONSTATE_NEUTRAL)
			{
				m_motionState = MOTIONSTATE_NEUTRAL;			// 移動状態
			}
		}
		else
		{
			if (m_motionState != MOTIONSTATE_MOVE)
			{
				m_motionState = MOTIONSTATE_MOVE;			// 移動状態
			}
		}

	}


	// 目的の角度を求める
	m_fAngle = m_fDestAngle - m_rot.y;


	//以前のモーションを代入
	m_motionStateOld = m_motionState;
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;											// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダリングクラスの呼び出し

	if (pRenderer != NULL)
	{	//Rendererクラスの取得
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;						//計算用マトリックス
	D3DXPLANE planeField;											// プレーンフィールド
	D3DXVECTOR4 vecLight;										// ライトのベクトル
	D3DXVECTOR3 pos, normal;									// 位置、法線
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動（位置）を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	// モデルの描画処理
	if (m_bDraw == true)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
		{
			m_apModel[nCntModel]->Draw();
		}
	}
	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// モーション設定
//=============================================================================
void CPlayer::PlayerMotion(void)
{
	//*****************************************************************************
	// モーションブレンド
	//*****************************************************************************
	if (m_motionState != m_motionStateOld)
	{	// モーションの状態が変わった
		m_nCntFrame = 0;		// 現在のキーフレーム数
		m_nCntKey = 0;			// キーフレーム
		m_nCntAllFrame = 0;		// モーションフレーム数
		m_nFrame = 5;				// モーションブレンドのフレーム数を設定
		m_nAllFrame = 0;

		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_motionState].nNumKey; nCntKey++)
		{	// フレーム数の計算
			m_nAllFrame += m_aKeyInfo[m_motionState].aKey[nCntKey].nFrame;
		}
		m_motionBlend = true;		// モーションブレンド：オン
	}

	//*****************************************************************************
	// キーフレームの入れ替わり
	//*****************************************************************************
	if (m_nCntFrame == m_nFrame)
	{
		m_nCntFrame = 0;		// 現在のキーフレームのフレーム数を初期化
		m_nFrame = m_aKeyInfo[m_motionState].aKey[m_nCntKey].nFrame;		// モーションのフレーム数を設定
		m_nCntKey++;		// キーフレームを進める

		if (m_nCntKey == m_aKeyInfo[m_motionState].nNumKey)
		{	// キーフレーム数が最後まできた
			if (m_aKeyInfo[m_motionState].nLoop == 0)
			{	// ループしない時
				switch (m_motionState)
				{	// モーションが、
				case MOTIONSTATE_TURNUP:		// ジャンプの時
					m_nCntKey--;		// キーフレームを戻す
					break;

				case MOTIONSTATE_DEATH:		// 攻撃の時
					m_bAction[PLAYERBOOLTYPE_DEATH] = false;
					m_nCntKey--;		// キーフレームを戻す
					break;
				}
			}
			else
			{	// ループする
				m_nCntKey = m_nCntKey % m_aKeyInfo[m_motionState].nNumKey;		// キーフレームを進める
				m_nCntAllFrame = 0;		 // モーションのフレーム数(ALL)を初期化
			}
		}
	}

	//*****************************************************************************
	// モーションの差分を代入
	//*****************************************************************************
	if (m_nCntFrame == 0)
	{	// フレーム数が最初の時
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{	// パーツ数分ループ
			// モーションの位置差分を代入
			m_motionPos[nCntParts] = (m_aKeyInfo[m_motionState].aKey[m_nCntKey].pos[nCntParts] + m_motionPos[nCntParts]) - m_PartsDefaultPos[nCntParts];

			// モーションの回転差分を代入
			m_motionRot[nCntParts] = m_aKeyInfo[m_motionState].aKey[m_nCntKey].rot[nCntParts] - m_PartsDefaultRot[nCntParts];

			//*****************************************************************************
			// 回転の差分を修正する
			//*****************************************************************************
			//-----------------------------------------------------------------------
			// X軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].x > D3DX_PI)
			{	// +3.14より、超えた
				m_motionRot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].x < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].x += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			// Y軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].y > D3DX_PI)
			{// +3.14より、超えた
				m_motionRot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].y < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].y += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			//  Z軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].z > D3DX_PI)
			{// +3.14より、超えた
				m_motionRot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].z < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].z += D3DX_PI * 2.0f;
			}
		}
	}

	//*****************************************************************************
	// モーションの動作
	//*****************************************************************************
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{	// パーツ数分ループ
		if (m_apModel[nCntParts] != NULL)
		{
			// 位置差分をフレーム数で割った値を加算していく
			m_PartsDefaultPos[nCntParts].x += m_motionPos[nCntParts].x / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
			m_PartsDefaultPos[nCntParts].y += m_motionPos[nCntParts].y / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
			m_PartsDefaultPos[nCntParts].z += m_motionPos[nCntParts].z / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく

			// 位置の代入
			m_apModel[nCntParts]->SetPostion(m_PartsDefaultPos[nCntParts]);

			// 回転差分をフレーム数で割った値を加算していく
			m_PartsDefaultRot[nCntParts].x += m_motionRot[nCntParts].x / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく
			m_PartsDefaultRot[nCntParts].y += m_motionRot[nCntParts].y / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく
			m_PartsDefaultRot[nCntParts].z += m_motionRot[nCntParts].z / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく

			//回転値の代入
			m_apModel[nCntParts]->SetRotation(m_PartsDefaultRot[nCntParts]);
		}

			//*****************************************************************************
			// モデルの回転を修正する
			//*****************************************************************************
			//-----------------------------------------------------------------------
			// X軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].x > D3DX_PI)
			{	// +3.14より、超えた
				m_motionRot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].x < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].x += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			// Y軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].y > D3DX_PI)
			{// +3.14より、超えた
				m_motionRot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].y < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].y += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			//  Z軸の修正
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].z > D3DX_PI)
			{// +3.14より、超えた
				m_motionRot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].z < -D3DX_PI)
			{// -3.14より、下回った
				m_motionRot[nCntParts].z += D3DX_PI * 2.0f;
			}
		}

		m_nCntFrame++;			// 現在のキーフレームのフレーム数を加算
		m_nCntAllFrame++;		// モーションのフレーム数を加算
}

//=============================================================================
// モーションステートの更新
//=============================================================================
void CPlayer::PlayerMotionState(bool *bAction)
{
	//switch (*bAction)
	//{
	//case PLAYERBOOLTYPE_ATTACK:
	//	break;
	//}
	//if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	//{	// 死亡フラグ
	//	m_motionState = MOTIONSTATE_DEATH;		// 死亡状態
	//}
	//else if (bAction[PLAYERBOOLTYPE_DAMAGE] == true)
	//{	// ダメージフラグ
	//	m_motionState = MOTIONSTATE_DAMAGE;		// ダメージ状態
	//}
	if (bAction[PLAYERBOOLTYPE_TURNUP] == true)
	{	// 着地フラグ
		m_motionState = MOTIONSTATE_TURNUP;	// 着地状態
	}
	else if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	{	// プレイヤーが攻撃
		m_motionState = MOTIONSTATE_DEATH;
	}
	else if (bAction[PLAYERBOOLTYPE_MOVE] == true)
	{
		if (m_move.x > PLAYER_MOVE ||
			m_move.x < -PLAYER_MOVE ||
			m_move.z >  PLAYER_MOVE ||
			m_move.z < -PLAYER_MOVE)
		{	// プレイヤーが動いているとき
			m_motionState = MOTIONSTATE_MOVE;			// 移動状態
		}
	}
	else
	{	// プレイヤーが動いてないとき
		//m_motionState = MOTIONSTATE_NEUTRAL;	 // ニュートラル状態
	}
}

//=============================================================================
// 1行読み込むための処理
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{	// pDstに文字が入るまで続ける
	while (1)
	{
		fgets(pDst, MAX_CHAR, pFile);		// 文字列の読み込み

		if (memcmp(pDst, "#", strlen("#")) == 0)
		{	// #：コメント行だから次の行に進む
			pDst += strlen("#");		// 次の文字に進む
		}
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{	// \n：それ以降は文字がない
			pDst += strlen("\n");		// 次の文字に進む
		}
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{	// \t（タブ）：その先に文字があるかもしれない
			pDst += strlen("\t");		// 次の文字に進む

			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{	// \t（タブ）：まだその先に文字があるかもしれない
					pDst += strlen("\t");		// 次の文字に進む
				}
				else
				{	// \tがみつからない
					break;
				}
			}	// while文
			break;
		}
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{	// 半角スペース：その先に文字あるかもしれない
			pDst += strlen(" ");		// 次の文字に進む
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{	// 半角スペース：まだその先に文字あるかもしれない
					pDst += strlen(" ");		// 次の文字に進む
				}
				else
				{	// 半角スペースがみつからない
					break;
				}
			}	// while文
			break;
		}
		else
		{	// 有効な文字列が見つかった
			break;
		}
	}	// while文

	return pDst;
}

//=============================================================================
// 文字列の先頭を設定するための処理
//=============================================================================
char *CPlayer::GetLineTop(char *pSrt)
{
	if (memcmp(pSrt, "\t", strlen("\t")) == 0)
	{	// \t（タブ）：その先に文字があるかもしれない
		pSrt += strlen("\t");		// 次の文字に進む
		while (1)
		{
			if (memcmp(pSrt, "\t", strlen("\t")) == 0)
			{	// \t（タブ）：その先に文字があるかもしれない
				pSrt += strlen("\t");		// 次の文字に進む
			}
			else
			{	// \tがみつからない
				break;
			}
			break;
		}
	}
	else if (memcmp(pSrt, " ", strlen(" ")) == 0)
	{	// 半角スペース：その先に文字あるかもしれない
		pSrt += strlen(" ");		// 次の文字に進む
		while (1)
		{
			if (memcmp(pSrt, " ", strlen(" ")) == 0)
			{	// 半角スペース：その先に文字あるかもしれない
				pSrt += strlen(" ");		// 次の文字に進む
			}
			else
			{	// 半角スペースがみつからない
				break;
			}
			break;
		}
	}

	return pSrt;
}

//=============================================================================
// 文字列を抜き出す処理
//=============================================================================
int CPlayer::PopString(char *pStr, char *pDst)
{	// スペースかタブがあったら終了
	int nWord = 0;

	while (1)
	{
		pStr += 1;		// 頭出し
		nWord += 1;	// 文字を一つ進める

		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	// \t（タブ）：その先に文字があるかもしれない
			pStr = "\0";		// \0を代入する
			nWord += strlen("\t");	// 次の文字に進む
			break;
		}
		else if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	// 半角スペース：その先に文字あるかもしれない
			pStr = "\0";		// \0を代入する
			nWord += 1;	// 文字を一つ進める
			break;
		}
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{	// \n：それ以降は文字がない
			pStr = "\0";
			nWord += strlen("\n");		// 次の文字に進む
			break;
		}
	}
	strcpy(pDst, pStr);						// 文字列を抜き出す

	return nWord;
}