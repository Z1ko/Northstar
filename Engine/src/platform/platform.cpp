#include "platform.hpp"

#include "win32.hpp"

namespace ns
{
	//Crea finestra e collega callback
	platform_service::platform_service(kernel* kernel)
		: service(kernel, "platform", NS_CORE_SERVICE_PLATFORM)
	{
	}

	//Callback di windows
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		platform_service* platform = (platform_service*)ptr;

		w32_event_data data;
		if (platform != nullptr) {
			
			data.message = message;
			data.wParam = wParam;
			data.lParam = lParam;

			platform->os_event(&data);
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	//Crea finestra e collega callback
	void platform_service::initialize() {

		HINSTANCE instance = GetModuleHandle(NULL);

		WNDCLASSEX wclass = { 0 };
		wclass.cbSize        = sizeof(WNDCLASSEX);
		wclass.style         = 0;
		wclass.lpfnWndProc   = WndProc;
		wclass.cbClsExtra    = 0;
		wclass.cbWndExtra    = 0;
		wclass.hInstance     = instance;
		wclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wclass.lpszMenuName  = NULL;
		wclass.lpszClassName = "NORTHSTAR";
		wclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		//ERRORE
		if (!RegisterClassEx(&wclass)) {
			printf("\nErrore creazione contexto W32");
		}

		RECT size = { 0, 0, 500, 500 };
		AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		u32 w = size.right - size.left;
		u32 h = size.bottom - size.top;

		_wnd_handle = CreateWindow("NORTHSTAR", "Northstar",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
			w, h, NULL, NULL, instance, NULL);

		SetWindowLongPtr((HWND)_wnd_handle, GWLP_USERDATA, (LONG)this);
	}

	//Ottiene eventi del SO
	void platform_service::update() {
		MSG event;
		while (PeekMessage(&event, (HWND)_wnd_handle, 0, 0, PM_REMOVE)) {
			TranslateMessage(&event); 
			DispatchMessage(&event);
		}
	}
}