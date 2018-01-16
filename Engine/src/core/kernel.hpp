#pragma once
#include "../common.hpp"

#include "service.hpp"
#include "event.hpp"

namespace ns
{
	class application;
	class platform_service;
	class input_service;
	class graphics_service;
	class scene_service;

	//Cuore pulsante dell'engine, amministra i servizi offerti, distribuisce eventi etc
	class kernel
	{
	public:
		//Inizializza tutti i servizi, crea macro standard, carica impostazioni etc
		kernel(char argc, char** argv);

		//Avvia applicazione
		void start(application* app);

	public: //Services

		//Aggiunge servizio
		void add_service(service* service);
		//Ottiene un servizio registrato, ritorna null se non esiste
		service* get_service(const string& name);

	private: //Window

		//Crea contesto della finestra
		void create_console();

	private:
		//Tutti i servizi registrati
		vector<service*> _services;
		
		//Servizi divisi per priorità
		set<service*, update_comp> _updateList;
		set<service*, render_comp> _renderList;

		//Indica se è stato richiesto di uscire
		bool _running = true;

		//Handle alla finestra della piattaforma
		//void* _window_handle;

		//SERVIZI CORE
		platform_service*	_platform;
		input_service*		_input;
		graphics_service*	_graphics;
		scene_service*		_scene;
	};
}

#include "kernel.hpp"
