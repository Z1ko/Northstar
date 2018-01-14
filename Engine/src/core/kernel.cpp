#include "kernel.hpp"

#include "../utils/path.hpp"

#include <Windows.h>

namespace ns
{
	//Trova la directory dove è in esecurione l'eseguibile
	static string find_root_directory(const string& argv0)
	{
		u32 lenght = argv0.find_last_of('\\') + 1;
		string root = argv0.substr(0, lenght);

		replace(root.begin(), root.end(), '\\', '/');
		return root;
	}

	//Inizializza tutti i servizi e macro standard, carica impostazioni etc
	kernel::kernel(char argc, char** argv)
	{
		//Cartella dov'è presente l'exe
		path::add_macro("root", find_root_directory(argv[0]));
	}

	//Avvia applicazione
	void kernel::start()
	{
		//Crea contesto W32
		create_w32_context();

		//Inizializza tutti i servizi
		for (service* serv : _services)
			serv->initialize();

		MSG event;
		while(_running)
		{
			//Aggiorna input se necessario
			while(PeekMessage(&event, (HWND)_window_handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&event);
				DispatchMessage(&event);
			}

			//Aggiorna tutti i sistemi in ordine
			for (service* serv : _updateList)
				serv->update();

			//Aggiorna applicazione

			//Aggiorna tutti i sistemi in ordine
			for (service* serv : _renderList)
				serv->render();
		}
	}

	//Evento di windows
	struct w32_event_data
	{
		UINT message;

		WPARAM wParam;
		LPARAM lParam;
	};

	//Callback di windows32
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		kernel* krnl = (kernel*)ptr;

		if (krnl != nullptr)
		{
			w32_event_data data;
			data.message = message;
			data.wParam = wParam;
			data.lParam = lParam;
			krnl->platform_event(&data);
		}
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	//Crea contesto della finestra
	void kernel::create_w32_context()
	{
		//Crea console per debug
		AttachConsole(ATTACH_PARENT_PROCESS);
		AllocConsole();

		SetConsoleTitle("NORTHSTAR32 - CONSOLE");

		freopen("CON", "w", stdout);
		SetConsoleOutputCP(CP_UTF8);

	//===================================================

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

		if (!RegisterClassEx(&wclass)) {
			//ERRORE
			printf("\nErrore creazione contexto W32");
		}

		RECT size = { 0, 0, 500, 500 };
		AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		u32 w = size.right - size.left;
		u32 h = size.bottom - size.top;

		_window_handle = CreateWindow("NORTHSTAR", "Northstar",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
			w, h, NULL, NULL, instance, NULL);

		SetWindowLongPtr((HWND)_window_handle, GWLP_USERDATA, (LONG)this);
	}

	//Aggiunge servizio
	void kernel::add_service(service* service)
	{
		//Controlla che non ci siano servizi con la stessa priorità
		for(auto serv : _services)
		{
			if (serv->_renderPriority == service->_renderPriority)
				printf("\n\"%s\" e \"%s\" condividono la stessa priorita' di rendering!",
					serv->_name.c_str(), service->_name.c_str());

			if (serv->_renderPriority == service->_renderPriority)
				printf("\n\"%s\" e \"%s\" condividono la stessa priorita' di updating!",
					serv->_name.c_str(), service->_name.c_str());
		}

		_services.push_back(service);

		_updateList.insert(service);
		_renderList.insert(service);
	}

	//Ottiene un servizio registrato, ritorna null se non esiste
	service* kernel::get_service(const string& name)
	{
		for(service* serv : _services)
		{
			if (serv->_name == name)
				return serv;
		}
		return nullptr;
	}
}