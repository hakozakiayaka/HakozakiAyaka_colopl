//◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　◇　
//ランキング処理 [ranking.h]
//Author : Kobayashi_Sho-hei/小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　
#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"
#include "manager.h"
#include "game.h"

//==============================================
//前方宣言
//==============================================
class CDebug;
class CSound;
class CLight;
class CCamera;
class CScene2D;
class CTexture2D;
class CScene3D;
class CMeshField;
class CNumber;
class CScore;
class CModel;
class CPlayer;
class CEnemy;

//==============================================
//派生なし → ランキング
//==============================================
class CRanking
{
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

	CRanking();
	~CRanking();

	static CRanking *Create(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ObjectLoad(void);			// オブジェクトの読み込み

	static CCamera *GetCamera(void) { return m_pCamera; }
	static CTexture2D *GetTexture2D(void) { return m_pTexture2D; }
	static CScene3D *GetScene3D(void) { return m_pScene3D; }
	static CMeshField *GetMeshField(void) { return m_pMeshField; }
	static CNumber *GetNumber(void) { return m_pNumber; }
	static CScore *GetScore(void) { return m_pScore; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static CEnemy *GetEnemy(void) { return m_pEnemy; }


private:

	static CDebug *m_pDebug;
	static CSound *m_pSound;
	static CLight *m_pLight;
	static CCamera *m_pCamera;
	static CScene2D *m_pScene2D;
	static CTexture2D *m_pTexture2D;
	static CScene3D *m_pScene3D;
	static CMeshField *m_pMeshField;
	static CNumber *m_pNumber;
	static CScore *m_pScore;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	TEXTSTATE	    m_textState[OBJTYPE_MAX];	//	テキスト読み込む変数[読み込むオブジェクトの種類]

	int m_nFadeCnt;//自動遷移用
};

#endif

