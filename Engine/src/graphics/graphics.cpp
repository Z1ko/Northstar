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
		_platform = (platform_service*)_kernel->get_service("platform");
		_window = _platform->get_window();
		_context.attach_window(_window.handle());

		//Setta callback
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
		glDebugMessageCallback(opengl_callback, this);
	}

	//Aggiorna renderer e pulisce lo schermo
	void graphics_service::render()
	{
		glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Invoca i singoli renderers
		for (auto* renderer : _renderers)
			renderer->render();

		//Swappa buffers
		_context.swap_buffer();
	}

	//Aggiorna i renderers
	void graphics_service::post_update() {
		for (auto* renderer : _renderers)
			renderer->pre_render();
	}

	//Callback di OpenGL
	void APIENTRY opengl_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* param)
	{
		auto& logger = log::get("engine");
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			logger->critical(message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			logger->warn(message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			logger->warn(message);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			logger->info(message);
			break;
		}
	}
}