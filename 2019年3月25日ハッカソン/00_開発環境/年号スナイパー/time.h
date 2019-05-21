//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//�^�C�}�[���� [time.h]
//Author : Kobayashi_Sho-hei(���� ����)
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _TIME_H_
#define _TIME_H_
#include "main.h"
#include "scene.h"

#define MAX_DIGIT	(5)
#define MAX_RANKING	(5)
class CNumber;

//==============================================
//�V�[���h�� �� �^�C��
//==============================================
class CTime : public CScene
{
public:
	typedef enum
	{
		GAME_TIME = 0,//�Q�[����Ŏg�p����X�R�A
		RANKING_TIME,//�Q�[����Ŏg�p�����X�R�A�����ʂƂ��Ĕ��f������
		MAX_SCORE
	}TIME_TYPE;


	//�R���X�g���N�^ / �f�X�g���N�^
	CTime();
	~CTime();

	//����
	static CTime *Create();

	//��ՂƂȂ�֐�(������ / �I�� / �X�V / �`��)
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			//�|���S���̈ʒu

	CNumber *m_apNumber[MAX_DIGIT];	//number�����̏��
	int m_nTimeDigit[MAX_DIGIT];	//����

	int m_nAddTime;				//	�^�C���̉��Z
	int m_nUnintTime;				//���̉�ʑJ�ڂ܂ł̎���

protected:
	int m_nTimeNow;;				//���݂̎���
};

#endif