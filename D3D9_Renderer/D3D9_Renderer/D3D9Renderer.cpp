#include <cassert>

#include "D3D9Renderer.h"
#include "ComHelpers.h"
#include <iostream>

namespace renderer
{
	D3D9Renderer::D3D9Renderer()
		:m_d3d9(Direct3DCreate9(D3D_SDK_VERSION)),
		m_device(std::make_unique<D3D9Device>()),
		m_modelList(),
		m_hWindow(),
		m_vBuffer(),
		m_iBuffer(),
		m_vertexDeclarations(),
		m_camera(),
		m_viewMat(DirectX::XMMatrixIdentity()),
		m_projMat(DirectX::XMMatrixIdentity()),
		m_worldMat(DirectX::XMMatrixIdentity()),
		m_vBufferVertexCount(0),
		m_iBufferIndexCount(0),
		m_primitiveCount(0)
	{
	}
	D3D9Renderer::~D3D9Renderer()
	{
		ComSafeRelease(m_d3d9);
	}
	void D3D9Renderer::Init(HWND hWindow)
	{
		memcpy(&m_hWindow, &hWindow, sizeof(HWND));
		SetupDeviceConfiguration();
		PrepareForRendering();
	}
	void D3D9Renderer::UnInit()
	{
		ComSafeRelease(m_d3d9);
		ComSafeRelease(m_vertexDeclarations.positionVertexDecl);
	}
	void D3D9Renderer::PrepareForRendering()
	{
		BuildMatrices();
		ParseModels();
		SetupVertexDeclaration();
		SetupStaticBuffers();
	}
	void D3D9Renderer::PreRender()
	{
		UpdateMatrices();

		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		HRESULT result = CheckDeviceStatus();
		if (result != S_OK)
			return;
		m_device->BeginScene();
		m_device->SetFVF(FVF);
	}
	void D3D9Renderer::RenderFrame()
	{
        m_device->SetRenderStates();
		m_device->SetStreamSource(0, m_vBuffer, 0, sizeof(PositionVertex));
		m_device->SetIndices(m_iBuffer);
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vBufferVertexCount, 0, m_primitiveCount);
	}
	void D3D9Renderer::PostRender()
	{
		m_device->EndScene();
		m_device->Present(nullptr, nullptr, nullptr, nullptr);
	}
	HRESULT D3D9Renderer::CheckDeviceStatus()
	{
		//check every-frame for lost devices
		HRESULT result = m_device->CheckCoorparativeLevel();

		if (result == D3DERR_DEVICELOST)
		{
			OnDeviceLost();
			return E_FAIL;
		}
		else if (result == D3DERR_DRIVERINTERNALERROR)
		{
			MessageBox(nullptr, L"Internal Driver Error... Quitting Program.", nullptr, NULL);
			return E_UNEXPECTED;
		}
		else if (result == D3DERR_DEVICENOTRESET)
		{
			OnDeviceAvailable();
			return E_FAIL;
		}
		else
			return S_OK;

		return E_NOTIMPL;
	}
	//>Present Params Setup
	void D3D9Renderer::SetupDeviceConfiguration()
	{
		D3DPRESENT_PARAMETERS params;
		ZeroMemory(&params, sizeof(params));

		params.BackBufferWidth = SCREEN_WIDTH;
		params.BackBufferHeight = SCREEN_HEIGHT;
		params.BackBufferFormat = D3DFMT_X8R8G8B8;
		params.BackBufferCount = 1;

		DWORD quality;
		//Multi-Sampling sanity check
		if (CheckMultiSampleSupport(D3DMULTISAMPLE_8_SAMPLES, &quality, true) == S_OK)
		{
			params.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
			params.MultiSampleQuality = 0; //quality 1 not supported
			params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		}
		else
		{
			params.MultiSampleType = D3DMULTISAMPLE_NONE;
			params.MultiSampleQuality = 0;
			params.SwapEffect = D3DSWAPEFFECT_DISCARD; //just in case we want something else when Multi-Sampling is off.
		}

		params.hDeviceWindow = m_hWindow;
		params.Windowed = true;
		params.EnableAutoDepthStencil = true; //for now let's just renderer take care of this
		params.AutoDepthStencilFormat = D3DFMT_D24S8;
		params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		params.Flags = NULL;
		params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		m_device->SetDeviceCharateristics(params);

		auto result = CreateD3DDevice(&params);
		assert(result == S_OK);
	}
	void D3D9Renderer::SetupVertexDeclaration()
	{
		constexpr int defaultVal = 0;

		D3DVERTEXELEMENT9 positionVertexElements[] =
		{
			{ defaultVal, defaultVal, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{ defaultVal, defaultVal, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			D3DDECL_END()
		};

		auto result = m_device->CreateVertexDeclaration(positionVertexElements, &m_vertexDeclarations.positionVertexDecl);
		assert(result == S_OK);
		assert(m_vertexDeclarations.positionVertexDecl);

		m_device->SetVertexDeclaration(m_vertexDeclarations.positionVertexDecl);
	}
	void D3D9Renderer::BuildMatrices()
	{
		//>View Matrix
		DirectX::XMVECTOR eye(DirectX::XMVectorSet(0.0f, 5.0f, -10.0f, 1.0f));
		DirectX::XMVECTOR lookAt(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
		DirectX::XMVECTOR up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		
		m_camera.SetViewMatrix(eye, lookAt, up);
		m_viewMat = m_camera.GetViewMatrix();

		//>Projection Matrices
		m_projMat = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(45),
			static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
			1.0f, 1000.0f);

		m_worldMat = DirectX::XMMatrixIdentity();

		m_device->SetTransform(D3DTS_VIEW, m_viewMat);
		m_device->SetTransform(D3DTS_PROJECTION, m_projMat);
		m_device->SetTransform(D3DTS_WORLD, m_worldMat);
	}
	void D3D9Renderer::UpdateMatrices()
	{
		m_device->SetTransform(D3DTS_VIEW, m_viewMat);
		m_device->SetTransform(D3DTS_PROJECTION, m_projMat);

		static FLOAT rad = 0.0f;
		DirectX::XMMATRIX matRotateY;
		matRotateY = DirectX::XMMatrixIdentity();
		matRotateY= DirectX::XMMatrixRotationY(rad);
		rad += 0.02f; //increment rotating triangle

		m_device->SetTransform(D3DTS_WORLD, (matRotateY));

		//update camera if input
	}
	void D3D9Renderer::OnDeviceLost()
	{
		Sleep(200);
	}
	void D3D9Renderer::OnDeviceAvailable()
	{
		m_device->ResetDevice();
	}
	//>Handle this in the mainloop of the game, define "FULLSCREEN" for fullscreen support in the game
	DWORD D3D9Renderer::GetSupportedFeaturesBehavioralFlags() const
	{
		D3DDISPLAYMODE displayMode;
		m_d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		HRESULT HALResult = E_NOTIMPL;

		//Checking HAL support
#ifdef FULLSCREEN
		HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
#else
		HALResult = m_d3d9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, displayMode.Format, true);
#endif
		assert(HALResult == S_OK);

		//Checking for HW Vertex Processing
		D3DCAPS9 d3dCaps;
		m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);

		DWORD flags = 0;
		if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		return flags;
	}
	HRESULT D3D9Renderer::CheckMultiSampleSupport(const D3DMULTISAMPLE_TYPE type, DWORD * quality, const bool isWindowed) const
	{
		HRESULT result = m_d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_D24S8, isWindowed, type, quality);
		return result;
	}
	HRESULT D3D9Renderer::CreateD3DDevice(D3DPRESENT_PARAMETERS * d3dpp)
	{
		assert(m_hWindow != nullptr);
		HRESULT result = m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_hWindow,
			GetSupportedFeaturesBehavioralFlags(),
			d3dpp,
			m_device->GetDeviceObjectRef());
		return result;
	}
	void D3D9Renderer::ParseModels()
	{
		std::string filename = "Z:/Projects/D3D9_Renderer/D3D9_Renderer/D3D9_Renderer/data/cube2.FBX"; //get files to load from somewhere else
		std::unique_ptr<Model> model = std::make_unique<Model>();

		model->LoadModelAndParseData(filename);

		m_modelList.push_back(std::move(model));
	}
	void D3D9Renderer::SetupStaticBuffers()
	{
		int vBufferVertexCount = 0;
		int iBufferIndexCount = 0;
		int primitiveCount = 0;
		
		std::vector<PositionVertex> positionVertices;
		std::vector<int> positionIndices;
		
		for (auto& model : m_modelList)
		{
			vBufferVertexCount += model->GetTotalVertices();
			iBufferIndexCount += model->GetTotalIndices();
			primitiveCount += model->GetNumTris();

			auto meshList = model->GetMeshes();

			for (auto& mesh : meshList)
			{
				auto meshVertices = mesh->GetVertices();
				auto meshNormals = mesh->GetNormals();
				for (auto vitr = meshVertices.begin(), nitr = meshNormals.begin(); vitr != meshVertices.end() && nitr != meshNormals.end(); vitr+=3, nitr += 3)
				{
					positionVertices.push_back({ *vitr, *(vitr + 1), *(vitr + 2), *nitr, *(nitr + 1), *(nitr + 2) });
				}

				auto meshIndices = mesh->GetIndices();
				for (auto index : meshIndices)
					positionIndices.push_back(index);
			}
		}  

		m_vBufferVertexCount = vBufferVertexCount;
        m_iBufferIndexCount = iBufferIndexCount;
        m_primitiveCount = primitiveCount;

        auto result = m_device->CreateVertexBuffer((sizeof(PositionVertex) * vBufferVertexCount), NULL, FVF, D3DPOOL_MANAGED, m_vBuffer, nullptr);
        assert(result == S_OK);

        result = m_device->CreateIndexBuffer(m_iBufferIndexCount, NULL, D3DFMT_INDEX32, D3DPOOL_DEFAULT, m_iBuffer, nullptr);
        assert(result == S_OK);

		m_vBuffer.AddDataToBuffer(positionVertices.data(), NULL, sizeof(PositionVertex)* vBufferVertexCount);
		m_iBuffer.AddDataToBuffer(positionIndices.data(), NULL, sizeof(float) * iBufferIndexCount);
	}
}