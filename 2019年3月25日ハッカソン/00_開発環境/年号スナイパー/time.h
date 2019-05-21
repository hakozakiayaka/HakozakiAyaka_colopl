//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//タイマー処理 [time.h]
//Author : Kobayashi_Sho-hei(小林 将兵)
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _TIME_H_
#define _TIME_H_
#include "main.h"
#include "scene.h"

#define MAX_DIGIT	(5)
#define MAX_RANKING	(5)
class CNumber;

//==============================================
//シーン派生 → タイム
//==============================================
class CTime : public CScene
{
public:
	typedef enum
	{
		GAME_TIME = 0,//ゲーム上で使用するスコア
		RANKING_TIME,//ゲーム上で使用したスコアを結果として反映させる
		MAX_SCORE
	}TIME_TYPE;


	//コンストラクタ / デストラクタ
	CTime();
	~CTime();

	//生成
	static CTime *Create();

	//基盤となる関数(初期化 / 終了 / 更新 / 描画)
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			//ポリゴンの位置

	CNumber *m_apNumber[MAX_DIGIT];	//number処理の情報
	int m_nTimeDigit[MAX_DIGIT];	//桁数

	int m_nAddTime;				//	タイムの加算
	int m_nUnintTime;				//次の画面遷移までの時間

protected:
	int m_nTimeNow;;				//現在の時間
};

#endif