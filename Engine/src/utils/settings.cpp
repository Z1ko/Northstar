#include "settings.hpp"

namespace ns
{
	json_value::json_value()
		: _valid(false)
	{
	}
	json_value::json_value(const string& name, picojson::value* value)
		: _valid(true), _name(name), _value(value)
	{ 
	}

	//Trova e ritorna un valore in base al nome
	json_value json_value::find(const string& name) {
		for (auto& val : _value->get<picojson::object>()) {
			if (val.first == name)
				return json_value(name, &val.second);
		}
		return json_value(); //INVALIDO
	}

	json_settings::json_settings()
	{
	}

	//Carica opzioni da file, ritorna false se non è stato trovato il file
	bool json_settings::load(path filepath) {

		string source = load_file_text(filepath);
		string error = picojson::parse(_document, source);

		if (error.empty() == false) {
			std::cerr << error << std::endl;
			return false;
		}

		return true;
	}

	//Trova e ritorna un valore in base al nome, può essere invalido
	json_value json_settings::find(const string& name) {
		for (auto& val : _document.get<picojson::object>()) {
			if (val.first == name)
				return json_value(name, &val.second);
		}
		return json_value(); //INVALIDO
	}
}