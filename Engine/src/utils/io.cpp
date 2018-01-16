#include "io.hpp"

#include <fstream>
#include <sstream>

namespace ns
{
	//Carica file di testo
	string load_file_text(path filepath)
	{
		ifstream file(filepath.text());
		if (file.bad() == false)
		{
			stringstream stream;
			stream << file.rdbuf();
			return stream.str();
		}

		return string("invalid filepath!");
	}
}