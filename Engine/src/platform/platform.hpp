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
		inline void* window() {
			return _wnd_handle;
		}

	public:
		//Evento generico del sistema operativo
		event<os_event_data*> os_event;

	private:
		//Handle alla finestra specifico del SO
		void* _wnd_handle;
	};
}