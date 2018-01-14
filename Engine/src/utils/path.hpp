#pragma once
#include "../common.hpp"

namespace ns
{
	//Class che risolve automaticamente i percorsi,
	//tramite l'uso di macro
	class path
	{
	public:
		//Risolve testo
		path(const string& text);

		//Ottiene testo risolto
		inline const string& text()
		{
			return _resolved_text;
		}

	private:
		string _resolved_text;

	//======================================================
	//					STATIC

	public:
		//Sostituisce le macro
		static string resolve(const string& text);

		//Aggiunge una nuova macro
		static void add_macro(const string& name, const string& value);

	private:
		//Tutte le macro registrate
		static unordered_map<string, string> _macros;
	};
}