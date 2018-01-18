#include "io.hpp"

#include <fstream>
#include <sstream>

namespace ns
{
	//Carica file di testo
	string load_file_text(path& filepath)
	{
		ifstream file(filepath.text());
		if ((file.rdstate() & ifstream::failbit) == 0)
		{
			stringstream stream;
			stream << file.rdbuf();
			return stream.str();
		}

		//ERRORE
		log::get("engine")->warn("Errore durante il caricamento di {}", filepath.text());
		return string("invalid filepath!");
	}
}