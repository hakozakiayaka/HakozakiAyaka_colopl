//=============================================================================
//
// �J�������� [camera.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================================================================
//	�O���錾
//=============================================================================
class CPlayer;

//*****************************************************************************
// �J�����̃N���X
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		CAMERATYPE_NONE = 0,
		CAMERATYPE_TITLE,
		CAMERATYPE_GAME,
		CAMERATYPE_RANKING,
		CAMERATYPE_MAX

	}CAMERA_MODE;
	CCamera();									//	�R���X�g���N�^
	~CCamera();									//	�f�X�g���N�^
	HRESULT Init(CAMERA_MODE CameraMode);		//	�J�����̏�����
	void Uninit(void);							//	�J�����̏I������
	void Update(void);							//	�J�����̍X�V����
	void SetCamera(void);						//	�J�����̐ݒ菈��
	static D3DXVECTOR3 GetCameraRotate(void);	//	�J�����̉�]�̎擾
private:

	CAMERA_MODE m_CameraType;					// �J�����̎��

	D3DXVECTOR3	m_posV;							// ���݂̎��_
	D3DXVECTOR3	m_posR;							// ���݂̒����_
	D3DXVECTOR3	m_posVDest;						// �ړI�̎��_
	D3DXVECTOR3	m_posRDest;						// �ړI�̒����_
	D3DXVECTOR3	m_vecU;							// ������x�N�g��
	D3DXMATRIX	m_mtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;						// �r���[�}�g���b�N�X
	static D3DXVECTOR3	m_rot;					// ���݂̌����@
	D3DXVECTOR3	m_rotDest;						// �ړI�̌���
	float		m_fLength;						// �����_�Ǝ��_�̋���
	float		m_fCameraSpeed;					// �J�����̓�������
	float		m_fContCamSpeed;				// �J�����̓����������
	float		m_fCameraRotSpeed;				// �J�����̉�]��������
	int			m_nCameraMove[3];				//	�J�����𓮂���
	int			m_nGameCounter;					//	�Q�[���̃J�E���g
	bool		m_bCameraMode;					//	�J�����̃��[�h�ؑ�
	D3DXVECTOR3 m_AddCamera;					//	�J�����̉��Z
};
#endif
