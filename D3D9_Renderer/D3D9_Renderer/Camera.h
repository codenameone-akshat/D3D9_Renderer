#pragma once

#include "BaseObject.h"
namespace renderer
{
	class Camera
		:public BaseObject
	{
		using super = BaseObject;
	public:
		Camera();
		~Camera();

		void SetViewMatrix(DirectX::XMVECTOR eye, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR up);
		
		inline DirectX::XMMATRIX GetViewMatrix() const
		{
			return m_perspectiveCam;
		}
	private:
		DirectX::XMVECTOR m_eye;
		DirectX::XMVECTOR m_lookAt;
		DirectX::XMVECTOR m_up;
		DirectX::XMMATRIX m_perspectiveCam;
	};
}