#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

typedef float RGBA[4];

void DebugConsole();

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_hwnd = NULL;

bool g_bQuit = false;

ComPtr<ID3D11Device> dev;
ComPtr<ID3D11DeviceContext> con;
ComPtr<IDXGISwapChain> sc;

ComPtr<ID3D11RenderTargetView> rtv;

D3D11_VIEWPORT viewport;

void InitD3D();
void MainLoop();

void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr))
		throw std::exception();
	return;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#ifndef NDEBUG
	DebugConsole();
#endif

	std::cout << "Welcome to CodeVortex debug console" << std::endl;
	
	const char CLASS_NAME[] = "CodeVortex";

	WNDCLASS wc = { };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	g_hwnd = CreateWindowEx(
		NULL,
		CLASS_NAME,
		"CodeVortex DLL Injector",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,

		NULL, NULL, hInstance, NULL
	 );

	if (g_hwnd)
		ShowWindow(g_hwnd, nShowCmd);

	InitD3D();

	MSG msg;
	while (!g_bQuit) {
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		MainLoop();
	}

	return 0;
}

void InitD3D() {
	RECT rect;
	GetWindowRect(g_hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC scDesc = { };
	scDesc.BufferCount = 1;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.Windowed = TRUE;
	scDesc.OutputWindow = g_hwnd;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Width = width;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&scDesc,
		sc.GetAddressOf(),
		dev.GetAddressOf(),
		nullptr,
		con.GetAddressOf()
	));

	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(sc->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	ThrowIfFailed(dev->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf()));

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.MaxDepth = 1.f;
	viewport.Width = width;
	viewport.Height = height;
}

void MainLoop() {
	con->ClearRenderTargetView(rtv.Get(), RGBA { 0.f, 0.f, 0.f, 1.f });
	con->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	con->RSSetViewports(1, &viewport);

	sc->Present(1, 0);
}

void DebugConsole() {
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		g_bQuit = true;
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}