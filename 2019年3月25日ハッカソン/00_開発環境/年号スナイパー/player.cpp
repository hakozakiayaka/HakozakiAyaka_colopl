//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �����ʉ�
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "manager.h"			// �}�l�[�W���[
#include "renderer.h"			// �����_�����O
#include "input.h"				// ����
#include "DebugProc.h"			// �f�o�b�O�\��
#include "game.h"				// �Q�[��
#include "player.h"				// �v���C���[
#include "camera.h"				// �J����
#include "scene3D.h"			// scene3D
#include "fade.h"				// �t�F�[�h
#include "model.h"				//	���f��
#include "sound.h"				//	��

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
D3DXVECTOR3 CPlayer::m_PartsPos[MAX_PLAYER_PARTS] = {};			//�p�[�c�̈ʒu
D3DXVECTOR3 CPlayer::m_PartsRot[MAX_PLAYER_PARTS] = {};			//�p�[�c�̈ʒu
D3DXVECTOR3 CPlayer::m_PartsDefaultPos[MAX_PLAYER_PARTS] = {};	//�f�t�H���g�̃p�[�c�ʒu
D3DXVECTOR3 CPlayer::m_PartsDefaultRot[MAX_PLAYER_PARTS] = {};	//�f�t�H���g�̃p�[�c�p�x
D3DXVECTOR3 CPlayer::m_motionPos[MAX_PLAYER_PARTS] = {};		//���[�V������������p�ʒu
D3DXVECTOR3 CPlayer::m_motionRot[MAX_PLAYER_PARTS] = {};		//���[�V������������p�p�x
int CPlayer::m_aParent[MAX_PLAYER_PARTS] = {};					//�p�[�c�̐e
int CPlayer::m_aIndex[MAX_PLAYER_PARTS] = {};					//�p�[�c�ԍ�
bool CPlayer::m_bNengouUp = false;								//	�N�����グ�����ǂ���
bool CPlayer::m_bDisp = false;									//	���S�������ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_bDraw = true;			//�`�悷�邩
	m_pMesh = NULL;							// ���b�V���̃|�C���^
	m_pBuffMat = NULL;						// �o�b�t�@�̃|�C���^
	m_nNumMat = 0;						// �}�e���A�����
	m_nCountFream = 0;					//
	m_bNengouUp = false;
	m_bDisp = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// �v���C���[�̐���
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer * pPlayer = NULL;

	if (pPlayer == NULL)
	{	// �v���C���[�̓��I�m��
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{
			pPlayer->m_nLife = PLAYER_LIFE;		// ���C�t��ݒ�
			pPlayer->m_pos = pos;
			pPlayer->m_rot = rot;
			pPlayer->m_bAction[PLAYERBOOLTYPE_JUMP] = false;
			pPlayer->m_bAction[PLAYERBOOLTYPE_MOVE] = false;
			pPlayer->m_bAction[PLAYERBOOLTYPE_TURNUP] = false;
			pPlayer->Init();
		}
	}


	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �l�̏�����
	m_fAnswer = 0.0f;		// �x�N�g���̓���

	FILE *pFile = fopen(PLAYER_FILENAME, "r");	// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

	int nCntMotion = 0;							// ���[�V�����̃J�E���^
	int nCntKey = 0;							// �L�[�̃J�E���^
	int nCntParts = 0;							// �p�[�c�̃J�E���^
	int nCntModel = 0;							// �ǂݍ��ރ��f���̃J�E���^
	int nCntTexture = 0;						// �ǂݍ��ރe�N�X�`���̃J�E���^
	int nIndex = 0;								// �g�p���郂�f���̃^�C�v
	int nParent = 0;							// �e�̐���
	int nMaxParts = 0;							// �p�[�c���̍ő吔
	D3DXVECTOR3 pos;							// �ʒu������ϐ�
	D3DXVECTOR3 rot;							// ����������ϐ�
	m_nCntSpeed = 1.00f;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	char * pStuCur;					// ������̐擪�ւ̃|�C���^
	char aLine[MAX_CHAR];			// ������̓ǂݍ��ݗp�i1�s���j
	char aStr[MAX_CHAR];			// ������̔����o���p
	int nCntWord = 0;				// �����̃J�E���g
	char *pTop = NULL;				// �ꕶ���ڂ�ۑ�

	if (pFile != NULL)
	{
		pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s���ǂݍ���
		strcpy(aStr, pStuCur);									// ������𔲂��o��

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{	// SCRIPT�̕�����������
			pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s���ǂݍ���
			strcpy(aStr, pStuCur);									// ������𔲂��o��

			//*****************************************************************************
			// ���f���̓ǂݍ���
			//*****************************************************************************
			if (memcmp(aStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{	// NUM_MODEL = �̕�����������
				pStuCur += strlen("NUM_MODEL = ");			// �����̐�ɁA�h���f���̐��h������
				strcpy(aStr, pStuCur);					// ������𔲂��o��
				m_nNumModel = atoi(aStr);		// ������i���f���̐��j��l�ɕϊ�

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{	// �ǂݍ��񂾃��f���̐�������for���܂킷
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s���ǂݍ���
					strcpy(aStr, pStuCur);									// ������𔲂��o��

					// ���f���̐���for���܂킵�Ă��钆�ŁA"MODEL_FILENAME = "��������
					if (memcmp(aStr, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{	// MODEL_FILENAME = �̕�����������
						pStuCur += strlen("MODEL_FILENAME = ");			// �����̐�ɁA"�ǂݍ��ރ��f���̃f�[�^"������
						strcpy(aStr, pStuCur);													// ������𔲂��o��
						strcpy(m_aFilenameModel[nCntModel], pStuCur);		// ������𔲂��o��

						//*****************************************************************************
						// �]�v�ȕ�������菜������
						//*****************************************************************************
						int nWord = PopString(pStuCur, aStr);						// ������𔲂��o������
						strcpy(aStr, pStuCur);													// ������𔲂��o��
						aStr[nWord - 1] = '\0';
						strcpy(&m_aFilenameModel[nCntModel][0], aStr);			// ������𔲂��o��
					}	// MODEL_FILENAME�@�I��
				}	// �p�[�c������for���@�I��
			}	// NUM_MODEL�@�I��

				// �����ēǂݍ���ł�����
			pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
			strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

			//*****************************************************************************
			// �L�����N�^�[���
			//*****************************************************************************
			if (memcmp(aStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{	// CHARACTERSET�̕�����������
				while (1)
				{
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
					strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

					if (memcmp(aStr, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	// NUM_PARTS = �̕�����������
						pStuCur += strlen("NUM_PARTS = ");			// �����̐�ɁA�h���f���̃p�[�c���h������
						strcpy(aStr, pStuCur);					// ������𔲂��o��
						m_nNumParts = atoi(aStr);			// ������i���f���̃p�[�c���j��l�ɕϊ�
					}	// NUM_PARTS�@�I��
					if (memcmp(aStr, "PARTSSET", strlen("PARTSSET")) == 0)
					{	// PARTSSET�̕�����������
						while (1)
						{	// PARTSSET�̒��g���J��Ԃ�
							pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
							strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

							if (memcmp(aStr, "INDEX = ", strlen("INDEX = ")) == 0)
							{	// INDEX = �̕�����������
								pStuCur += strlen("INDEX = ");	// �����̐�ɁA�h���f���̔ԍ��h������
								strcpy(aStr, pStuCur);					// ������𔲂��o��
								nIndex = atoi(aStr);		// ������i���f���̔ԍ��j��l�ɕϊ�
							}	// INDEX�@�I��
							if (memcmp(aStr, "PARENT = ", strlen("PARENT = ")) == 0)
							{	// PARENT = �̕�����������
								pStuCur += strlen("PARENT = ");	// �����̐�ɁA�h���f���̔ԍ��h������
								strcpy(aStr, pStuCur);						// ������𔲂��o��
								m_aIndexParent[nIndex] = atoi(aStr);		// ������i���f���̔ԍ��j��l�ɕϊ�
							}	// PARENT�@�I��
							else if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
							{	// POS = �̕�����������
								pStuCur += strlen("POS = ");		// �����̐�ɁA�h���f���̈ʒu�h������
								strcpy(aStr, pStuCur);					// ������𔲂��o��

								// posX�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								pos.x = (float)atof(pStuCur);		// X�̒l���擾
								m_PartsPos[nIndex].x = pos.x;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�

								// posY�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								pos.y = (float)atof(pStuCur);		// Y�̒l���擾
								m_PartsPos[nIndex].y = pos.y;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�

								// posZ�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								pos.z = (float)atof(pStuCur);		// Z�̒l���擾
								m_PartsPos[nIndex].z = pos.z;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�
							}	// POS�@�I��
							else if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
							{	// ROT = �̕�����������
								pStuCur += strlen("ROT = ");				// �����̐�ɁA�h���f���̌����h������
								strcpy(aStr, pStuCur);						// ������𔲂��o��

								// rotX�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								rot.x = (float)atof(pStuCur);				// X�̒l���擾
								m_PartsRot[nIndex].x = rot.x;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�

								// rotY�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								rot.y = (float)atof(pStuCur);				// Y�̒l���擾
								m_PartsRot[nIndex].y = rot.y;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�

								// rotZ�̎擾----------------------------------------------------------------------------
								nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
								rot.z = (float)atof(pStuCur);				// Z�̒l���擾
								m_PartsRot[nIndex].z = rot.z;
								pStuCur += nCntWord;								// �擾�������������A������i�߂�
							}	// ROT�@�I��
							else if (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{	// END_PARTSSET�̕�����������
								if (m_apModel[nIndex] == NULL)
								{	// �|�C���^�̒��g�����������ĂȂ�������
									m_apModel[nIndex] = CModel::Create(m_PartsPos[nIndex], m_PartsRot[nIndex], m_nNumParts, CModel::MODELTYPE_PLAYER);	// ���f���̐���
									m_apModel[nIndex]->Load(m_aFilenameModel[nIndex]);		// ���f���̓ǂݍ���
								}
								if (m_apModel[nIndex] != NULL)
								{
									// ���f���̐e���w�肷��
									if (m_aIndexParent[nIndex] == -1)
									{	// 0�i���炾�j�̎���NULL
										m_apModel[nIndex]->SetParent(NULL);
									}
									else
									{	// ����ȊO
										m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);
									}
								}
								break;
							}	// END_PARTSSET�@�I��
						}	// PARTSSET while���@�I��
					}	// PARTSSET�@�I��
					else if (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{	// END_CHARACTERSET�̕�����������
						break;
					}	// END_CHARACTERSET�@�I��
				}	// CHARACTERSET while���@�I��
			}	// CHARACTERSET�@�I��

			if (nCntMotion < MOTIONSTATE_MAX)
			{// ���[�V�����̑����ȏ�͐ݒ肵�Ȃ�
				while (1)
				{
					pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
					strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

					if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
					{
						nCntKey = 0;
					}
					if (memcmp(aStr, "LOOP = ", strlen("LOOP = ")) == 0)
					{// LOOP����������
						pStuCur += strlen("LOOP = ");		// �����̐�ɁA�h���[�v���邩���Ȃ����h������
						strcpy(aStr, pStuCur);						// ������𔲂��o��
						m_aKeyInfo[nCntMotion].nLoop = atoi(aStr);		// ������i���[�v���邩���Ȃ������j��l�ɕϊ�
					}
					if (memcmp(aStr, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{// NUM_KEY����������
						pStuCur += strlen("NUM_KEY = ");		// �����̐�ɁA�h�L�[���h������
						strcpy(aStr, pStuCur);								// ������𔲂��o��
						m_aKeyInfo[nCntMotion].nNumKey = atoi(aStr);		// ������i�L�[���j��l�ɕϊ�
					}
					if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
					{
						nCntParts = 0;		// �p�[�c�̃J�E���g�ϐ���������

						while (1)
						{
							pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
							strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

							if (memcmp(aStr, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								pStuCur += strlen("FRAME = ");			// �����̐�ɁA�h�t���[�����h������
								strcpy(aStr, pStuCur);								// ������𔲂��o��
								m_aKeyInfo[nCntMotion].aKey[nCntKey].nFrame = atoi(aStr);		// ������i�t���[�����j��l�ɕϊ�
							}
							if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
							{
								while (1)
								{
									pStuCur = ReadLine(pFile, &aLine[0]);		// 1�s�ǂݍ���
									strcpy(aStr, pStuCur);									// �Ώۂ̕�����ɔ����o��

									if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
									{// POS����������
										pStuCur += strlen("POS = ");		// �����̐�ɁA�h���f���̈ʒu�h������
										strcpy(aStr, pStuCur);					// ������𔲂��o��

										// posX�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].x = (float)atof(pStuCur);						// X�̒l����
										pStuCur += nCntWord;								// �擾�������������A������i�߂�

										// posY�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].y = (float)atof(pStuCur);		// Y�̒l���擾
										pStuCur += nCntWord;								// �擾�������������A������i�߂�

										// posZ�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].z = (float)atof(pStuCur);		// Z�̒l���擾
										pStuCur += nCntWord;								// �擾�������������A������i�߂�
									}
									if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
									{// ROT����������
										pStuCur += strlen("ROT = ");		// �����̐�ɁA�h���f���̌����h������
										strcpy(aStr, pStuCur);					// ������𔲂��o��

										// rotX�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].x = (float)atof(pStuCur);		// X�̒l���擾
										pStuCur += nCntWord;								// �擾�������������A������i�߂�

										// rotY�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].y = (float)atof(pStuCur);		// Y�̒l���擾
										pStuCur += nCntWord;								// �擾�������������A������i�߂�

										// rotZ�̎擾----------------------------------------------------------------------------
										nCntWord = PopString(pStuCur, aStr);		// �������ڂɋ󔒂����������ǂ����𒲂ׂ�
										m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].z = (float)atof(pStuCur);		// Z�̒l���擾
										pStuCur += nCntWord;								// �擾�������������A������i�߂�
									}//ROT
									if (memcmp(aStr, "END_KEY", strlen("END_KEY")) == 0)
									{
										nCntParts++;
										break;
									}
								}
							}
							if (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								nCntKey++;
								break;
							}
						}	// KEY_SET
					}
					if (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						nCntMotion++;
					}
					if (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) == 0)
					{
						break;
					}
				}
			}
		}

		fclose(pFile);
	}
	else
	{
		printf(" �t�@�C���ǂݍ��ݎ��s�I\n");
	}

	//*****************************************************************************
	// ���[�V�����֌W
	//*****************************************************************************
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCntMotion = 0;
	m_motionState = MOTIONSTATE_NEUTRAL;

	//�v���C���[���
	m_fRadius = 5.0f; // �v���C���[�̓����蔻��̔��a
	m_StateCnt = 20;

	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_motionState].nNumKey; nCntKey++)
	{//���t���[�����̌v�Z
		m_nAllFrame += m_aKeyInfo[m_motionState].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// �p�[�c�������[�v
	 // ���f���̈ʒu���j���[�g�������[�V�����ɏ�����
		m_PartsDefaultPos[nCntParts] = m_PartsPos[nCntParts] + m_aKeyInfo[m_motionState].aKey[0].pos[nCntParts];
		m_PartsDefaultRot[nCntParts] = m_aKeyInfo[m_motionState].aKey[0].rot[nCntParts];
	}

	//2D�V�[���̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		//���f���̏I������
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}
	}

	// �t���O�𗧂Ă�
	CScene::SetDeath();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
	CCamera * pCamera = CManager::GetCamera();											// �}�l�[�W���[�̏��������Ă���

	// �O��̈ʒu�ɁA���݂̈ʒu��ۑ�
	m_posOld = m_pos;

	//=========================================
	// ���[�V�����̍X�V
	//=========================================
	PlayerMotionState(m_bAction);
	PlayerMotion();

	//*****************************************************************************
	// �v���C���[���ړ�
	//*****************************************************************************
	if (m_pos.x == 0.0f)
	{
		m_nCountFream++;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//=========================================
		// �U������
		//=========================================
		if (m_motionState != MOTIONSTATE_DEATH)
		{			// ���S��Ԃ���Ȃ�������
			if (m_nCountFream > rand() % 360 + 240)
			{
				if (m_bAction[PLAYERBOOLTYPE_TURNUP] == false)
				{
					m_bNengouUp = true;					//	�N�����グ��
					m_bAction[PLAYERBOOLTYPE_TURNUP] = true;
				}
			}
		}
		if (m_bAction[PLAYERBOOLTYPE_TURNUP] == true)
		{
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{	// �G���^�[�L�[������
				m_bAction[PLAYERBOOLTYPE_TURNUP] = false;
				m_motionState = MOTIONSTATE_DEATH;		// ���ʏ��
				m_bNengouUp = false;					//	�N�����グ��
				m_bDisp = true;
				CSound *pSound = CManager::GetSound();			//	�T�E���h�̎擾
				pSound->PlaySound(CSound::SOUND_LABEL_SE_BULLET);	//	���艹
			}
		}

	}
	else
	{
		m_move.x = m_nCntSpeed;
	}

	// �ړ��ʂ��ʒu�̒l�ɉ��Z
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	if (m_pos.x != 0.0f)
	{
		if (m_move.x < 0.3f && m_move.x > -0.3f && m_move.z < 0.3f && m_move.z > -0.3f)
		{
			if (m_motionState != MOTIONSTATE_NEUTRAL)
			{
				m_motionState = MOTIONSTATE_NEUTRAL;			// �ړ����
			}
		}
		else
		{
			if (m_motionState != MOTIONSTATE_MOVE)
			{
				m_motionState = MOTIONSTATE_MOVE;			// �ړ����
			}
		}

	}


	// �ړI�̊p�x�����߂�
	m_fAngle = m_fDestAngle - m_rot.y;


	//�ȑO�̃��[�V��������
	m_motionStateOld = m_motionState;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;											// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_�����O�N���X�̌Ăяo��

	if (pRenderer != NULL)
	{	//Renderer�N���X�̎擾
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;						//�v�Z�p�}�g���b�N�X
	D3DXPLANE planeField;											// �v���[���t�B�[���h
	D3DXVECTOR4 vecLight;										// ���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;									// �ʒu�A�@��
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��i�ʒu�j�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	// ���f���̕`�揈��
	if (m_bDraw == true)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
		{
			m_apModel[nCntModel]->Draw();
		}
	}
	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// ���[�V�����ݒ�
//=============================================================================
void CPlayer::PlayerMotion(void)
{
	//*****************************************************************************
	// ���[�V�����u�����h
	//*****************************************************************************
	if (m_motionState != m_motionStateOld)
	{	// ���[�V�����̏�Ԃ��ς����
		m_nCntFrame = 0;		// ���݂̃L�[�t���[����
		m_nCntKey = 0;			// �L�[�t���[��
		m_nCntAllFrame = 0;		// ���[�V�����t���[����
		m_nFrame = 5;				// ���[�V�����u�����h�̃t���[������ݒ�
		m_nAllFrame = 0;

		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_motionState].nNumKey; nCntKey++)
		{	// �t���[�����̌v�Z
			m_nAllFrame += m_aKeyInfo[m_motionState].aKey[nCntKey].nFrame;
		}
		m_motionBlend = true;		// ���[�V�����u�����h�F�I��
	}

	//*****************************************************************************
	// �L�[�t���[���̓���ւ��
	//*****************************************************************************
	if (m_nCntFrame == m_nFrame)
	{
		m_nCntFrame = 0;		// ���݂̃L�[�t���[���̃t���[������������
		m_nFrame = m_aKeyInfo[m_motionState].aKey[m_nCntKey].nFrame;		// ���[�V�����̃t���[������ݒ�
		m_nCntKey++;		// �L�[�t���[����i�߂�

		if (m_nCntKey == m_aKeyInfo[m_motionState].nNumKey)
		{	// �L�[�t���[�������Ō�܂ł���
			if (m_aKeyInfo[m_motionState].nLoop == 0)
			{	// ���[�v���Ȃ���
				switch (m_motionState)
				{	// ���[�V�������A
				case MOTIONSTATE_TURNUP:		// �W�����v�̎�
					m_nCntKey--;		// �L�[�t���[����߂�
					break;

				case MOTIONSTATE_DEATH:		// �U���̎�
					m_bAction[PLAYERBOOLTYPE_DEATH] = false;
					m_nCntKey--;		// �L�[�t���[����߂�
					break;
				}
			}
			else
			{	// ���[�v����
				m_nCntKey = m_nCntKey % m_aKeyInfo[m_motionState].nNumKey;		// �L�[�t���[����i�߂�
				m_nCntAllFrame = 0;		 // ���[�V�����̃t���[����(ALL)��������
			}
		}
	}

	//*****************************************************************************
	// ���[�V�����̍�������
	//*****************************************************************************
	if (m_nCntFrame == 0)
	{	// �t���[�������ŏ��̎�
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{	// �p�[�c�������[�v
			// ���[�V�����̈ʒu��������
			m_motionPos[nCntParts] = (m_aKeyInfo[m_motionState].aKey[m_nCntKey].pos[nCntParts] + m_motionPos[nCntParts]) - m_PartsDefaultPos[nCntParts];

			// ���[�V�����̉�]��������
			m_motionRot[nCntParts] = m_aKeyInfo[m_motionState].aKey[m_nCntKey].rot[nCntParts] - m_PartsDefaultRot[nCntParts];

			//*****************************************************************************
			// ��]�̍������C������
			//*****************************************************************************
			//-----------------------------------------------------------------------
			// X���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].x > D3DX_PI)
			{	// +3.14���A������
				m_motionRot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].x < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].x += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			// Y���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].y > D3DX_PI)
			{// +3.14���A������
				m_motionRot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].y < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].y += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			//  Z���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].z > D3DX_PI)
			{// +3.14���A������
				m_motionRot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].z < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].z += D3DX_PI * 2.0f;
			}
		}
	}

	//*****************************************************************************
	// ���[�V�����̓���
	//*****************************************************************************
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{	// �p�[�c�������[�v
		if (m_apModel[nCntParts] != NULL)
		{
			// �ʒu�������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultPos[nCntParts].x += m_motionPos[nCntParts].x / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultPos[nCntParts].y += m_motionPos[nCntParts].y / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultPos[nCntParts].z += m_motionPos[nCntParts].z / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���

			// �ʒu�̑��
			m_apModel[nCntParts]->SetPostion(m_PartsDefaultPos[nCntParts]);

			// ��]�������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultRot[nCntParts].x += m_motionRot[nCntParts].x / m_nFrame;				// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultRot[nCntParts].y += m_motionRot[nCntParts].y / m_nFrame;				// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultRot[nCntParts].z += m_motionRot[nCntParts].z / m_nFrame;				// ��]�̍������t���[�����Ŋ������l�����Z���Ă���

			//��]�l�̑��
			m_apModel[nCntParts]->SetRotation(m_PartsDefaultRot[nCntParts]);
		}

			//*****************************************************************************
			// ���f���̉�]���C������
			//*****************************************************************************
			//-----------------------------------------------------------------------
			// X���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].x > D3DX_PI)
			{	// +3.14���A������
				m_motionRot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].x < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].x += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			// Y���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].y > D3DX_PI)
			{// +3.14���A������
				m_motionRot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].y < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].y += D3DX_PI * 2.0f;
			}

			//-----------------------------------------------------------------------
			//  Z���̏C��
			//-----------------------------------------------------------------------
			if (m_motionRot[nCntParts].z > D3DX_PI)
			{// +3.14���A������
				m_motionRot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_motionRot[nCntParts].z < -D3DX_PI)
			{// -3.14���A�������
				m_motionRot[nCntParts].z += D3DX_PI * 2.0f;
			}
		}

		m_nCntFrame++;			// ���݂̃L�[�t���[���̃t���[���������Z
		m_nCntAllFrame++;		// ���[�V�����̃t���[���������Z
}

//=============================================================================
// ���[�V�����X�e�[�g�̍X�V
//=============================================================================
void CPlayer::PlayerMotionState(bool *bAction)
{
	//switch (*bAction)
	//{
	//case PLAYERBOOLTYPE_ATTACK:
	//	break;
	//}
	//if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	//{	// ���S�t���O
	//	m_motionState = MOTIONSTATE_DEATH;		// ���S���
	//}
	//else if (bAction[PLAYERBOOLTYPE_DAMAGE] == true)
	//{	// �_���[�W�t���O
	//	m_motionState = MOTIONSTATE_DAMAGE;		// �_���[�W���
	//}
	if (bAction[PLAYERBOOLTYPE_TURNUP] == true)
	{	// ���n�t���O
		m_motionState = MOTIONSTATE_TURNUP;	// ���n���
	}
	else if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	{	// �v���C���[���U��
		m_motionState = MOTIONSTATE_DEATH;
	}
	else if (bAction[PLAYERBOOLTYPE_MOVE] == true)
	{
		if (m_move.x > PLAYER_MOVE ||
			m_move.x < -PLAYER_MOVE ||
			m_move.z >  PLAYER_MOVE ||
			m_move.z < -PLAYER_MOVE)
		{	// �v���C���[�������Ă���Ƃ�
			m_motionState = MOTIONSTATE_MOVE;			// �ړ����
		}
	}
	else
	{	// �v���C���[�������ĂȂ��Ƃ�
		//m_motionState = MOTIONSTATE_NEUTRAL;	 // �j���[�g�������
	}
}

//=============================================================================
// 1�s�ǂݍ��ނ��߂̏���
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{	// pDst�ɕ���������܂ő�����
	while (1)
	{
		fgets(pDst, MAX_CHAR, pFile);		// ������̓ǂݍ���

		if (memcmp(pDst, "#", strlen("#")) == 0)
		{	// #�F�R�����g�s�����玟�̍s�ɐi��
			pDst += strlen("#");		// ���̕����ɐi��
		}
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{	// \n�F����ȍ~�͕������Ȃ�
			pDst += strlen("\n");		// ���̕����ɐi��
		}
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{	// \t�i�^�u�j�F���̐�ɕ��������邩������Ȃ�
			pDst += strlen("\t");		// ���̕����ɐi��

			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{	// \t�i�^�u�j�F�܂����̐�ɕ��������邩������Ȃ�
					pDst += strlen("\t");		// ���̕����ɐi��
				}
				else
				{	// \t���݂���Ȃ�
					break;
				}
			}	// while��
			break;
		}
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{	// ���p�X�y�[�X�F���̐�ɕ������邩������Ȃ�
			pDst += strlen(" ");		// ���̕����ɐi��
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{	// ���p�X�y�[�X�F�܂����̐�ɕ������邩������Ȃ�
					pDst += strlen(" ");		// ���̕����ɐi��
				}
				else
				{	// ���p�X�y�[�X���݂���Ȃ�
					break;
				}
			}	// while��
			break;
		}
		else
		{	// �L���ȕ����񂪌�������
			break;
		}
	}	// while��

	return pDst;
}

//=============================================================================
// ������̐擪��ݒ肷�邽�߂̏���
//=============================================================================
char *CPlayer::GetLineTop(char *pSrt)
{
	if (memcmp(pSrt, "\t", strlen("\t")) == 0)
	{	// \t�i�^�u�j�F���̐�ɕ��������邩������Ȃ�
		pSrt += strlen("\t");		// ���̕����ɐi��
		while (1)
		{
			if (memcmp(pSrt, "\t", strlen("\t")) == 0)
			{	// \t�i�^�u�j�F���̐�ɕ��������邩������Ȃ�
				pSrt += strlen("\t");		// ���̕����ɐi��
			}
			else
			{	// \t���݂���Ȃ�
				break;
			}
			break;
		}
	}
	else if (memcmp(pSrt, " ", strlen(" ")) == 0)
	{	// ���p�X�y�[�X�F���̐�ɕ������邩������Ȃ�
		pSrt += strlen(" ");		// ���̕����ɐi��
		while (1)
		{
			if (memcmp(pSrt, " ", strlen(" ")) == 0)
			{	// ���p�X�y�[�X�F���̐�ɕ������邩������Ȃ�
				pSrt += strlen(" ");		// ���̕����ɐi��
			}
			else
			{	// ���p�X�y�[�X���݂���Ȃ�
				break;
			}
			break;
		}
	}

	return pSrt;
}

//=============================================================================
// ������𔲂��o������
//=============================================================================
int CPlayer::PopString(char *pStr, char *pDst)
{	// �X�y�[�X���^�u����������I��
	int nWord = 0;

	while (1)
	{
		pStr += 1;		// ���o��
		nWord += 1;	// ��������i�߂�

		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	// \t�i�^�u�j�F���̐�ɕ��������邩������Ȃ�
			pStr = "\0";		// \0��������
			nWord += strlen("\t");	// ���̕����ɐi��
			break;
		}
		else if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	// ���p�X�y�[�X�F���̐�ɕ������邩������Ȃ�
			pStr = "\0";		// \0��������
			nWord += 1;	// ��������i�߂�
			break;
		}
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{	// \n�F����ȍ~�͕������Ȃ�
			pStr = "\0";
			nWord += strlen("\n");		// ���̕����ɐi��
			break;
		}
	}
	strcpy(pDst, pStr);						// ������𔲂��o��

	return nWord;
}