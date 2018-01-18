#pragma once
#include "../common.hpp"

#include "../utils/path.hpp"
#include "../utils/io.hpp"


namespace ns
{
	enum class shader_type
	{
		undefined,
		vertex, 
		pixel
	};

	struct shader_sources
	{
		string vertex;
		string pixel;
	};

	//Programma che gira sulla GPU
	class shader
	{

	public:
		shader();
		//Carica da file
		shader(path filepath);

		~shader();

	private:
		//Carica file e applica preprocessore
		shader_sources load_file(path& filepath);

		//Inserisce un file incluso
		void insert_include_source(string& filename, stringstream& stream);

		//Compila sub-shader
		u32  compile_sub_shader(const string& source, shader_type type);
		void destroy_sub_shader(u32 id);

		//Linka sub shaders in un unico programma
		u32 create_program(u32 vtx_id, u32 pxl_id);

	private:
		u32 _handle;

	// === STATIC ==============================================

	public: 
		//Aggiunge nuovo file includibile
		static void include(const string& name, path filepath);

	private:
		//Tutti i file includibili caricati
		static unordered_map<string, string> _included;
	};
}
