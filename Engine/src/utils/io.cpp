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
		printf("\nErrore caricamento file da %s", filepath.text().c_str());
		return string("invalid filepath!");
	}
}