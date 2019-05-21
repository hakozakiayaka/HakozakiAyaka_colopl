//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//タイマー処理 [time.cpp]
//Author : Kobayashi_Sho-hei(小林 将兵)
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "time.h"
#include "number.h"
#include "fade.h"
#include "game.h"
#include "player.h"
#include "input.h"

//マクロ
#define SCORE_SIZE_X	(40)
#define SCORE_SIZE_Y	(40)
#define SCORE_SPACE		(80)
#define SCORE_SIZE_posX	(200)
#define SCORE_SIZE_posY	(650)

#define INITIAL_TIME (0)//初期開始時間
#define END_TIME (0)//ゲーム終了時間

//+ 時間の初期値はマネージャが管理している!!

//=============================================================================
//+ スコアのコンストラクタ
//=============================================================================
CTime::CTime() :CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{
	m_nTimeNow = INITIAL_TIME *100;

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		m_nTimeDigit[nCnt] = 0;
		m_apNumber[nCnt] = NULL;
	}
	m_nAddTime = 0;

}
//=============================================================================
//+ スコアのデストラクタ
//=============================================================================
CTime::~CTime()
{

}
//=============================================================================
//+ スコアの生成処理
//=============================================================================
CTime *CTime::Create()
{
	CTime *pTime = NULL;

	if (pTime == NULL)
	{
		pTime = new CTime;
		pTime->Init();
	}

	return pTime;
}
//=============================================================================
//+ スコアの初期化処理
//=============================================================================
HRESULT CTime::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//↓呼び出したnumber処理を使う
		m_apNumber[nCnt] = new CNumber;
		m_apNumber[nCnt]->Init();
		m_apNumber[nCnt]->SetSize(SCORE_SIZE_X, SCORE_SIZE_Y);
		m_apNumber[nCnt]->SetPos(D3DXVECTOR3(SCORE_SIZE_posX, SCORE_SIZE_posY, 0.0f), SCORE_SPACE, nCnt);
	}

	m_nTimeNow = INITIAL_TIME *100;
	m_nUnintTime = 0;
	return S_OK;
}
//=============================================================================
//+ スコアの終了処理
//=============================================================================
void CTime::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//↓呼び出したnumber処理を使う
		m_apNumber[nCnt]->Uninit();
		m_apNumber[nCnt] = NULL;
	}

	CManager::SetLastTime(m_nTimeNow);//manager保存してあるスコアの合計をランキングで表示する

	//Release();
}
//=============================================================================
//+ スコアの更新処理
//=============================================================================
void CTime::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//↓呼び出したnumber処理を使う
		m_apNumber[nCnt]->Update();
		m_apNumber[nCnt]->SetNumber(m_nTimeDigit[nCnt]);//カウント分間隔をあける
	}

	//int digit = 10;//初期桁
	//int newdigit01 = powf(digit, MAX_DIGIT - 1);//桁数を変える
	//int newdigit02 = powf(digit, MAX_DIGIT);//桁数を変える

	if (CPlayer::GetNengouUp() == false)
	{//	年号を下げている場合
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
		{
			if (CPlayer::GetNengouUp() == false)
			{
				m_nAddTime = 10;
				m_nTimeNow += m_nAddTime;
			}
		}

	}
	else if (CPlayer::GetNengouUp() == true)
	{//	年号を上げた場合

		m_nTimeNow++;
	}

	m_nTimeDigit[0] = m_nTimeNow % 100000 / 10000;
	m_nTimeDigit[1] = m_nTimeNow % 10000 / 1000;
	m_nTimeDigit[2] = m_nTimeNow % 1000 / 100;
	m_nTimeDigit[3] = m_nTimeNow % 100 / 10;
	m_nTimeDigit[4] = m_nTimeNow % 10 / 1;

}
//=============================================================================
//+ スコアの描画処理
//=============================================================================
void CTime::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//↓呼び出したnumber処理を使う
		//m_apNumber[nCnt]->Draw();
	}
}

void CTime::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//通常表示背景(サイズ変化、回転なし,各.cppでサイズの書き換え可)
	pVtx[0].pos = D3DXVECTOR3(-100, -100, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(+100, -100, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-100, +100, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(+100, +100, 0.0f) + m_pos;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
