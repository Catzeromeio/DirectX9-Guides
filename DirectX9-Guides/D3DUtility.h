#pragma once
#include <d3dx9.h>

namespace d3d
{
	IDirect3D9 * _d3d9;
	IDirect3DDevice9 * _device;

	int ScreenHight = 800;
	int ScreenWeight = 800;

	IDirect3DVertexBuffer9 * Triangle = 0;
	D3DXMATRIX World;

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

	struct ColorVertex
	{
		ColorVertex() {}

		ColorVertex(float x, float y, float z, D3DCOLOR color)
		{
			_x = x;
			_y = y;
			_z = z;
			_color = color;
		}

		float _x, _y, _z;
		D3DCOLOR _color;
		static const DWORD FVF;
	};

	const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
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
		_device->CreateVertexBuffer(3*sizeof(ColorVertex),D3DUSAGE_WRITEONLY,ColorVertex::FVF,D3DPOOL_MANAGED,&Triangle,0);

		ColorVertex* v;
		Triangle->Lock(0,0,(void **)&v,0);
		v[0] = ColorVertex(-1.0f,0.0f,2.0f,D3DCOLOR_XRGB(255,0,0));
		v[1] = ColorVertex(0.0f,1.0f,2.0f,D3DCOLOR_XRGB(0,255,0));
		v[2] = ColorVertex(1.0f,0.0f,2.0f,D3DCOLOR_XRGB(0,0,255));
		Triangle->Unlock();

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI * 0.5f,(float)ScreenWeight/(float)ScreenHight,1.0f,1000.0f);
		_device->SetTransform(D3DTS_PROJECTION, &proj);

		_device->SetRenderState(D3DRS_LIGHTING,false);
	
		return true;
	}

	bool Display(float timeDelta)
	{

		if (_device)
		{
			_device->Clear(0,0,D3DCLEAR_TARGET,0xffffffff,1.0f,0);
			_device->BeginScene();
			_device->SetFVF(ColorVertex::FVF);
			_device->SetStreamSource(0,Triangle,0,sizeof(ColorVertex));

			D3DXMatrixTranslation(&World, -1.25f, 0, 0);
			_device->SetTransform(D3DTS_WORLD,&World);
			_device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);
			_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);

			D3DXMatrixTranslation(&World, 1.25f, 0, 0);
			_device->SetTransform(D3DTS_WORLD,&World);
			_device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
			_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);

			_device->EndScene();
			_device->Present(0,0,0,0);
			return true;
		}
		return false;
	}

	void CleanUp()
	{
		Triangle->Release();
	}
}

