#include "shader.hpp"

/*
namespace ns
{
	//Aggiunge file risorsa
	void shader_factory::add_common_file(path filepath) {
		string source = ns::load_file_text(filepath.text());
		string name = path::name(filepath);
		_commons.emplace(name, source);
	}

	//Genera source e crea shader
	shader* shader_factory::load_from_file(path filepath) {
		string source = ns::load_file_text(filepath);
		stringstream stream(source);

		subshader_type current_type = subshader_type::null;
		stringstream vtx_source, pxl_source;

		string line;
		while(std::getline(stream, line))
		{
			//Dichiarazione shader
			u32 i = line.find("#shader");
			if (i != string::npos) {

				i += strlen("#shader \"");
				u32 end = line.find_first_of(' ', i);

				string target = line.substr(i, end - i);
				if (target == "vertex")
					current_type == subshader_type::vertex;
				else if (target == "pixel")
					current_type == subshader_type::pixel;
			}
			else if (line.find('#') == string::npos){
				switch (current_type) {
					case subshader_type::vertex:
						vtx_source << line;
						break;
					case subshader_type::pixel:
						pxl_source << line;
						break;
				}
			}
		}
		return nullptr;
	}
}
*/