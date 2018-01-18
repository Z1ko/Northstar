#include "platform.hpp"

#include "../core/kernel.hpp"
#include "win32.hpp"

namespace ns
{
	window::window()
		: _handle(nullptr), _width(0), _height(0)
	{
	}

	window::window(void* handle)
		: _handle(handle), _width(0), _height(0)
	{
		RECT rect = { 0 };
		GetClientRect((HWND)_handle, &rect);

		_width  = rect.right - rect.left;
		_height = rect.bottom - rect.top;
	}


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
			log::get("engine")->error("Errore creazione constesto Win32");
		}

		//VALORI STANDARD
		u32 width = 1600, height = 900;
		string title = "Northstar";

		//Cerca nelle opzioni se è stata specificata una finestra
		json_value wnd_settings = _kernel->settings().find("window");
		if (wnd_settings) {
			for (auto& val : wnd_settings.values()) {
				if (val.first == "width"  && val.second.is<double>())
					width  = (u32)val.second.get<double>();
				else
				if (val.first == "height" && val.second.is<double>())
					height = (u32)val.second.get<double>();
				else
				if (val.first == "title"  && val.second.is<string>())
					title = val.second.get<string>();
			}
		}


		RECT size = { 0, 0, width, height };
		AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		u32 w = size.right - size.left;
		u32 h = size.bottom - size.top;

		_wnd_handle = CreateWindow("NORTHSTAR", title.c_str(),
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