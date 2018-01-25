#pragma once
#include <d3dx9.h>

namespace d3d
{
	IDirect3D9 * _d3d9;
	IDirect3DDevice9 * _device;

	bool InitD3D(HWND hwnd)
	{
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		D3DCAPS9 caps;
		_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

		int vp = 0;
		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.BackBufferWidth = 800;
		d3dpp.BackBufferHeight = 600;
		d3dpp.BackBufferFormat = D3DFMT_A8B8G8R8;
		d3dpp.BackBufferCount = 1;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = false;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24FS8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		HRESULT hr = _d3d9->CreateDevice(
			0,
			D3DDEVTYPE_HAL,
			hwnd,
			vp,
			&d3dpp,
			&_device
		);

		if (FAILED(hr))
		{
			const wchar_t contents[] = L"create device failed";
			const wchar_t cap[] = L"error";
			::MessageBox(hwnd, contents, cap, 0);
			return 0;
		}
	}

	bool Display()
	{
		if (_device)
		{
			_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
			_device->Present(0, 0, 0, 0);

			return true;
		}

		return false;
	}
}

