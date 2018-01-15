#include "graphics.hpp"

#include "../platform/platform.hpp"

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
		platform_service* platform = (platform_service*)_kernel->get_service("platform");
		_context.attach_window(platform->window());
	}

	//Aggiorna renderer e pulisce lo schermo
	void graphics_service::render()
	{
		glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f( 0.5f, -0.5f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f( 0.0f,  0.5f);
		glEnd();

		//Swappa buffers
		_context.swap_buffer();
	}
}