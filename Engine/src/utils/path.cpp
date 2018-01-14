#include "path.hpp"

namespace ns
{
	//Risolve testo
	path::path(const string& text) {
		_resolved_text = path::resolve(text);
	}

	//Tutte le macro registrate
	unordered_map<string, string> path::_macros;

	//Sostituisce le macro
	string path::resolve(const string& text) {

		stringstream stream;

		for(u32 i = 0; i < text.length(); ++i) {
		
			char token = text[i];

			//Risoluzione marco
			if(token == '@')
			{
				u32 beg = i + 2;
				u32 end = text.find_first_of(')', beg);
				u32 lenght = end - beg;

				string keyword = text.substr(beg, lenght);
				stream << _macros.at(keyword);

				i += lenght + 2;
			}
			else stream << token;
		}

		return stream.str();
	}

	//Aggiunge una nuova macro
	void path::add_macro(const string& name, const string& value) {
		_macros.emplace(name, value);
	}

}