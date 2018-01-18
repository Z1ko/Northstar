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

	//Distrugge l'entit�
	void entity::destroy() {
		_scene->destroy_entity(_id);
	}

	//Rimuove tutti i componenti
	void entity::clear() {
		for (u32 i = 0; i < NS_COMPONENT_TYPES; ++i) {
			if (_signature[i]) {
				_signature[i] = false;
				_scene->bucket(i)->remove(_id);
			}
		}
	}
}