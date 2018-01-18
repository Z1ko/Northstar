#include "scene.hpp"

namespace ns
{
	scene_service::scene_service(kernel* kernel)
		: service(kernel, "scene", NS_CORE_SERVICE_SCENE)
	{
		for (u32 i = 0; i < NS_COMPONENT_TYPES; ++i)
			_buckets[i] = nullptr;
	}

	//Crea nuova entità vuota
	entity* scene_service::create_entity() {
		_entities.emplace_back(this, ++_created_count);
		return &_entities.back();
	}
	entity* scene_service::create_entity(const string& name) {
		_entities.emplace_back(this, ++_created_count, name);
		return &_entities.back();
	}

	//Trova entità
	entity* scene_service::find_entity(ident id) {
		for (entity& ent : _entities) {
			if (ent._id == id)
				return &ent;
		}
		return nullptr;
	}
	entity* scene_service::find_entity(const string& name) {
		for (entity& ent : _entities) {
			if (ent._name == name)
				return &ent;
		}
		return nullptr;
	}

	//Rimuove entità
	void scene_service::destroy_entity(ident id) {
		_entities.remove_if([&](entity& ent) {
			return ent._id == id;
		});
	}
	void scene_service::destroy_entity(const string& name) {
		_entities.remove_if([&](entity& ent) {
			return ent._name == name;
		});
	}

	//Registra un nuovo template delle entità
	void scene_service::register_template(const string& name, factory_func func) {
		_factory.emplace(name, func);
	}

	//Crea una nuova entità usando un template
	entity* scene_service::create_template_entity(const string& type) {
		return _factory.at(type)(this->create_entity());
	}
	entity* scene_service::create_template_entity(const string& type, const string& name) {
		return _factory.at(type)(this->create_entity(name));
	}
}