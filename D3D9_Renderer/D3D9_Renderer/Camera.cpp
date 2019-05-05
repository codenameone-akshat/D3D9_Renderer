#include "Camera.h"

namespace renderer
{
	Camera::Camera()
		:m_eye(0.0f, 0.0f, 0.0f),
		m_lookAt(0.0f, 0.0f, 0.0f),
		m_up(0.0f, 0.0f, 0.0f)
	{
		D3DXMatrixIdentity(&m_perspectiveCam);
	}

	Camera::~Camera()
	{
	}
	void Camera::SetViewMatrix(D3DXVECTOR3 eye, D3DXVECTOR3 lookAt, D3DXVECTOR3 up)
	{
		m_eye = eye;
		m_lookAt = lookAt;
		m_up = up;
		D3DXMatrixLookAtLH(&m_perspectiveCam, &eye, &lookAt, &up);
	}
}