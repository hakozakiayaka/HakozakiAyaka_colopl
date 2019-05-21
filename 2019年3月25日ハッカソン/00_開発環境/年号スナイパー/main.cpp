//=============================================================================
//
// ���C������ [main.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

#include "renderer.h"	// �����_���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�N���X�i�C�p�["			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������[�N���o
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,									// �\������E�B���h�E�̃X�^�C����ݒ�
		WndProc,									// �E�B���h�E�v���V�[�W���̃A�h���X���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		hInstance,									// WinMain�̈����̃C���X�^���X�n���h�����w��
		NULL,										// �^�X�N�o�[�Ɏg�p����A�C�R�����w��
		LoadCursor(NULL, IDC_ARROW),				// �g�p����}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),					// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,										// Windows�ɂ��郁�j���[��ݒ�
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		NULL										// �t�@�C���A�C�R���Ɏg�p����A�C�R�����w��
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };// �N���C�A���g�̈�̐ݒ�
	HWND hWnd;										// �E�B���h�E�n���h���i���ʎq�j
	MSG msg;										// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;							// ���ݎ���
	DWORD dwExecLastTime;							// �Ō�ɏ�����������
	DWORD dwFrameCount;								// FPS�l�J�E���g
	DWORD dwFPSLastTime;							// FPS�l�Ō�ɏ�����������

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,					// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,								// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,						// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E�̍���X���W		/*�d�Ȃ�Ə��������炵�Ă����    */
		CW_USEDEFAULT,								// �E�B���h�E�̍���Y���W		/*�Œ�l���Ɠ����ꏊ�ɕ\�����ꑱ����*/
		(rect.right - rect.left),					// �E�B���h�E�̕�
		(rect.bottom - rect.top),					// �E�B���h�E�̍���
		NULL,										// �e�E�B���h�E�̃n���h��
		NULL,										// ���j���[�n���h���܂��̓E�B���h�EID
		hInstance,									// �C���X�^���X�n���h��
		NULL);										// �E�B���h�E�쐬�f�[�^

	srand((unsigned int)time(0));					// �����_��

	if (pManager == NULL)
	{// NULL�̏ꍇ
		pManager = new CManager;
	}

	if (pManager != NULL)
	{// NULL�ȊO�̏ꍇ
		if (FAILED(pManager->Init(hInstance, hWnd, FALSE)))
		{
			return -1;
		}

		// ����\��ݒ�
		timeBeginPeriod(1);

		// �t���[���J�E���g������
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();				// ���ݎ������擾


		// �E�C���h�E�̕\��
		ShowWindow(hWnd, nCmdShow);						// �E�B���h�E�̕\���i�E�B���h�E�̕\����Ԃ�ݒ�j
		UpdateWindow(hWnd);								// �E�B���h�E�̔��f�i�E�B���h�E�̃N���C�A���g�̈�̍X�V�j

		// ���b�Z�[�W���[�v
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
			{// Windows�̏���
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
					break;
				}
				else
				{
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);				// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
					DispatchMessage(&msg);				// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
				}
			}
			else
			{// DirectX�̏���
				static int nCntFps = 0;
				dwCurrentTime = timeGetTime();		// ���݂̎��Ԃ��擾
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5�b���ƂɎ��s
					nCntFps = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
					dwFrameCount = 0;


				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60�b�o��
					dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();

					//CManager::m_CDebugProcLeft->Print("\nFPS %d", nCntFps);


					dwFrameCount++;
				}
			}
		}
	}

	// �I������
	if (pManager != NULL)
	{// NULL�ȊO�̏ꍇ
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
