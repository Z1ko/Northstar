#pragma once

#include "context.hpp"
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

	private:
		//Contesto opengl che permette il rendering
		gl_context _context;

		platform_service* _platform;
		window			  _window;
	};
}