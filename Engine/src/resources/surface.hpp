#pragma once
#include "../common.hpp"

#include "../utils/path.hpp"

namespace ns
{
	struct rgba32
	{
		char r, g, b, a;
	};
	struct rgb24
	{
		char r, g, b;
	};

	//Rappresenta un'immagine semplice
	class surface
	{
	public:
		surface();
		surface(path filepath);
		~surface();

		inline i32 width() {
			return _width;
		}
		inline i32 height() {
			return _height;
		}
		inline i32 bpp() {
			return _bpp;
		}

		//Numero di bytes presenti
		inline i32 bytes() {
			return _width * _height * _bpp;
		}

	private:
		//Dimensioni e numero di bit per pixel
		i32 _width, _height, _bpp;
		//Pixels dell'immagine
		char* _pixels;
	};
}