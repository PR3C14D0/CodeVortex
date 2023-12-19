#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d11.h>
#include <dxgi.h>

void DebugConsole();

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_hwnd = NULL;

bool g_bQuit = false;

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

	MSG msg;
	while (!g_bQuit) {
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
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