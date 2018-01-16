#pragma once

#include "../core/kernel.hpp"
#include "../core/service.hpp"

#include "entity.hpp"

namespace ns
{
	//Classe che organizza le entit�, i componenti ed i sistemi
	class scene_service : public service
	{
		//Funzione usata per i template delle entit�
		typedef entity*(*factory_func)(entity*);

	public:
		scene_service(kernel* kernel);

		//Vuoto
		void initialize() override { };

	public: //Entities

		//Crea nuova entit� vuota
		entity* create_entity();
		entity* create_entity(const string& name);

		//Trova entit�
		entity* find_entity(ident id);
		entity* find_entity(const string& name);

		//Rimuove entit�
		void destroy_entity(ident id);
		void destroy_entity(const string& name);

	public: //Factory

		//Registra un nuovo template delle entit�
		void register_template(const string& name, factory_func func);

		//Crea una nuova entit� usando un template
		entity* create_template_entity(const string& type);
		entity* create_template_entity(const string& type, const string& name);

	private:
		//Tutte le entit� create
		list<entity> _entities;
		//Incrementa con ogni nuova entit�
		u32 _created_count = 0;

		//Mappa dei template delle entit� conosciuti
		unordered_map<string, factory_func> _factory;
	};
}
