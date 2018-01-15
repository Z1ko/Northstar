#include "surface.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ns
{
	surface::surface()
		: _pixels(nullptr), _width(-1), _height(-1), _bpp(-1)
	{
	}

	//Carica da file
	surface::surface(path filepath) {
		_pixels = (char*)stbi_load(filepath.text().c_str(), &_width, &_height, &_bpp, STBI_rgb_alpha);
		if (_pixels == nullptr) {
			//ERRORE
			printf("\nErrore caricamento surface!");
			return;
		}
	}

	//Elimina file da memoria
	surface::~surface() {
		stbi_image_free(_pixels);
	}
}