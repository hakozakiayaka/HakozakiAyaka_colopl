//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//�^�C�}�[���� [time.cpp]
//Author : Kobayashi_Sho-hei(���� ����)
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "time.h"
#include "number.h"
#include "fade.h"
#include "game.h"
#include "player.h"
#include "input.h"

//�}�N��
#define SCORE_SIZE_X	(40)
#define SCORE_SIZE_Y	(40)
#define SCORE_SPACE		(80)
#define SCORE_SIZE_posX	(200)
#define SCORE_SIZE_posY	(650)

#define INITIAL_TIME (0)//�����J�n����
#define END_TIME (0)//�Q�[���I������

//+ ���Ԃ̏����l�̓}�l�[�W�����Ǘ����Ă���!!

//=============================================================================
//+ �X�R�A�̃R���X�g���N�^
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
//+ �X�R�A�̃f�X�g���N�^
//=============================================================================
CTime::~CTime()
{

}
//=============================================================================
//+ �X�R�A�̐�������
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
//+ �X�R�A�̏���������
//=============================================================================
HRESULT CTime::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//���Ăяo����number�������g��
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
//+ �X�R�A�̏I������
//=============================================================================
void CTime::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//���Ăяo����number�������g��
		m_apNumber[nCnt]->Uninit();
		m_apNumber[nCnt] = NULL;
	}

	CManager::SetLastTime(m_nTimeNow);//manager�ۑ����Ă���X�R�A�̍��v�������L���O�ŕ\������

	//Release();
}
//=============================================================================
//+ �X�R�A�̍X�V����
//=============================================================================
void CTime::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//���Ăяo����number�������g��
		m_apNumber[nCnt]->Update();
		m_apNumber[nCnt]->SetNumber(m_nTimeDigit[nCnt]);//�J�E���g���Ԋu��������
	}

	//int digit = 10;//������
	//int newdigit01 = powf(digit, MAX_DIGIT - 1);//������ς���
	//int newdigit02 = powf(digit, MAX_DIGIT);//������ς���

	if (CPlayer::GetNengouUp() == false)
	{//	�N���������Ă���ꍇ
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
	{//	�N�����グ���ꍇ

		m_nTimeNow++;
	}

	m_nTimeDigit[0] = m_nTimeNow % 100000 / 10000;
	m_nTimeDigit[1] = m_nTimeNow % 10000 / 1000;
	m_nTimeDigit[2] = m_nTimeNow % 1000 / 100;
	m_nTimeDigit[3] = m_nTimeNow % 100 / 10;
	m_nTimeDigit[4] = m_nTimeNow % 10 / 1;

}
//=============================================================================
//+ �X�R�A�̕`�揈��
//=============================================================================
void CTime::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		//���Ăяo����number�������g��
		//m_apNumber[nCnt]->Draw();
	}
}

void CTime::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʏ�\���w�i(�T�C�Y�ω��A��]�Ȃ�,�e.cpp�ŃT�C�Y�̏���������)
	pVtx[0].pos = D3DXVECTOR3(-100, -100, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(+100, -100, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-100, +100, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(+100, +100, 0.0f) + m_pos;


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
