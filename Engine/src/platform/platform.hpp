#pragma once
#include "../common.hpp"

#include "../core/service.hpp"
#include "../core/event.hpp"

namespace ns
{
	//ELEMENTI IN COMUNE
	struct os_event_data
	{
	};

	class window
	{
	public:
		//Ottiene lunghezza e altezza
		window();
		window(void* handle);

		inline u32 width() {
			return _width;
		}
		inline u32 height() {
			return _height;
		}

		//Ritorna hadle all'handle della piattaforma
		inline void* handle() {
			return _handle;
		}

	private:
		//Dimensioni
		u32 _width, _height;
		void* _handle;
	};

	//Servizio dedicato alla creazione della finestra e l'acquisizione di eventi dal SO
	class platform_service : public service
	{
	public:
		platform_service(kernel* kernel);

		//Crea finestra e collega callback
		void initialize() override;

		//Ottiene eventi del SO
		void update() override;

	public: //Getters

		//Ottiene handle alla finestra
		inline window get_window() {
			return window(_wnd_handle);
		}

	public:
		//Evento generico del sistema operativo
		event<os_event_data*> os_event;

	private:
		//Handle alla finestra specifico del SO
		void* _wnd_handle;
	};
}