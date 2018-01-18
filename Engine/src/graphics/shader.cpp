#include "shader.hpp"


namespace ns
{
	shader::shader()
		: _handle(0)
	{
	}

	//Carica da file
	shader::shader(path filepath)
		: _handle(0)
	{
		shader_sources sources = load_file(filepath);

		u32 vtx_id = compile_sub_shader(sources.vertex, shader_type::vertex);
		u32 pxl_id = compile_sub_shader(sources.pixel,  shader_type::pixel);

		_handle = create_program(vtx_id, pxl_id);

		destroy_sub_shader(vtx_id);
		destroy_sub_shader(pxl_id);
	}

	shader::~shader() {
		glDeleteProgram(_handle);
	}

	//Carica file e applica preprocessore
	shader_sources shader::load_file(path& filepath) {

		string source = load_file_text(filepath);
		stringstream stream(source);

		shader_type type = shader_type::vertex;
		stringstream vtx_stream, pxl_stream;

		string line;
		while (std::getline(stream, line)) {

			u32 s = line.find("#shader ");
			if (s != string::npos)
			{
				s += strlen("#shader ");
				u32 end = line.find_first_of(" \n");

				string name = line.substr(s, end - s);

				if (name == "vertex")
					type == shader_type::vertex;
				else if (name == "pixel")
					type == shader_type::pixel;

				continue;
			}

			u32 i = line.find("#include ");
			if (i != string::npos)
			{
				i += strlen("#include ");

				u32 beg = line.find_first_of('"', i) + 1;
				u32 end = line.find_first_of('"', beg);

				string filename = line.substr(beg, end - beg);

				auto& target = (type == shader_type::vertex) ? vtx_stream : pxl_stream;
				insert_include_source(filename, target);

				continue;
			}

			auto& target = (type == shader_type::vertex) ? vtx_stream : pxl_stream;
			target << line;
		}

		shader_sources sources;
		sources.vertex = vtx_stream.str();
		sources.pixel  = pxl_stream.str();

		return sources;
	}

	//Inserisce un file incluso
	void shader::insert_include_source(string& filename, stringstream& stream) {
		stream << _included.at(filename);
	}

	//Compila sub-shader
	u32 shader::compile_sub_shader(const string& source, shader_type type) {
		const char* c_str = source.c_str();

		u32 ty = (type == shader_type::vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		u32 id = glCreateShader(ty);

		glShaderSource(id, 0, &c_str, NULL);
		glCompileShader(id);

		return id;
	}

	void shader::destroy_sub_shader(u32 id) {
		glDeleteShader(id);
	}

	//Linka sub shaders in un unico programma
	u32 shader::create_program(u32 vtx_id, u32 pxl_id) {
		u32 id = glCreateProgram();
		glAttachShader(id, vtx_id);
		glAttachShader(id, pxl_id);
		glLinkProgram(id);
		return id;
	}

	//Tutti i file includibili caricati
	unordered_map<string, string> shader::_included;

	//Aggiunge nuovo file includibile
	void shader::include(const string& name, path filepath) {
		_included.emplace(name, load_file_text(filepath));
	}


	/*
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
	*/
}