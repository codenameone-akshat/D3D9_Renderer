#pragma once

#include <d3dx9.h>

namespace renderer
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		[[maybe_unused]] inline D3DXMATRIX GetViewMatrix() const { return m_viewMatrix; }
		[[maybe_unused]] inline D3DXVECTOR3 GetCamPosition() const { return m_camPos; }
		[[maybe_unused]] inline D3DXVECTOR2 GetCamRotation() const { return m_camRot; }

        void HandleCameraInput();
	private:
		
		D3DXVECTOR3 m_camPos;
		D3DXVECTOR2 m_camRot;

		D3DXMATRIX m_viewMatrix;

        POINT m_cursorPos;
	};
}