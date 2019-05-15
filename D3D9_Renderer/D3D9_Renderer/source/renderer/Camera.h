#pragma once

#include "../enginecore/BaseObject.h"

namespace renderer
{
	class Camera
		:public BaseObject
	{
		using super = BaseObject;
	public:
		Camera();
		~Camera();

		inline D3DXMATRIX GetViewMatrix() const	{return m_viewMatrix;}
		inline D3DXVECTOR3 GetCamPosision() const { return m_camPos; }
		inline D3DXVECTOR2 GetCamRotation() const { return m_camRot; }

        void HandleCameraInput();
	private:
		
		D3DXVECTOR3 m_camPos;
		D3DXVECTOR2 m_camRot;
		D3DXVECTOR3 m_forward;
		D3DXVECTOR3 m_right;
		D3DXVECTOR3 m_up;

		D3DXMATRIX m_viewMatrix;

        POINT m_cursorPos;
	};
}