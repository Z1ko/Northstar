#pragma once
#include "../common.hpp"

#include "component.hpp"

namespace ns
{
	template<typename T>
	class component_bucket;

	//Gruppo logico di componenti
	class entity
	{
		friend class scene_service;

	public:
		//Solo il servizio della scena può creare entità
		entity() = delete;
		entity(scene_service* scene, ident id);
		entity(scene_service* scene, ident id, const string& name);

		//Distrugge l'entità
		void destroy();

	public:
		//Assegna nuovo componente
		template<typename T, typename ... Ts>
		void assign(Ts&& ... args) {
			auto bucket = _scene->bucket<T>();
			bucket->construct(_id, forward<Ts>(args)...);
			_signature[T::type()] = true;
		}

		template<typename T>
		void remove() {
			_scene->bucket<T>()->remove(_id);
			_signature[T::type()] = false;
		}

		template<typename T>
		T* get() {
			return _scene->bucket<T>()->retrive(_id);
		}

		//Rimuove tutti i componenti
		void clear();

	public: //Getters
		inline const string& name() {
			return _name;
		}

	private:
		string _name;
		ident  _id;

		//Manager
		scene_service* _scene;
		//Indica i componenti posseduti
		component_signature _signature;
	};
}