//���@���@���@���@���@���@���@���@���@���@���@
//�X�R�A���� [score.cpp]
//Author : Kobayashi_Sho-hei/���� ����
//���@���@���@���@���@���@���@���@���@���@���@
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "scene.h"

//==============================================
//�}�N����`
//==============================================
#define MAX_SCORE_DIGIT	(5)//�ő包��
#define MAX_RANK	(5)//�ő叇��
#define SCORE_SIZE_X	(40)
#define SCORE_SIZE_Y	(46)
#define SCORE_SPACE		(70)

class CNumber;
//==============================================
//�V�[���h�� �� �X�R�A
//==============================================
class CScore : public CScene
{
public:
	typedef enum
	{
		GAME_SCORE = 0,//�Q�[����Ŏg�p����X�R�A
		RANKING_SCORE,//�Q�[����Ŏg�p�����X�R�A�����ʂƂ��Ĕ��f������
		MAX_SCORE
	}SCORE_TYPE;

	//�R���X�g���N�^ / �f�X�g���N�^
	CScore();
	~CScore();

	//����
	static CScore *Create(D3DXVECTOR3 pos, SCORE_TYPE type);//�X�R�A�̐�������

	//��{�֐�
	virtual HRESULT Init(void) = 0;
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//�⏕
	//����
	int GetNowScore(void) { return m_nScore; }//���݃X�R�A�̎擾

	//���擾 / ���f
	void SetPos(D3DXVECTOR3 pos);
	void AddScore(int nValue);//�X�R�A���_

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	m_pos;//�|���S���̈ʒu


protected:
	int m_nScore;//���݂̃X�R�A
	int m_nTotalScore;
	int m_nScoreNo[MAX_SCORE_DIGIT];//����

};

//==============================================
//�X�R�A�h�� �� �Q�[����Ŏg�p����X�R�A
//==============================================
class CGameScore : public CScore
{
public:
	//�R���X�g���N�^ / �f�X�g���N�^
	CGameScore();
	~CGameScore();

	//��{�֐�
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:
	
	CNumber *m_apNumber[MAX_SCORE_DIGIT];
};

//==============================================
//�X�R�A�h�� �� �����L���O
//==============================================
class CRankingScore : public CScore
{
public:
	//�R���X�g���N�^ / �f�X�g���N�^
	CRankingScore();
	~CRankingScore();

	//��{�֐�
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRankingSort(int nScore);//�擾�����X�R�A���\�[�g

private:
	//�����L���O�����g�p(�S5�� / 7��)
	CNumber *m_apNumber[MAX_RANK][MAX_SCORE_DIGIT];
	int m_nDigt[MAX_RANK][MAX_SCORE_DIGIT];

};

#endif