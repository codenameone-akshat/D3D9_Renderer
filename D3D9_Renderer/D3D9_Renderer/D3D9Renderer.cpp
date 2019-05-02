#include <cassert>

#include "D3D9Renderer.h"
#include "ComHelpers.h"
#include <iostream>


#include<string>
#include <sstream>
#include <stdlib.h>
#define LogInfo( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << std::endl;                   \
   OutputDebugString(os_.str().c_str());  \
}

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
		m_device->SetStreamSource(0, m_vBuffer, 0, sizeof(PositionVertex) * m_vBufferVertexCount);
		m_device->SetIndices(m_iBuffer);
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vBufferVertexCount, 0, m_primitiveCount);
		//m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
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
		if (CheckMultiSampleSupport(D3DMULTISAMPLE_4_SAMPLES, &quality, true) == S_OK)
		{
			params.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
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
		params.AutoDepthStencilFormat = D3DFMT_D16;
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
			{ defaultVal, defaultVal, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
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
		DirectX::XMVECTOR eye(DirectX::XMVectorSet(0.0f, 10.0f, -100.0f, 1.0f));
		DirectX::XMVECTOR lookAt(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
		DirectX::XMVECTOR up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		
		m_camera.SetViewMatrix(eye, lookAt, up);
		m_viewMat = m_camera.GetViewMatrix();

		//>Projection Matrices
		m_projMat = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(45),
			static_cast<float>(SCREEN_HEIGHT / SCREEN_HEIGHT),
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
		std::string filename = "Z:/Projects/D3D9_Renderer/D3D9_Renderer/D3D9_Renderer/data/Cube.FBX"; //get files to load from somewhere else
		std::unique_ptr<Model> model = std::make_unique<Model>();

		model->LoadModelAndParseData(filename);

		m_modelList.push_back(std::move(model));
	}
	void D3D9Renderer::SetupStaticBuffers()
	{
		int vBufferVertexCount = 0;
		int iBufferIndexCount = 0;
		int primitiveCount = 0;
		
		float* vertices = static_cast<float*>(calloc(0, sizeof(float)));
		int* indices = static_cast<int*>(calloc(0, sizeof(int)));
		
		size_t vOffset = 0;
		size_t iOffset = 0;
		
		for (auto& model : m_modelList)
		{
			vBufferVertexCount += model->GetTotalVertices();
			iBufferIndexCount += model->GetTotalIndices();
			primitiveCount += model->GetNumTris();

			auto meshList = model->GetMeshes();

			for (auto& mesh : meshList)
			{
				auto meshVertices = mesh->GetVertices();
				vertices = static_cast<float*>(realloc(vertices, sizeof(float) * mesh->GetNumVertices()));
				memcpy(vertices + vOffset, meshVertices.data(), (sizeof(float) * mesh->GetNumVertices()));
				vOffset += sizeof(meshVertices);
				
				auto meshIndices = mesh->GetIndices();
				indices = static_cast<int*>(realloc(indices, sizeof(int) * mesh->GetNumIndices()));
				memcpy(indices + iOffset, meshIndices.data(), (sizeof(int) * mesh->GetNumIndices()));
				iOffset += sizeof(meshIndices);
			}
		}  
		//user vertices
		LogInfo(std::to_string(sizeof(float)).c_str());
		LogInfo(std::to_string(sizeof(float*)).c_str());
		for (int i = 0; i < vBufferVertexCount; ++i)
		{
			LogInfo(std::to_string(vertices[i]).c_str());
		}
		auto result = m_device->CreateVertexBuffer(vBufferVertexCount * sizeof(PositionVertex), NULL, FVF, D3DPOOL_MANAGED, m_vBuffer, nullptr);
		assert(result == S_OK);

		result = m_device->CreateIndexBuffer(iBufferIndexCount, NULL, D3DFMT_INDEX32, D3DPOOL_DEFAULT, m_iBuffer, nullptr);
		assert(result == S_OK);

		PositionVertex verticestemp[] =
		{
			{ -30.0f, -30.0f, 30.0f} ,    // side 1
			{ 30.0f, -30.0f, 30.0f },
			{ -30.0f, 30.0f, 30.0f},
			{ 30.0f, 30.0f, 30.0f },

			{ -30.0f, -30.0f, -30.0f},    // side 2
			{ -30.0f, 30.0f, -30.0f},
			{ 30.0f, -30.0f, -30.0f},
			{ 30.0f, 30.0f, -30.0f },

			{ -30.0f, 30.0f, -30.0f},    // side 3
			{ -30.0f, 30.0f, 30.0f},
			{ 30.0f, 30.0f, -30.0f },
			{ 30.0f, 30.0f, 30.0f},

			{ -30.0f, -30.0f, -30.0f},    // side 4
			{ 30.0f, -30.0f, -30.0f},
			{ -30.0f, -30.0f, 30.0f},
			{ 30.0f, -30.0f, 30.0f},

			{ 30.0f, -30.0f, -30.0f },    // side 5
			{ 30.0f, 30.0f, -30.0f},
			{ 30.0f, -30.0f, 30.0f},
			{ 30.0f, 30.0f, 30.0f},

			{ -30.0f, -30.0f, -30.0f},    // side 6
			{ -30.0f, -30.0f, 30.0f},
			{ -30.0f, 30.0f, -30.0f},
			{ -30.0f, 30.0f, 30.0f},
		};
		int indicestemp[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 5, 6,    // side 2
			6, 5, 7,
			8, 9, 10,    // side 3
			10, 9, 11,
			12, 13, 14,    // side 4
			14, 13, 15,
			16, 17, 18,    // side 5
			18, 17, 19,
			20, 21, 22,    // side 6
			22, 21, 23,
		};
		m_vBuffer.AddDataToBuffer(verticestemp, NULL, sizeof(verticestemp));
		m_iBuffer.AddDataToBuffer(indicestemp, NULL, sizeof(indicestemp));
		
		m_vBufferVertexCount = 24;
		m_iBufferIndexCount = 24;
		m_primitiveCount = 12;

		free(vertices);
		free(indices);

	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////HACK///////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	struct VERT
	{
		FLOAT X, Y, Z;
		D3DVECTOR NORMAL;
	};

	void D3D9Renderer::HackInit(HWND hWindow)
	{
		m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		auto window = hWindow;

		D3DPRESENT_PARAMETERS d3dpp;	//struct to hold device info

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = window;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		assert(window != nullptr);
		HRESULT result = m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			window,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&device);
		assert(result == S_OK);

		VERT vertices[] =
		{
			{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },    // side 1
			{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
			{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
			{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },

			{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },    // side 2
			{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
			{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
			{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },

			{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },    // side 3
			{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },
			{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },
			{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },

			{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },    // side 4
			{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },
			{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },
			{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },

			{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },    // side 5
			{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },
			{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },
			{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },

			{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },    // side 6
			{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },
			{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },
			{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },
		};
		int indices[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 5, 6,    // side 2
			6, 5, 7,
			8, 9, 10,    // side 3
			10, 9, 11,
			12, 13, 14,    // side 4
			14, 13, 15,
			16, 17, 18,    // side 5
			18, 17, 19,
			20, 21, 22,    // side 6
			22, 21, 23,
		};
		device->CreateVertexBuffer(sizeof(vertices),
			0,
			FVF,
			D3DPOOL_MANAGED,
			&vBuffer,
			nullptr);

		VOID* v_bufferData;

		vBuffer->Lock(0, 0, static_cast<void**>(&v_bufferData), 0);
		memcpy(v_bufferData, vertices, sizeof(vertices));
		vBuffer->Unlock();

		device->CreateIndexBuffer(sizeof(indices),
			0,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&iBuffer,
			nullptr);

		VOID* i_bufferData;

		iBuffer->Lock(0, 0, static_cast<void**>(&i_bufferData), 0);
		memcpy(i_bufferData, indices, sizeof(indices));
		iBuffer->Unlock();

		D3DLIGHT9 light;
		D3DMATERIAL9 mat;

		ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		light.Direction = { -1.0f, -0.3f, 1.0f };
		device->SetLight(0, &light);
		device->LightEnable(0, TRUE);

		ZeroMemory(&mat, sizeof(mat));
		mat.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		mat.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };

		device->SetMaterial(&mat);


		device->SetRenderState(D3DRS_LIGHTING, TRUE);
		device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		device->SetRenderState(D3DRS_ZENABLE, TRUE); //turning on z buffer
		device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}
	void D3D9Renderer::HackRender()
	{
		device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		device->Clear(0, nullptr, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		device->BeginScene();
		device->SetFVF(FVF);

		//>View Matrix
		DirectX::XMVECTOR eye(DirectX::XMVectorSet(0.0f, 10.0f, -30.0f, 1.0f));
		DirectX::XMVECTOR lookAt(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
		DirectX::XMVECTOR up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		auto matCam = DirectX::XMMatrixLookAtLH(eye, lookAt, up);

		//>Projection Matrices
		auto projMat = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(45),
			static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT),
			1.0f, 100.0f);

		auto worldMat = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 dxcast_matrix;
		DirectX::XMStoreFloat4x4(&dxcast_matrix, matCam);
		device->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&dxcast_matrix));

		DirectX::XMStoreFloat4x4(&dxcast_matrix, projMat);
		device->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&dxcast_matrix));

		DirectX::XMStoreFloat4x4(&dxcast_matrix, worldMat);
		device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&dxcast_matrix));

		device->SetStreamSource(0, vBuffer, 0, sizeof(VERT));
		device->SetIndices(iBuffer);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		device->EndScene();
		device->Present(nullptr, nullptr, nullptr, nullptr);
	}
}