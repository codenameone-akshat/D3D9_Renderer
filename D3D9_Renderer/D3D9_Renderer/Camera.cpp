#include "Camera.h"

namespace renderer
{
	Camera::Camera()
		:m_eye(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
		m_lookAt(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
		m_up(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
		m_perspectiveCam(DirectX::XMMatrixIdentity())
	{
	}

	Camera::~Camera()
	{
	}
	void Camera::SetViewMatrix(DirectX::XMVECTOR eye, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up)
	{
		m_eye = eye;
		m_lookAt = lookAt;
		m_up = up;
		m_perspectiveCam = DirectX::XMMatrixLookAtLH(eye, lookAt, up);
	}
}