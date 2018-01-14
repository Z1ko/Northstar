#include "graphics.hpp"

namespace ns
{
	graphics_service::graphics_service(kernel* kernel)
		: service(kernel, "graphics", NS_CORE_SERVICE_GRAPHICS)
	{
	}

	//Crea contesto opengl
	void graphics_service::initialize()
	{
		//Crea contesto opengl
		_context.attach_window(_kernel->window_handle());
	}

	//Aggiorna renderer e pulisce lo schermo
	void graphics_service::render()
	{
		
	}
}