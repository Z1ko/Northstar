#include "renderer.hpp"

#include "graphics.hpp"

namespace ns
{
	renderer::renderer(graphics_service* graphics, scene_service* scene)
		: _graphics(graphics), _scene(scene)
	{
	}
}