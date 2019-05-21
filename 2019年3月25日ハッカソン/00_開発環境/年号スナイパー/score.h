//◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　
//スコア処理 [score.cpp]
//Author : Kobayashi_Sho-hei/小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "scene.h"

//==============================================
//マクロ定義
//==============================================
#define MAX_SCORE_DIGIT	(5)//最大桁数
#define MAX_RANK	(5)//最大順位
#define SCORE_SIZE_X	(40)
#define SCORE_SIZE_Y	(46)
#define SCORE_SPACE		(70)

class CNumber;
//==============================================
//シーン派生 → スコア
//==============================================
class CScore : public CScene
{
public:
	typedef enum
	{
		GAME_SCORE = 0,//ゲーム上で使用するスコア
		RANKING_SCORE,//ゲーム上で使用したスコアを結果として反映させる
		MAX_SCORE
	}SCORE_TYPE;

	//コンストラクタ / デストラクタ
	CScore();
	~CScore();

	//生成
	static CScore *Create(D3DXVECTOR3 pos, SCORE_TYPE type);//スコアの生成処理

	//基本関数
	virtual HRESULT Init(void) = 0;
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//補助
	//情報提供
	int GetNowScore(void) { return m_nScore; }//現在スコアの取得

	//情報取得 / 反映
	void SetPos(D3DXVECTOR3 pos);
	void AddScore(int nValue);//スコア加点

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点バッファへのポインタ
	D3DXVECTOR3	m_pos;//ポリゴンの位置


protected:
	int m_nScore;//現在のスコア
	int m_nTotalScore;
	int m_nScoreNo[MAX_SCORE_DIGIT];//桁数

};

//==============================================
//スコア派生 → ゲーム上で使用するスコア
//==============================================
class CGameScore : public CScore
{
public:
	//コンストラクタ / デストラクタ
	CGameScore();
	~CGameScore();

	//基本関数
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:
	
	CNumber *m_apNumber[MAX_SCORE_DIGIT];
};

//==============================================
//スコア派生 → ランキング
//==============================================
class CRankingScore : public CScore
{
public:
	//コンストラクタ / デストラクタ
	CRankingScore();
	~CRankingScore();

	//基本関数
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRankingSort(int nScore);//取得したスコアをソート

private:
	//ランキング個数分使用(全5位 / 7桁)
	CNumber *m_apNumber[MAX_RANK][MAX_SCORE_DIGIT];
	int m_nDigt[MAX_RANK][MAX_SCORE_DIGIT];

};

#endif