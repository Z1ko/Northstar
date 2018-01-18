#pragma once
#include "io.hpp"
#include "path.hpp"

namespace ns
{
	//Wrapper di picojson::value
	class json_value
	{
	public:
		json_value(); //INVALIDO
		json_value(const string& name, picojson::value* value); //VALIDO

		//Nome di questo valore
		inline string& name() {
			return _name;
		}

		//Ritorna un vettore contenente tutti gli oggetti superiori
		inline picojson::object values() {
			return _value->get<picojson::object>();
		}

		//Trova e ritorna un valore in base al nome, può essere invalido
		json_value find(const string& name);

		//Vero se il valore è valido
		inline operator bool() {
			return _valid;
		}

	private:
		picojson::value* _value;
		string _name;

		//Indica se questo valore esiste
		bool _valid;
	};

	//Rappresenta un file json
	class json_settings
	{
	public:
		json_settings();
		
		//Carica opzioni da file, ritorna false se non è stato trovato il file
		bool load(path filepath);

		//Ritorna un vettore contenente tutti gli oggetti superiori
		inline picojson::object values() {
			return _document.get<picojson::object>();
		}

		//Trova e ritorna un valore in base al nome, può essere invalido
		json_value find(const string& name);

	private:
		//Documento JSON
		picojson::value _document;
	};
}