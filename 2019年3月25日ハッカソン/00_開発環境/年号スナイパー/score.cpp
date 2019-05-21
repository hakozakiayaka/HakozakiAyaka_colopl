//◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　
//スコア処理 [score.cpp]
//Author : Kobayashi_Sho-hei/小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　
#include "score.h"
#include "number.h"
//#include "ranking.h"
#include "manager.h"

//+ スコアの初期値はマネージャが管理している!!
//=============================================================================
//コンストラクタ
//=============================================================================
CScore::CScore() :CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_nScoreNo[nCnt] = 0;
		
	}

}
//=============================================================================
//デストラクタ
//=============================================================================
CScore::~CScore()
{

}
//=============================================================================
//生成処理
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos,SCORE_TYPE type)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		switch (type)
		{
		case GAME_SCORE:
			pScore = new CGameScore;
			pScore->AddScore(0);//モードが変わっても常に初期で(引き継いだ数値)スコアの表示を行う
			pScore->Init(pos);
			break;

		case RANKING_SCORE:
			pScore = new CRankingScore;
			pScore->Init(pos);

			break;
		}
	}

	return pScore;
}

//=============================================================================
//表示位置
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-100, -100, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(+100, -100, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-100, +100, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(+100, +100, 0.0f) + m_pos;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//スコアの加点
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore = CManager::GetLastTime();//ランキングに関係なく最新のスコアを表示する
	m_nTotalScore = CManager::GetLastScore(0);//表示用に加算するため(ランキングなどの結果には関係ない)
											  //(0)はタイムの数値をスコアとして加算するがここでは常に加算させない
	float digit = 10.0f;//初期桁
	float newdigit01 = powf(digit, (float)MAX_SCORE_DIGIT-1.0f);//桁数を変える
	float newdigit02 = powf(digit, (float)MAX_SCORE_DIGIT);//桁数を変える

	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_nScoreNo[nCnt] = ((m_nScore +m_nTotalScore) % (int)newdigit02) / (int)newdigit01;
		newdigit01 = newdigit01 / 10;
		newdigit02 = newdigit02 / 10;
	}
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//++ ゲーム上で使用するスコア
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//=============================================================================
//コンストラクタ
//=============================================================================
CGameScore::CGameScore()
{
	
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

}
//=============================================================================
//デストラクタ
//=============================================================================
CGameScore::~CGameScore()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CGameScore::Init(void)
{
	return S_OK;
}
HRESULT CGameScore::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = new CNumber;
		m_apNumber[nCnt]->Init();
		m_apNumber[nCnt]->SetSize(SCORE_SIZE_X, SCORE_SIZE_Y);
		m_apNumber[nCnt]->SetPos(pos, SCORE_SPACE, nCnt);
	}
	return S_OK;
}
//=============================================================================
//終了処理
//=============================================================================
void CGameScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = NULL;
		}
	}

	CManager::SetLastScore(m_nScore);//manager保存してあるスコアの合計をランキングで表示する

	//Release();
}
//=============================================================================
//更新処理
//=============================================================================
void CGameScore::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		
		m_apNumber[nCnt]->Update();
		m_apNumber[nCnt]->SetNumber(m_nScoreNo[nCnt]);//カウント分重ならないように横にずらす
	}
}
//=============================================================================
//描画処理
//=============================================================================
void CGameScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//++ ランキング
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//=============================================================================
//コンストラクタ
//=============================================================================
CRankingScore::CRankingScore()
{
	for (int nRankNo = 0; nRankNo < MAX_RANK; nRankNo++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			m_apNumber[nRankNo][nCnt] = NULL;
			m_nDigt[nRankNo][nCnt] = 0;
		}
	}
}
//=============================================================================
//デストラクタ
//=============================================================================
CRankingScore::~CRankingScore()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRankingScore::Init(void)
{
	return S_OK;
}
HRESULT CRankingScore::Init(D3DXVECTOR3 pos)
{

	for (int nRankNo = 0; nRankNo < MAX_RANK; nRankNo++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			m_apNumber[nRankNo][nCnt] = new CNumber;
			m_apNumber[nRankNo][nCnt]->Init();

			m_apNumber[nRankNo][nCnt]->SetSize(35.0f, 35.0f);
			if (nRankNo == 0)
			{
				m_apNumber[nRankNo][nCnt]->SetSize(60.0f, 60.0f);
			}
			if (nRankNo == 1)
			{
				m_apNumber[nRankNo][nCnt]->SetSize(45.0f, 45.0f);
				
			}
			/*if (nRankNo >= 0)
			{
				m_apNumber[nRankNo][nCnt]->SetPos(pos + D3DXVECTOR3(-100.0f + (20.0f* (float)nRankNo), (120.0f* (float)nRankNo), 0.0f), 120 - (20 * nRankNo), nCnt);
			}*/
			m_apNumber[nRankNo][nCnt]->SetPos(pos + D3DXVECTOR3(-100.0f+(80.0f* (float)nRankNo),(170.0f* (float)nRankNo),0.0f), 120-(20* nRankNo), nCnt);
			if (nRankNo >= 2)
			{
				m_apNumber[nRankNo][nCnt]->SetPos(pos + D3DXVECTOR3(100.0f, 120.0f+(100.0f* (float)nRankNo), 0.0f), 80, nCnt);

			}
			

		}
	}


	SetRankingSort(CManager::GetLastScore(CManager::GetLastTime()));//最終スコア+スコア化した最終タイムを順位に追加

	return S_OK;
 }
//=============================================================================
//終了処理
//=============================================================================
void CRankingScore::Uninit(void)
{
	for (int nRankNo = 0; nRankNo < MAX_RANK; nRankNo++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			m_apNumber[nRankNo][nCnt]->Uninit();
			m_apNumber[nRankNo][nCnt] = NULL;
		}
	}

	//Release();
}
//=============================================================================
//更新処理
//=============================================================================
void CRankingScore::Update(void)
{
	for (int nRankNo = 0; nRankNo < MAX_RANK; nRankNo++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			
			//m_apNumber[nRankNo][nCnt]->Update();
		
		}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CRankingScore::Draw(void)
{
	for (int nRankNo = 0; nRankNo < MAX_RANK; nRankNo++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
		
			m_apNumber[nRankNo][nCnt]->Draw();
		}

	}
}

//=============================================================================
//取得したスコアをもとに順位を反映(ソート処理)
//=============================================================================
void CRankingScore::SetRankingSort(int nScore)
{
	int nRank = 0;
	int nSave = 0;
	nRank  = nScore;
	int nSave02 = 0;
	//降順
	/*for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (nRank > CManager::nRank[nCnt])
		{
			nSave = CManager::nRank[nCnt];
			CManager::nRank[nCnt] = nRank;
			nRank = nSave;
			for (int nSortCnt = nCnt + 1; nSortCnt < MAX_RANK; nSortCnt++)
			{
				
				nSave02 = CManager::nRank[nSortCnt];
				CManager::nRank[nSortCnt] = nRank;
				nRank = nSave02;
			}
			break;
		}
	}*/
	//昇順
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (nRank < CManager::nRank[nCnt])
		{
			nSave = CManager::nRank[nCnt];
			CManager::nRank[nCnt] = nRank;
			nRank = nSave;
			for (int nSortCnt = nCnt + 1; nSortCnt < MAX_RANK; nSortCnt++)
			{

				nSave02 = CManager::nRank[nSortCnt];
				CManager::nRank[nSortCnt] = nRank;
				nRank = nSave02;
			}

			break;
		}
	}


	//桁に関する処理
	float digit = 10;//初期桁
	float newdigit01 = powf(digit, (float)MAX_SCORE_DIGIT - 1.0f);//桁数を変える
	float newdigit02 = powf(digit, (float)MAX_SCORE_DIGIT);//桁数を変える

	for (int Rank = 0; Rank < MAX_RANK; Rank++)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
		{
			m_nDigt[Rank][nCnt] = (CManager::nRank[Rank] % (int)newdigit02) / (int)newdigit01;
			
			m_apNumber[Rank][nCnt]->SetNumber(m_nDigt[Rank][nCnt]);
			newdigit01 = newdigit01 / 10;
			newdigit02 = newdigit02 / 10;
			if (nCnt == MAX_SCORE_DIGIT - 1)
			{
				 newdigit01 = powf(digit, MAX_SCORE_DIGIT - 1);//桁数を変える
				 newdigit02 = powf(digit, MAX_SCORE_DIGIT);//桁数を変える
			}

		}
	}

	CManager::SetLastScore(0);//初期化しなければ正しくないRankingになる
}