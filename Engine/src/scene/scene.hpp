#pragma once

#include "../core/kernel.hpp"
#include "../core/service.hpp"

#include "entity.hpp"

namespace ns
{
	//Classe che organizza le entità, i componenti ed i sistemi
	class scene_service : public service
	{
		//Funzione usata per i template delle entità
		typedef entity*(*factory_func)(entity*);

	public:
		scene_service(kernel* kernel);

		//Vuoto
		void initialize() override { };

	public: //Entities

		//Crea nuova entità vuota
		entity* create_entity();
		entity* create_entity(const string& name);

		//Trova entità
		entity* find_entity(ident id);
		entity* find_entity(const string& name);

		//Rimuove entità
		void destroy_entity(ident id);
		void destroy_entity(const string& name);

	public: //Factory

		//Registra un nuovo template delle entità
		void register_template(const string& name, factory_func func);

		//Crea una nuova entità usando un template
		entity* create_template_entity(const string& type);
		entity* create_template_entity(const string& type, const string& name);

	public: //Components

		//Ritorna il bucket di un tipo di componenti
		template<typename T>
		component_bucket<T>* bucket() {
			u32 type = T::type();
			if (_buckets[type] == nullptr)
				_buckets[type] = new component_bucket<T>();
			return (component_bucket<T>*)_buckets[type];
		}

		//Ottiene componente in base al numero di tipo, senza controllo di allocazione!
		inline component_bucket_access* bucket(u32 component_type) {
			return _buckets[component_type];
		}

	private:
		//Tutte le entità create
		list<entity> _entities;
		//Incrementa con ogni nuova entità
		u32 _created_count = 0;

		//Mappa dei template delle entità conosciuti
		unordered_map<string, factory_func> _factory;

		//Contiene tutti i bucket dei componenti
		array<component_bucket_access*, NS_COMPONENT_TYPES> _buckets;
	};
}
