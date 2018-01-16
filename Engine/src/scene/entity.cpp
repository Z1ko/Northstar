#include "entity.hpp"

#include "scene.hpp"

namespace ns
{
	entity::entity(scene_service* scene, ident id)
		: _scene(scene), _id(id)
	{
	}

	entity::entity(scene_service* scene, ident id, const string& name)
		: _scene(scene), _id(id), _name(name)
	{
	}

	//Distrugge l'entità
	void entity::destroy() {
		_scene->destroy_entity(_id);
	}
}