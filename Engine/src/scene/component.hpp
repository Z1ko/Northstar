#pragma once
#include "../common.hpp"

#define NS_INVALID_IDENT -1

//Numero massimo di tipi di componente
#define NS_COMPONENT_TYPES 64

namespace ns
{
	//Identificatore unico
	typedef u32 ident;

	//Ogni bit rappresenta un tipo di componente
	using component_signature = bitset<NS_COMPONENT_TYPES>;

	//Presenta un incrementatore stati usato per elencare i tipi
	class component_type
	{
	protected:
		static u32 _type_counter;
	};

	template<typename T>
	class component : public component_type
	{
	public:
		component(u32 ent_id)
			: _ent_id(ent_id)
		{
		}

	public:
		//Ritorna un ID unico per questo tipo
		static u32 type()
		{
			static u32 this_type = _type_counter++;
			return this_type;
		}

	private:
		//Id dell'entità che lo possiede
		u32 _ent_id;
	};

	//Permette l'accesso senza sapere T
	class component_bucket_access
	{
	public:
		virtual void remove(ident entity_id) = 0;
	};

	//Mantiene in un array velocemente percorribile tutti i componenti dello stesso tipo
	template<typename T>
	class component_bucket : public component_bucket_access
	{
		struct index
		{
			inline index(u32 offset, ident entity_id)
				: offset(offset), entity_id(entity_id)
			{
			}

			u32	  offset;	 // Offset all'interno della array dei componenti
			ident entity_id; // ID dell'entità che lo possiede
		};

	public:
		//Costruisce un componente assegnandolo all'entità
		template<typename ... Ts>
		void construct(ident entity_id, Ts&& ... args) {
			_indices.emplace(entity_id, index(_components.size(), entity_id));
			_components.emplace_back(entity_id, forward<Ts>(args)...);
			_last_entity_id = entity_id;
		}

		//Ottiene componente
		inline T* retrive(ident entity_id) {
			return &_components[_indices.at(entity_id).offset];
		}

		//Rimuove componente
		void remove(ident entity_id) override {
			index& to_rem = _indices.at(entity_id);
			index& to_mov = _indices.at(_last_entity_id);

			swap(_components[to_rem.offset], _components[to_mov.offset]);
			_components.pop_back();

			to_mov.offset = to_rem.offset;
			_indices.erase(entity_id);
		}

		//Ritorna vero se esiste un componente assegnato a questà entità
		inline bool entity_has_component(ident entity_id) {
			return _indices.find(entity_id) != _indices.end();
		}

		inline auto& components() { 
			return _components;
		}

	private:
		//Permette di accere ai componenti tramite ID di un entità
		unordered_map<ident, index> _indices;
		//Array con tutti i componenti
		vector<T> _components;
		//ID dell'ultima entità inserita, velocizza l'algoritmo di rimozione
		ident _last_entity_id = NS_INVALID_IDENT;
	};
}