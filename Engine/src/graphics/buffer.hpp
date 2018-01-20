#pragma once
#include "../common.hpp"

namespace ns
{
	enum buffer_type : GLenum
	{
		ns_array   = GL_ARRAY_BUFFER,		 //Per qualunque cosa
		ns_indices = GL_ELEMENT_ARRAY_BUFFER //Specifico per gli indici
	};

	enum buffer_usage : GLenum
	{
		ns_static_usage  = GL_STATIC_DRAW, 
		ns_dynamic_usage = GL_DYNAMIC_DRAW
	};

	enum buffer_access : GLenum
	{
		ns_read_only  = GL_READ_ONLY,
		ns_write_only = GL_WRITE_ONLY,
		ns_read_write = GL_READ_WRITE
	};

	//Sub-buffer per la GPU specifico
	template<typename T>
	class sub_buffer
	{
	public:
		sub_buffer()
			: _handle(0), _type(0), _usage(0), _size(0)
		{
		}
		sub_buffer(const vector<T>& elements, buffer_type type, buffer_usage usage)
			: _handle(0), _type(type), _usage(usage), _size(elements.size())
		{
			glGenBuffers(1, &_handle);
			glBindBuffer(_type, _handle);
			glBufferData(_type, _size * sizeof(T), elements.data(), _usage);
		}

		//Permette di modificare direttamente la memoria
		T* map(buffer_access access) {
			return (T*)glMapBuffer(GL_ARRAY_BUFFER, access)
		}
		//Blocca modifica diretta
		void unmap() {
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		//Inizia ad usare questo buffer
		void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, _handle);
		}
		//Smette di usare questo buffer
		void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, _handle);
		}

	public: // Getters

		inline u32 usage() {
			return _usage;
		}
		inline u32 size() {
			return _size;
		}

	private:
		u32 _handle, _size;
		buffer_type  _type;
		buffer_usage _usage;
	};

	//Tutti gli indici sono necessariamente di questo tipo
	typedef u32 index;

	enum buffer_layout_type : GLenum
	{
		ns_byte    = GL_BYTE,
		ns_float   = GL_FLOAT,
		ns_short   = GL_SHORT,
		ns_u_short = GL_UNSIGNED_SHORT,
	};

	//Descrive il layout di un buffer
	template<typename vtx>
	class vertex_layout
	{
		struct buffer_layout_element
		{
			u32 count;
			u32 type;
			u32 stride;
		};

	public:
		//Aggiunge un nuovo elemento
		void add(u32 count, buffer_layout_type type, u32 stride) {
			_elements.push_back({ count, type, stride });
		}

		//Invia ad opengl il layout del buffer
		void upload() {
			auto& logger = log::get("engine");
			logger->debug("Definizione vertex layout:");
			for (u32 i = 0; i < _elements.size(); ++i) {
				auto& e = _elements[i];
				glVertexAttribPointer(i, e.count, e.type, false, 0, (void*)e.stride);
				logger->debug("\t(array: {}, count: {}, type: {}, stride: {})", i, e.count, e.type, e.stride);
			}
		}

	private:
		vector<buffer_layout_element> _elements;
	};

	//Indica come considerare vertici ed indici
	enum topology : GLenum
	{
		ns_triangles = GL_TRIANGLES,
		ns_quads	 = GL_QUADS
	};

	//Rappresenta una mesh a livello di GPU
	template<typename vtx, typename idx>
	class static_mesh_buffer
	{
	public:
		//Crea buffer con solo indici
		static_mesh_buffer(const vector<vtx>& vertices, vertex_layout<vtx>& vtx_layout)
			: _only_vertices(true), _vtx_buffer(vertices, ns_static_usage), _vtx_layout(vtx_layout)
		{
		};
		//Crea buffer partendo dai vertici e dagli indici
		static_mesh_buffer(const vector<vtx>& vertices, vertex_layout<vtx>& vtx_layout, const vector<idx>& indices)
			: _only_vertices(false), _vtx_buffer(vertices, ns_array, ns_static_usage), _vtx_layout(vtx_layout)
			, _idx_buffer(indices, ns_indices, ns_static_usage)
		{
			glGenVertexArrays(1, &_handle);
			glBindVertexArray(_handle);

			_vtx_buffer.bind();
			_vtx_layout.upload();
			_idx_buffer.bind();

			glBindVertexArray(0);
		};

		//Inizia ad usare questo buffer
		void bind() const {
			glBindVertexArray(_handle);
		}
		//Smette di usare questo buffer
		void unbind() const {
			glBindVertexArray(0);
		}

		//Semplice comando di rendering
		void render(topology topology) {
			glBindVertexArray(_handle);
			if (_only_vertices)
				glDrawArrays(topology, 0, _vtx_buffer.size());
			else
				glDrawElements(topology, _idx_buffer.size(), GL_UNSIGNED_INT, nullptr);
		}

	private:
		//Sub-buffer per i vertici
		sub_buffer<vtx> _vtx_buffer;
		//Layout dei vertici
		vertex_layout<vtx> _vtx_layout;
		//Sub-buffer per gli indici
		sub_buffer<idx> _idx_buffer;

		//Handle per il VAO
		u32 _handle;

		//Vero se non sono stati specificati gli indici
		bool _only_vertices;
	};

	//Binda il buffer all'interno di uno scope
	template<typename vtx, typename idx>
	class mesh_buffer_lock
	{
	public:
		//Binda buffer
		mesh_buffer_lock(const static_mesh_buffer<vtx, idx>& target)
			: _target(target)
		{
			_target.bind();
		}
		//Unbinda buffer
		~mesh_buffer_lock()
		{
			_target.unbind();
		}

	private:
		const static_mesh_buffer<vtx, idx>& _target;
	};
}