#include "Window.h"
#include <string>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	//윈도우 클래스 등록
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	std::string str = GetName();
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const wchar_t* widecstr = widestr.c_str();
	wc.lpszClassName = widecstr;
	wc.hIcon = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	std::string str = wndClassName;
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const wchar_t* widecstr = widestr.c_str();
	UnregisterClass(widecstr, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


Window::Window(int width, int height, const char* name) noexcept : width(width), height(height)
{
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	//rect는 Clinet Region임. 이를 윈도우 크기로 조정(Adjust)
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	
	std::string str = name;
	std::wstring widestr = std::wstring(str.begin(), str.end());
	str = WindowClass::GetName();
	std::wstring widestr2 = std::wstring(str.begin(), str.end());
	hWnd = CreateWindow(
		widestr2.c_str(), widestr.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
