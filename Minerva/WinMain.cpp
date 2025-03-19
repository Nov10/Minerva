#include<Windows.h>
#include <string>
#include <sstream>
#include "Engine/EngineController.h"
#include "Window.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "Engine/File/GraphicsFile/MeshReader.h"

GPUController* GController;
EngineController engine = EngineController(1920, 1080);

//Camera MainCamera(Vector3(0, 0, -135), Vector3(0, 0, 1), 60, 800, 800);
//Camera MainCamera(Vector3(0, 0, -5), Vector3(0, 0, 1), 60, 800, 800);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//WM : Window Message
	switch (msg)
	{
		case WM_CREATE :
		SetTimer(hWnd, 1, 16, NULL);  // 16ms(60FPS) 간격으로 WM_TIMER 발생
		break;
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"Respects");
		}
		break;
	case WM_CHAR:
	{
		static std::string title;
		title.push_back((char)wParam);
		std::wstring widestr = std::wstring(title.begin(), title.end());
		const wchar_t* widecstr = widestr.c_str();
		SetWindowText(hWnd, widecstr);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss; //문자열 합치는데 쓰이는거 o : output : strings -> string
		oss << "(" << pt.x << "," << pt.y << ")";
		std::string title = oss.str();
		std::wstring widestr = std::wstring(title.begin(), title.end());
		const wchar_t* widecstr = widestr.c_str();
		SetWindowText(hWnd, widecstr);

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//Rend.Render();
		engine.Render();
		auto& result = (*engine.GetRenderedResult());
		auto hBitmap = result.ToHBITMAP(hdc);
		//auto hBitmap = MainCamera.Target.ToHBITMAP(hdc);

		HDC memDC = CreateCompatibleDC(hdc);
		HGDIOBJ oldBitmap = SelectObject(memDC, hBitmap);


		BitBlt(hdc, 50, 50, result.GetWidth(), result.GetHeight(), memDC, 0, 0, SRCCOPY);

		SelectObject(memDC, oldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memDC);
	}
	break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int   nCmdShow)
{
	(*engine.GetRenderedResult()).Clear(MColor(10, 20, 150));
	OpenCLController o = OpenCLController();
	GController = &o;
	GPUControl.Initialize();


	Scene scene = Scene();
	engine.Scenes.emplace_back(&scene);

	Renderer mainRenderer = Renderer();
	mainRenderer.Initialize();
	engine.AddRenderer(&mainRenderer);

	Camera mainCamera = Camera(Vector3(0, 0, -10), Vector3(0, 0, 1), 60, 800, 800);
	mainCamera.IsMainGameCamera = true;
	mainRenderer.AddCamera(&mainCamera);

	auto go = GameObject::CreateGameObject(scene);
	auto mr = go->AddComponent<MeshRenderer>();
	//go->SetActive(false);
	go->SetLocalScale(Vector3(1, 1, 1) * 0.5f);
	RenderData d;
	MeshReader::ReadMesh("C:\Aereo.obj", &d);
	mr.get()->Datas.emplace_back(d);

	auto go2 = GameObject::CreateGameObject(scene);
	go2->SetLocalPosition(Vector3(2, 0, 0));
	auto mr2 = go2->AddComponent<MeshRenderer>();

	RenderData d2;
	d2.AddCubeData();
	mr2.get()->Datas.emplace_back(d2);

	go2->SetLocalScale(Vector3(1, 1, 1) * 0.4f);


	const auto pClassName = L"hw3d";
	//윈도우 클래스 등록
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIcon = nullptr;
	RegisterClassEx(&wc);
	RECT rect;
	rect.left = 0;
	rect.right = 1000 + rect.left;
	rect.top = 0;
	rect.bottom = 1000 + rect.top;
	//rect는 Clinet Region임. 이를 윈도우 크기로 조정(Adjust)
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	//윈도우 인스턴스 생성
	HWND hWnd = CreateWindowEx(
		0, pClassName, L"Happy Hard Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
	//메세지 처리
	MSG msg;
	BOOL gResult;
	Timer::Initialize();
	go->SetLocalRotation(Quaternion::FromEulerAngles(30, -30, 30));

	while (true) {
		Timer::Update();

		go->SetLocalRotation(Quaternion::FromEulerAngles(10 * Timer::GetTotalTime(), 30 * Timer::GetTotalTime(), 10 * Timer::GetTotalTime()));
		go2->SetLocalRotation(Quaternion::FromEulerAngles(10 * Timer::GetTotalTime(), 30 * Timer::GetTotalTime(), 10 * Timer::GetTotalTime()));
		if ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			InvalidateRect(hWnd, nullptr, TRUE); 
			UpdateWindow(hWnd);                 
		}
		else
		{
			break;
		}
	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}