#include <windows.h>

#include "Camera.h"
#include "../utils/Logger.h"

namespace renderer
{
	Camera::Camera()
		:m_camPos	(0.0f, 1.0f, 0.0f),
		m_camRot	(0.0f, 0.0f)
	{
		D3DXMatrixIdentity(&m_viewMatrix);
	}

	Camera::~Camera()
	{
	}

	void Camera::HandleCameraInput()
	{
		D3DXMATRIX rotationMat;
        D3DXMatrixIdentity(&rotationMat);
        D3DXVECTOR3 forward = { 0.0f, 0.0f, 1.0f };
        D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
        D3DXVECTOR3 right = { 1.0f, 0.0f, 0.0f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		auto dx = cursorPos.x - m_cursorPos.x;
		auto dy = cursorPos.y - m_cursorPos.y;
		if (GetKeyState(VK_RBUTTON) & (1 << 15))
		{
			m_camRot.x += dx;
			m_camRot.y += dy;
		}
		m_cursorPos = cursorPos;
		D3DXMatrixRotationYawPitchRoll(&rotationMat, D3DXToRadian(m_camRot.x / 10.0f), D3DXToRadian(m_camRot.y / 15.0f), 0.0f);
		
        D3DXVec3TransformNormal(&forward, &forward, &rotationMat);
		D3DXVec3TransformNormal(&right, &right, &rotationMat);
        D3DXVec3Normalize(&forward, &forward);
		D3DXVec3Normalize(&right, &right);


        float multiplier(1.0f);
		if (GetKeyState(VK_SHIFT) & (1 << 15))
			multiplier = 10.0f;
		if (GetKeyState('W') & (1 << 15))
			m_camPos += (forward * multiplier);
		if (GetKeyState('S') & (1 << 15))
			m_camPos -= (forward * multiplier);
		if (GetKeyState('A') & (1 << 15))
			m_camPos -= (right * multiplier);
		if (GetKeyState('D') & (1 << 15))
			m_camPos += (right * multiplier);
		if (GetKeyState('Q') & (1 << 15))
			m_camPos -= (up * multiplier);
		if (GetKeyState('E') & (1 << 15))
			m_camPos += (up * multiplier);

		D3DXMATRIX translationMat;
		D3DXMatrixIdentity(&translationMat);
		D3DXMatrixTranslation(&translationMat, m_camPos.x, m_camPos.y, m_camPos.z);


		m_viewMatrix = (rotationMat * translationMat);
		float viewDeterminent = D3DXMatrixDeterminant(&m_viewMatrix);
		D3DXMatrixInverse(&m_viewMatrix, &viewDeterminent, &m_viewMatrix);
	}
}