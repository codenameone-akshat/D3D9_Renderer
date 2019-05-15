#include "Camera.h"
#include <windows.h>
namespace renderer
{
	Camera::Camera()
		:m_eye(0.0f, 0.0f, 0.0f),
		m_lookAt(0.0f, 0.0f, 0.0f),
		m_up(0.0f, 1.0f, 0.0f)
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

    void Camera::HandleCameraInput()
    {
        float x(0.0f);
        float y(0.0f);
        float z(0.0f);
        float multiplier(1.0f);
        
        if (GetKeyState(VK_SHIFT) & (1 << 15))
            multiplier = 5.0f;
        if (GetKeyState('W') & (1 << 15))
            z -= 0.5f*multiplier;
        if (GetKeyState('S') & (1 << 15))
            z += 0.5f*multiplier;
        if (GetKeyState('A') & (1 << 15))
            x += 0.5f*multiplier;
        if (GetKeyState('D') & (1 << 15))
            x -= 0.5f*multiplier;
        if (GetKeyState('Q') & (1 << 15))
            y -= 0.5f*multiplier;
        if (GetKeyState('E') & (1 << 15))
            y += 0.5f*multiplier;

        m_perspectiveCam(3, 0) += x;
        m_perspectiveCam(3, 1) += y;
        m_perspectiveCam(3, 2) += z;


        GetCursorPos(&m_cursorPos);

    }
}