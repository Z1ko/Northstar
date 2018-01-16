#pragma once
#include "../common.hpp"

namespace ns
{
	//Identificatore unico
	typedef u32 ident;

	//Gruppo logico di componenti
	class entity
	{
		friend class scene_service;

	public:
		//Solo il servizio della scena può creare entità
		entity() = delete;
		entity(scene_service* scene, ident id);
		entity(scene_service* scene, ident id, const string& name);

	public:
		//Distrugge l'entità
		void destroy();

	public: //Getters
		inline const string& name() {
			return _name;
		}

	private:
		string _name;
		ident  _id;

		//Manager
		scene_service* _scene;
	};
}