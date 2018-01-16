#include "application.hpp"

#include "../platform/platform.hpp"
#include "../input/input.hpp"
#include "../graphics/graphics.hpp"
#include "../scene/scene.hpp"

namespace ns
{
	//Setta i servizi principali sempre presenti
	void application::set_core_services(platform_service* platform, input_service* input,
		graphics_service* graphics, scene_service* scene) {
		_platform = platform;
		_input = input;
		_graphics = graphics;
		_scene = scene;
	}
}