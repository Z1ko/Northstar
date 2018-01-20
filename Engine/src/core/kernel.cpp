#include "kernel.hpp"

#include "../utils/path.hpp"

#include "application.hpp"

#include "../input/input.hpp"
#include "../graphics/graphics.hpp"
#include "../scene/scene.hpp"

#include "../platform/win32.hpp"

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
		//Crea console di base
		this->create_console();

		//Cartella dov'è presente l'exe e carica opzioni
		path::add_macro("root", find_root_directory(argv[0]));
		this->load_standard_options();

		//SERVIZI CORE

		_platform = new platform_service(this);
		this->add_service(_platform);

		_input = new input_service(this);
		this->add_service(_input);

		_graphics = new graphics_service(this);
		this->add_service(_graphics);

		_scene = new scene_service(this);
		this->add_service(_scene);
	}

	//Carica impostazioni dalla posizione standard e le processa
	void kernel::load_standard_options() {
		//Carica opzioni principali
		auto log = log::get("engine");
		if (_settings.load("@(root)/options.json")) {
			json_value macros = _settings.find("macros");
			if (macros) {
				log->debug("Caricamento macros...");
				for (auto& macro : macros.values()) {
					string value = macro.second.get<string>();
					log->debug("\t{} -> {}", macro.first, value);
					path::add_macro(macro.first, path::resolve(value));
				}
			}
		}
	}

	//Avvia applicazione
	void kernel::start(application* app)
	{
		//Inizializza tutti i servizi, in base alla priorità di update
		for (service* serv : _updateList)
			serv->initialize();

		//Inizializza applicazione
		app->set_core_services(_platform, _input, _graphics, _scene);
		app->initialize();

		while(_running)
		{
			//Aggiorna tutti i sistemi in ordine
			for (service* serv : _updateList)
				serv->update();

			//Aggiorna applicazione
			app->update();

			//Aggiorna tutti i sistemi in ordine
			for (service* serv : _updateList)
				serv->post_update();

			//Aggiorna tutti i sistemi in ordine
			for (service* serv : _renderList)
				serv->render();
		}
	}

	//Crea contesto della finestra
	void kernel::create_console()
	{
		//Crea console per debug
		AttachConsole(ATTACH_PARENT_PROCESS);
		AllocConsole();

		SetConsoleTitle("NORTHSTAR32 - CONSOLE");

		freopen("CON", "w", stdout);
		SetConsoleOutputCP(CP_UTF8);

		auto console = log::stdout_color_st("engine");
		console->set_pattern("[%H:%M:%S:%e][%L] %v");

#ifdef _DEBUG
		console->set_level(log::level::debug);
#endif
	}

	//Aggiunge servizio
	void kernel::add_service(service* service)
	{
		//Controlla che non ci siano servizi con la stessa priorità
		for(auto serv : _services) {
			if (serv->_renderPriority == service->_renderPriority)
				log::get("engine")->error("\n\"{}\" e \"{}\" condividono la stessa priorita' di rendering!",
					serv->_name, service->_name);
			if (serv->_renderPriority == service->_renderPriority)
				log::get("engine")->error("\n\"{}\" e \"{}\" condividono la stessa priorita' di rendering!",
					serv->_name, service->_name);
		}

		_services.push_back(service);

		_updateList.insert(service);
		_renderList.insert(service);

		log::get("engine")->info("Servizio: \"{}\", (update : {}, rendering : {})", 
			service->_name, service->_updatePriority, service->_renderPriority);
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