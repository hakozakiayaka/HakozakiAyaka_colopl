//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//�����E�i���o�[���� [number.cpp]
//Author : Kobayashi_Sho-hei(���� ����)
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��

#ifndef _NUMBER_H_
#define _NUMBER_H_

//=================================================
//include / ���̃f�[�^�E�R�[�h�̌Ăяo���E���p
//=================================================
#include "main.h"

//==============================================
//�}�N����`
//==============================================
#define TIME_TEX_NAME01	("data/TEXTURE/Nunber_original_master.png")

//==============================================
//�i���o�[
//==============================================
class CNumber
{
public:
	//�R���X�g���N�^ / �f�X�g���N�^
	CNumber();
	~CNumber();

	//����
	static CNumber *Create(void);

	//�w�g�p����摜�x�̓ǂݍ���
	static HRESULT Load(void);
	static void UnLoad(void);

	//��ՂƂȂ�֐�(������ / �I�� / �X�V / �`��)
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	void SetPos(D3DXVECTOR3 pos, int nSpace, int nCnt);//(�ʒu���,�Ԋu,�����Ԋu��������)
	void SetSize(float X, float Y);//�|���S���̃T�C�Y�ύX

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;
	float					m_SizeX;
	float					m_SizeY;


};




#endif