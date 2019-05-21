//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//数字・ナンバー処理 [number.cpp]
//Author : Kobayashi_Sho-hei(小林 将兵)
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆

#ifndef _NUMBER_H_
#define _NUMBER_H_

//=================================================
//include / 他のデータ・コードの呼び出し・利用
//=================================================
#include "main.h"

//==============================================
//マクロ定義
//==============================================
#define TIME_TEX_NAME01	("data/TEXTURE/Nunber_original_master.png")

//==============================================
//ナンバー
//==============================================
class CNumber
{
public:
	//コンストラクタ / デストラクタ
	CNumber();
	~CNumber();

	//生成
	static CNumber *Create(void);

	//『使用する画像』の読み込み
	static HRESULT Load(void);
	static void UnLoad(void);

	//基盤となる関数(初期化 / 終了 / 更新 / 描画)
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	void SetPos(D3DXVECTOR3 pos, int nSpace, int nCnt);//(位置情報,間隔,桁分間隔をあける)
	void SetSize(float X, float Y);//ポリゴンのサイズ変更

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;
	float					m_SizeX;
	float					m_SizeY;


};




#endif