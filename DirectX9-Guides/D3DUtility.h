#pragma once
#include <d3dx9.h>

namespace d3d
{
	IDirect3D9 * _d3d9;
	IDirect3DDevice9 * _device;

	int ScreenHight = 600;
	int ScreenWeight = 800;

	IDirect3DVertexBuffer9 * VB = 0;
	IDirect3DIndexBuffer9 * IB = 0;

	struct Vertex
	{
		Vertex() {}

		Vertex(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}

		float _x, _y, _z;
		static const DWORD FVF;
	};

	const DWORD Vertex::FVF = D3DFVF_XYZ;

	bool InitD3D(HWND hwnd)
	{
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		D3DCAPS9 caps;
		_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		int count = _d3d9->GetAdapterCount();

		int vp = 0;
		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		D3DPRESENT_PARAMETERS d3dpp;

		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.BackBufferWidth = ScreenWeight;
		d3dpp.BackBufferHeight = ScreenHight;
		//d3dpp.BackBufferFormat = D3DFMT_X8B8G8R8;
		d3dpp.BackBufferCount = 1;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = true;
		d3dpp.EnableAutoDepthStencil = false;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		HRESULT hr = _d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hwnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&_device
		);


		if (FAILED(hr))
		{
			const wchar_t cap[] = L"error";

			switch (hr)
			{
			case D3DERR_DEVICELOST:
			{
				const wchar_t contents[] = L"DEVICELOST";
				::MessageBox(hwnd, contents, cap, 0);
				break;
			}
			case D3DERR_INVALIDCALL:
			{
				const wchar_t contents[] = L"INVALIDCALL";
				::MessageBox(hwnd, contents, cap, 0);
				break;
			}
			case D3DERR_NOTAVAILABLE:
			{
				const wchar_t contents[] = L"NOTAVAILABLE";
				::MessageBox(hwnd, contents, cap, 0);
				break;
			}
			case D3DERR_OUTOFVIDEOMEMORY:
			{
				const wchar_t contents[] = L"OUTOFVIDEOMEMORY";
				::MessageBox(hwnd, contents, cap, 0);
				break;
			}
			default:
			{
				const wchar_t contents[] = L"create device failed";
				::MessageBox(hwnd, contents, cap, 0);
				break;
			}
			}

			return 0;
		}

		return true;
	}

	bool SetUp()
	{
		_device->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &VB, 0);
		_device->CreateIndexBuffer(36 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &IB, 0);

		//fill data
		Vertex* vertices = 0;
		VB->Lock(0, 0, (void **)&vertices, 0);
		//vetertices of  unit cube
		vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
		vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
		vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
		vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
		vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
		vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
		vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
		vertices[7] = Vertex(1.0f, -1.0f, 1.0f);
		VB->Unlock();

		//define triangles of the cube
		WORD* indices = 0;
		IB->Lock(0, 0, (void **)&indices, 0);
		//front
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;
		//back
		indices[6] = 4; indices[7] = 6; indices[8] = 5;
		indices[9] = 4; indices[10] = 7; indices[11] = 6;
		//left
		indices[12] = 4; indices[13] = 5; indices[14] = 1;
		indices[15] = 4; indices[16] = 1; indices[17] = 0;
		//right
		indices[18] = 3; indices[19] = 2; indices[20] = 6;
		indices[21] = 3; indices[22] = 6; indices[23] = 7;
		//top
		indices[24] = 1; indices[25] = 5; indices[26] = 6;
		indices[27] = 1; indices[28] = 6; indices[29] = 2;
		//botton
		indices[30] = 4; indices[31] = 0; indices[32] = 3;
		indices[33] = 4; indices[34] = 3; indices[35] = 7;
		IB->Unlock();

		//position and aim camera
		D3DXVECTOR3 position(0, 0, -5.0f);
		D3DXVECTOR3 target(0, 0, 0);
		D3DXVECTOR3 up(0, 1.0f, 0);

		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		_device->SetTransform(D3DTS_VIEW, &V);

		//set projection matrix
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)ScreenWeight / (float)ScreenHight, 1, 1000.0f);
		_device->SetTransform(D3DTS_PROJECTION, &proj);

		//set render model
		_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		return true;
	}

	bool Display(float timeDelta)
	{
		if (_device)
		{
			//spin the cube
			D3DXMATRIX Rx, Ry;
			static float y = 0.0f;

			D3DXMatrixRotationX(&Rx, y);
			D3DXMatrixRotationY(&Ry, y);
			y += timeDelta;
			if (y >= 2 * D3DX_PI)
				y = 0;

			D3DXMATRIX R = Rx * Ry;

			_device->SetTransform(D3DTS_WORLD, &R);

			//draw the scene
			_device->Clear(0, 0, D3DCLEAR_TARGET, 0xffffff, 1.0f, 0);
			_device->BeginScene();
			_device->SetStreamSource(0, VB, 0, sizeof(Vertex));
			_device->SetIndices(IB);
			_device->SetFVF(Vertex::FVF);
			_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

			_device->EndScene();
			_device->Present(0, 0, 0, 0);

			return true;
		}
		return false;
	}

	void CleanUp()
	{
		VB->Release();
		IB->Release();
	}
}

