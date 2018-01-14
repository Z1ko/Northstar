#pragma once

#include "context.hpp"
#include "../core/kernel.hpp"
#include "../core/service.hpp"

namespace ns
{
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
	};
}