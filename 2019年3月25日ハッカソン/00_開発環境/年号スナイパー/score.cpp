//���@���@���@���@���@���@���@���@���@���@���@
//�X�R�A���� [score.cpp]
//Author : Kobayashi_Sho-hei/���� ����
//���@���@���@���@���@���@���@���@���@���@���@
#include "score.h"
#include "number.h"
//#include "ranking.h"
#include "manager.h"

//+ �X�R�A�̏����l�̓}�l�[�W�����Ǘ����Ă���!!
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScore::CScore() :CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_nScoreNo[nCnt] = 0;
		
	}

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}
//=============================================================================
//��������
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
			pScore->AddScore(0);//���[�h���ς���Ă���ɏ�����(�����p�������l)�X�R�A�̕\�����s��
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
//�\���ʒu
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-100, -100, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(+100, -100, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-100, +100, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(+100, +100, 0.0f) + m_pos;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�X�R�A�̉��_
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore = CManager::GetLastTime();//�����L���O�Ɋ֌W�Ȃ��ŐV�̃X�R�A��\������
	m_nTotalScore = CManager::GetLastScore(0);//�\���p�ɉ��Z���邽��(�����L���O�Ȃǂ̌��ʂɂ͊֌W�Ȃ�)
											  //(0)�̓^�C���̐��l���X�R�A�Ƃ��ĉ��Z���邪�����ł͏�ɉ��Z�����Ȃ�
	float digit = 10.0f;//������
	float newdigit01 = powf(digit, (float)MAX_SCORE_DIGIT-1.0f);//������ς���
	float newdigit02 = powf(digit, (float)MAX_SCORE_DIGIT);//������ς���

	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_nScoreNo[nCnt] = ((m_nScore +m_nTotalScore) % (int)newdigit02) / (int)newdigit01;
		newdigit01 = newdigit01 / 10;
		newdigit02 = newdigit02 / 10;
	}
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//++ �Q�[����Ŏg�p����X�R�A
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CGameScore::CGameScore()
{
	
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CGameScore::~CGameScore()
{

}

//=============================================================================
//����������
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
//�I������
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

	CManager::SetLastScore(m_nScore);//manager�ۑ����Ă���X�R�A�̍��v�������L���O�ŕ\������

	//Release();
}
//=============================================================================
//�X�V����
//=============================================================================
void CGameScore::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		
		m_apNumber[nCnt]->Update();
		m_apNumber[nCnt]->SetNumber(m_nScoreNo[nCnt]);//�J�E���g���d�Ȃ�Ȃ��悤�ɉ��ɂ��炷
	}
}
//=============================================================================
//�`�揈��
//=============================================================================
void CGameScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//++ �����L���O
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//=============================================================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//=============================================================================
CRankingScore::~CRankingScore()
{

}

//=============================================================================
//����������
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


	SetRankingSort(CManager::GetLastScore(CManager::GetLastTime()));//�ŏI�X�R�A+�X�R�A�������ŏI�^�C�������ʂɒǉ�

	return S_OK;
 }
//=============================================================================
//�I������
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
//�X�V����
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
//�`�揈��
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
//�擾�����X�R�A�����Ƃɏ��ʂ𔽉f(�\�[�g����)
//=============================================================================
void CRankingScore::SetRankingSort(int nScore)
{
	int nRank = 0;
	int nSave = 0;
	nRank  = nScore;
	int nSave02 = 0;
	//�~��
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
	//����
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


	//���Ɋւ��鏈��
	float digit = 10;//������
	float newdigit01 = powf(digit, (float)MAX_SCORE_DIGIT - 1.0f);//������ς���
	float newdigit02 = powf(digit, (float)MAX_SCORE_DIGIT);//������ς���

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
				 newdigit01 = powf(digit, MAX_SCORE_DIGIT - 1);//������ς���
				 newdigit02 = powf(digit, MAX_SCORE_DIGIT);//������ς���
			}

		}
	}

	CManager::SetLastScore(0);//���������Ȃ���ΐ������Ȃ�Ranking�ɂȂ�
}