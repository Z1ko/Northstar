#include "texture.hpp"

/*
namespace ns
{
	texture::texture()
		: _gl_handle(0)
	{
	}

	texture::texture(surface* surface)
		: _gl_handle(0), _width(surface->width()), _height(surface->height()), _bpp(surface->bpp())
	{
		glGenTextures(1, &_gl_handle);
		glBindTexture(GL_TEXTURE_2D, _gl_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		u32 format = (_bpp == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, format, GL_UNSIGNED_BYTE, surface->pixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	texture::~texture() {
		glDeleteTextures(1, &_gl_handle);
	}
}
*/