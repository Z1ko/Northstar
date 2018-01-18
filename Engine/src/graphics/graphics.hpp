#pragma once

#include "context.hpp"
#include "renderer.hpp"

#include "../core/kernel.hpp"
#include "../core/service.hpp"

#include "../platform/platform.hpp"

namespace ns
{
	//Mantiene il contesto opengl e la finestra (quindi si occupa anche di ottenere gli eventi del sistema operativo)
	class graphics_service : public service
	{
	public:
		graphics_service(kernel* kernel);

		//Crea contesto opengl
		void initialize() override;

		//Aggiorna renderer e pulisce lo schermo
		void render() override;

		//Genera lista di entità da renderizzare e le invia ai renderer
		void post_update() override;

		inline void add_renderer(renderer* renderer) {
			log::get("engine")->info("Aggiunto nuovo renderer!");
			_renderers.push_back(renderer);
		}

		inline window get_window() {
			return _window;
		}

	private:
		//Contesto opengl che permette il rendering
		gl_context _context;

		platform_service* _platform;
		window			  _window;

		//Tutti i renderers registrati (es: terreno - opachi - trasparenti - GUI)
		vector<renderer*> _renderers;
	};
}