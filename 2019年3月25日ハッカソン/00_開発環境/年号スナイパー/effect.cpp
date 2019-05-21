//=============================================================================
//
// �G�t�F�N�g(�p�[�e�B�N��)���� [effect.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "effect.h"			// �G�t�F�N�g
#include "scene.h"			// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int g_nCounteraaa = 0;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9   CEffect::m_pTexture[MAX_EFFECTTYPE] = {};
char				 CEffect::m_cTextName[128] = {};
char				 CEffect::m_cTextureName[MAX_EFFECTTYPE][128] = {};
int					 CEffect::m_nTexttureType = NULL;
CEffect::EFFECT_STATE CEffect::m_EffectState = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect() : CScene(CScene::PRIORITY_5, CScene::OBJTYPE_EFFECT)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{
		m_apScene3D[nCount] = NULL;
		m_NumMove[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//	�����̊��蓖��
		m_nNumLife[nCount] = 0;												//	����
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//	�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//	��]
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);								//	�F
	m_TexUV = D3DXVECTOR2(0.0f, 0.0f);										//	UV
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//	�傫��
	m_posRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//	�ʒu�̕�
	m_nSetPoriMAX = 0;														//	�|���S���̐�
	m_nEffectType = 0;														//	�G�t�F�N�g�̎��
	m_nLife = 0;															//	����
	m_fGravity = 0;															//	�d��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}
//=============================================================================
//	�G�t�F�N�g�̐���
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, int textype, int txsttype)
{
	CEffect *pEffect = NULL;
	if (pEffect == NULL)
	{//	�A�C�e���̐���
		pEffect = new CEffect;
		if (pEffect != NULL)
		{//�A�C�e���̏�����
			pEffect->m_pos = pos;							//	�ʒu�̐ݒ�
			pEffect->m_nEffectType = textype;				//	�G�t�F�N�g�̉摜�̎��
			pEffect->m_nBindText = txsttype;				//	�G�t�F�N�g�̉��o���蓖��
			pEffect->Init();
		}
	}
	return pEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(void)
{
	for (int nCount = 0; nCount < m_EffectState.nSetPoriMAX[m_nBindText]; nCount++)
	{//	�m�ۂ�����
	 //	�ϐ��̏�����
		m_apScene3D[nCount] = NULL;
		if (m_apScene3D[nCount] == NULL)
		{//	�ϐ��ɉ��������Ă��Ȃ������ꍇ
		 //	�������̊m��
			m_apScene3D[nCount] = new CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_EFFECT);
			if (m_apScene3D[nCount] != NULL)
			{//	3D�|���S���̃��������m�ۏo������
			 //	�����̏����ݒ�
				m_NumMove[nCount] = D3DXVECTOR3(
					(sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_EffectState.move[m_nBindText].x)),
					(cosf((rand() % 314 / 2) / 100.0f) * ((rand() % (int)1) + m_EffectState.move[m_nBindText].y)),
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_EffectState.move[m_nBindText].z)));
				//	�ʒu�̏����ݒ�
				m_pos += D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_EffectState.posRange[m_nBindText].x)),
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_EffectState.posRange[m_nBindText].y)),
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_EffectState.posRange[m_nBindText].z)));
				//	�����̐ݒ�
				m_nNumLife[nCount] = m_EffectState.nLife[m_nBindText];

				//	�e�N�X�`���̊��蓖��
				m_apScene3D[nCount]->BindTexture(m_pTexture[m_nEffectType]);

				if (m_EffectState.nBillType[m_nBindText] == 0)
				{//	���Z�����Ȃ�
					m_apScene3D[nCount]->SetInitAll(m_pos, m_EffectState.rot[m_nBindText], m_EffectState.size[m_nBindText], m_EffectState.col[m_nBindText], m_EffectState.TexUV[m_nBindText], CScene3D::SCENE3DTYPE_BILLBOARD);
				}
				else if (m_EffectState.nBillType[m_nBindText] == 1)
				{//	���Z��������
					m_apScene3D[nCount]->SetInitAll(m_pos, m_EffectState.rot[m_nBindText], m_EffectState.size[m_nBindText], m_EffectState.col[m_nBindText], m_EffectState.TexUV[m_nBindText], CScene3D::SCENE3DTYPE_BILLEFFECT);
				}
				m_apScene3D[nCount]->Init();
			}
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{//	�ő吔����
		if (m_apScene3D[nCount] != NULL)
		{//	�g�p����Ă��Ȃ�������
			m_apScene3D[nCount]->Uninit();
			m_apScene3D[nCount] = NULL;
		}
	}
	// �V�[���J��
	CScene::SetDeath();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos[MAX_EFFECT];							//	�ʒu�̎擾
	D3DXVECTOR3 rot[MAX_EFFECT];							//	�����̐ݒ�
	D3DXVECTOR3 size[MAX_EFFECT];							//	�傫���̎擾
	D3DXCOLOR   col[MAX_EFFECT];							//	�F�̐ݒ�

	CInputKeyboard *pInputKeybaod = CManager::GetInputKeyboard();

	for (int nCount = 0; nCount < m_EffectState.nSetPoriMAX[m_nBindText]; nCount++)
	{//	�|���S���̐�����
		if (m_apScene3D[nCount] != NULL)
		{//	�󂶂�Ȃ������ׂ�
			pos[nCount] = m_apScene3D[nCount]->Getpos();	//	�ʒu�̎擾
			rot[nCount] = m_apScene3D[nCount]->Getrot();	//	�����̐ݒ�
			size[nCount] = m_apScene3D[nCount]->Getsize();	//	�傫���̎擾
			col[nCount] = m_apScene3D[nCount]->Getcol();	//	�F�̐ݒ�

			//	�ʒu�ɓ�����������
			pos[nCount] += m_NumMove[nCount];
			//	�d�͂̐ݒ�
			m_NumMove[nCount].y -= m_fGravity;

			m_apScene3D[nCount]->Setpos(pos[nCount]);		//	�ʒu�̐ݒ�
			m_apScene3D[nCount]->Setsize(size[nCount]);		//	�傫���̐ݒ�
			m_apScene3D[nCount]->Setcol(col[nCount]);		//	�F�̐ݒ�
			m_apScene3D[nCount]->Setrot(rot[nCount]);		//	�����̐ݒ�
			//	�̗͂̌���
			m_nNumLife[nCount]--;
			if (m_nNumLife[nCount] < 0)
			{//	���C�t��0�ȉ��ɂȂ����ꍇ����
				Uninit();
			}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{

}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CEffect::Load(void)
{
	LoadTexNameEffect();
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	for (int nCount = 0; nCount < m_nTexttureType; nCount++)
	{//	�e�N�X�`���̐�����
		D3DXCreateTextureFromFile(pDevice, &m_cTextureName[nCount][0], &m_pTexture[nCount]);
	}

	return S_OK;
}
//=============================================================================
// �A�����[�h����
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{//	�G�t�F�N�g�̃f�[�^�̏�����
		m_EffectState.posRange[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//	�ʒu�͈̔�
		m_EffectState.rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//	����
		m_EffectState.move[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//	����
		m_EffectState.size[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//	�ʒu
		m_EffectState.col[nCount] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//	�F
		m_EffectState.TexUV[nCount] = D3DXVECTOR2(0.0f, 0.0f);			//	UV
		m_EffectState.nSetPoriMAX[nCount] = 0;							//	�|���S���̐�
		m_EffectState.fGravity[nCount] = 0;								//	�d��
		m_EffectState.nLife[nCount] = 0;								//	����
		m_EffectState.nBillType[nCount] = false;						//	���Z�������肩�Ȃ���
	}
	m_EffectState.nCounter = false;										//	���Z�������肩�Ȃ���
	////	�g���Ă��Ȃ��������̊J��
	//for (int nCount = 0; nCount < MAX_EFFECTTYPE; nCount++)
	//{//	�e�N�X�`���̐�����
	//	if (m_pTexture[nCount] != NULL)
	//	{//	�e�N�X�`���̔j��
	//		m_pTexture[nCount]->Release();
	//		m_pTexture[nCount] = NULL;
	//	}
	//}
}

//=============================================================================
// �G�t�F�N�g�̏��ǂݍ���(�f�[�^�ǂݍ���)
//=============================================================================
void CEffect::LoadDataEffect(void)
{
	FILE *pFile = NULL;
	//	�e�L�X�g�̕�����ǂݍ���
	char read[128];
	//	�e�L�X�g�̓ǂݍ���
	pFile = fopen(&m_cTextName[0], "r");

	if (pFile != NULL)
	{//	�t�@�C���̓ǂݍ��݂��ݒ�o������
	 //	�e�L�X�g�̃f�[�^�ǂݍ���
		fscanf(pFile, "%s", &read[0]);

		while (read[0] != NULL &&strcmp(&read[0], "EFFECTSET_STATE") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{//	�G�t�F�N�g�̓ǂݍ���
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POSRANGE") == 0)
				{//	��]�̐ݒ�
					fscanf(pFile, "%s%f%f%f", &read[0], &m_EffectState.posRange[m_EffectState.nCounter].x, &m_EffectState.posRange[m_EffectState.nCounter].y, &m_EffectState.posRange[m_EffectState.nCounter].z);
				}
				else if (strcmp(&read[0], "ROT") == 0)
				{//	��]�̐ݒ�
					fscanf(pFile, "%s%f%f%f", &read[0], &m_EffectState.rot[m_EffectState.nCounter].x, &m_EffectState.rot[m_EffectState.nCounter].y, &m_EffectState.rot[m_EffectState.nCounter].z);
				}
				else if (strcmp(&read[0], "MOVE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f", &read[0], &m_EffectState.move[m_EffectState.nCounter].x, &m_EffectState.move[m_EffectState.nCounter].y, &m_EffectState.move[m_EffectState.nCounter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f", &read[0], &m_EffectState.size[m_EffectState.nCounter].x, &m_EffectState.size[m_EffectState.nCounter].y, &m_EffectState.size[m_EffectState.nCounter].z);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	�F�̐ݒ�
					fscanf(pFile, "%s%f%f%f%f", &read[0], &m_EffectState.col[m_EffectState.nCounter].r, &m_EffectState.col[m_EffectState.nCounter].g, &m_EffectState.col[m_EffectState.nCounter].b, &m_EffectState.col[m_EffectState.nCounter].a);
				}
				else if (strcmp(&read[0], "TEXUV") == 0)
				{//	UV�̐ݒ�
					fscanf(pFile, "%s%f%f", &read[0], &m_EffectState.TexUV[m_EffectState.nCounter].x, &m_EffectState.TexUV[m_EffectState.nCounter].y);
				}
				else if (strcmp(&read[0], "PRIGONMAX") == 0)
				{//	�|���S���̐��̐ݒ�
					fscanf(pFile, "%s%d", &read[0], &m_EffectState.nSetPoriMAX[m_EffectState.nCounter]);
				}
				else if (strcmp(&read[0], "GRAVITY") == 0)
				{//	�G�t�F�N�g�������鎞�Ԃ̐ݒ�
					fscanf(pFile, "%s%f", &read[0], &m_EffectState.fGravity[m_EffectState.nCounter]);
				}
				else if (strcmp(&read[0], "LIFE") == 0)
				{//	�G�t�F�N�g�������鎞�Ԃ̐ݒ�
					fscanf(pFile, "%s%d", &read[0], &m_EffectState.nLife[m_EffectState.nCounter]);
				}
				else if (strcmp(&read[0], "BILLTYPE") == 0)
				{//	�G�t�F�N�g���Z�����̐ݒ�
					fscanf(pFile, "%s%d", &read[0], &m_EffectState.nBillType[m_EffectState.nCounter]);
				}
			} while (strcmp(&read[0], "EFFECT_END") != 0);
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
			m_EffectState.nCounter++;					//	�J�E���g
		}
		fclose(pFile);
	}
}


//=============================================================================
// �G�t�F�N�g�̏��ǂݍ���(�e�L�X�g�̖��O�ǂݍ���)
//=============================================================================
void CEffect::LoadNameEffect(void)
{
	FILE *pFile = NULL;
	int nNumTxst = 0;	//	�ǂݍ��ރe�L�X�g�̐�
	int nCount = 0;		//
	char *pStrCur;		//	������̐擪�ւ̃|�C���^
	char aLine[256];	//	������̓ǂݍ��ݗp[1�s��]
	char aStr[256];		//	������̔����o���p
	char *pCheck;
	char *pStrLen;

	pFile = fopen("data/TEXT/EFFECT/EFFECT_SET_TEXT.txt", "r");

	if (pFile != NULL)
	{//	�t�@�C���̓ǂݍ��݂��ݒ�o������
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1�s�����ǂݍ���
			pStrCur = aLine;			//	�擪�A�h���X��ǂݍ���
			strcpy(aStr, pStrCur);		//	�擪�A�h���X�𔲂��o��
			pStrLen = "NUM_TEXT = ";
			if (memcmp(pStrCur, "NUM_TEXT = ", strlen(pStrLen)) == 0)
			{//	���f���̔ԍ�
				pStrCur += strlen(pStrLen);			//	������̂Ƃ���܂ŃA�h���X�����炷
				strcpy(aStr, pStrCur);				//	���炵���Ƃ���܂ł̕������������
				nNumTxst = atoi(aStr);				//	�������int�^�ɕϊ�����
				break;
			}
		}
		nCount = 0;
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1�s�����ǂݍ���
		 //	���s���������邩�Ȃ���
			pCheck = strchr(aLine, '\n');
			if (pCheck != NULL)
			{//	���s�������������ꍇ
				*pCheck = '\0';
			}//	���s�������I�[�����ɓ���ւ���
			pStrCur = aLine;			//	�擪�A�h���X��ǂݍ���
			strcpy(aStr, pStrCur);		//	�擪�A�h���X�𔲂��o��
			pStrLen = "TEXT_NAME = ";
			if (memcmp(pStrCur, "TEXT_NAME = ", strlen(pStrLen)) == 0)
			{//	���f���̖��O
				pStrCur += strlen(pStrLen);
				strcpy(&m_cTextName[0], pStrCur);
				if (nCount > nNumTxst)
				{
					break;
				}
			}
			nCount += 1;
		}
		fclose(pFile);
	}
}

//=============================================================================
// �G�t�F�N�g�̏��ǂݍ���(�G�t�F�N�g�̉摜�̖��O�ǂݍ���)
//=============================================================================
void CEffect::LoadTexNameEffect(void)
{
	FILE *pFile = NULL;
	int nNumTex = 0;	//	�ǂݍ��ރe�N�X�`���̐�
	int nCount = 0;
	char *pStrCur;		//	������̐擪�ւ̃|�C���^
	char aLine[256];	//	������̓ǂݍ��ݗp[1�s��]
	char aStr[256];		//	������̔����o���p
	char *pCheck;		//	������̒��̃S�~check
	char *pStrLen;		//	������̒���

	pFile = fopen("data/TEXT/EFFECT/EFFECT_SET_TEXTURE.txt", "r");

	if (pFile != NULL)
	{//	�t�@�C���̓ǂݍ��݂��ݒ�o������
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1�s�����ǂݍ���
			pStrCur = aLine;			//	�擪�A�h���X��ǂݍ���
			strcpy(aStr, pStrCur);		//	�擪�A�h���X�𔲂��o��
			pStrLen = "MAX_TEXTURE = ";
			if (memcmp(pStrCur, "MAX_TEXTURE = ", strlen(pStrLen)) == 0)
			{//	���f���̔ԍ�
				pStrCur += strlen(pStrLen);			//	������̂Ƃ���܂ŃA�h���X�����炷
				strcpy(aStr, pStrCur);				//	���炵���Ƃ���܂ł̕������������
				nNumTex = atoi(aStr);				//	�������int�^�ɕϊ�����
				break;
			}
		}
		m_nTexttureType = nNumTex;
		nCount = 0;
		while (fgets(aLine, 256, pFile) != NULL)
		{//	1�s�����ǂݍ���
		 //	���s���������邩�Ȃ���
			pCheck = strchr(aLine, '\n');
			if (pCheck != NULL)
			{//	���s�������������ꍇ
				*pCheck = '\0';
			}//	���s�������I�[�����ɓ���ւ���
			pStrCur = aLine;			//	�擪�A�h���X��ǂݍ���
			strcpy(aStr, pStrCur);		//	�擪�A�h���X�𔲂��o��
			pStrLen = "TEXTURE_NAME = ";
			if (memcmp(pStrCur, "TEXTURE_NAME = ", strlen(pStrLen)) == 0)
			{//	���f���̖��O
				pStrCur += strlen(pStrLen);
				strcpy(&m_cTextureName[nCount][0], pStrCur);
				if (nCount > nNumTex)
				{
					break;
				}
			}
			nCount += 1;
		}
		fclose(pFile);
	}

}