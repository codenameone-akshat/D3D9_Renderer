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

		void SetViewMatrix(D3DXVECTOR3 eye, D3DXVECTOR3 lookAt, D3DXVECTOR3 up);
		
		inline D3DXMATRIX GetViewMatrix() const	{return m_perspectiveCam;}

        inline D3DXVECTOR3 GetEye() const { return m_eye; }
        inline D3DXVECTOR3 GetUpDir() const { return m_lookAt; }
	private:
		D3DXVECTOR3 m_eye;
		D3DXVECTOR3 m_lookAt;
		D3DXVECTOR3 m_up;
		D3DXMATRIX m_perspectiveCam;
	};
}