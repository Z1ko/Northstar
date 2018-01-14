#include "context.hpp"

#include <Windows.h>

namespace ns
{
	//Collega il contesto ad una finestra
	void gl_context::attach_window(void* platform_window)
	{
		HWND window = (HWND)platform_window;
		HDC device_context = GetDC(window);

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		int res = ChoosePixelFormat(device_context, &pfd);
		if (res == 0)
		{
			//ERRORE
			printf("\nIl pixel formato richiesto non è disponibile");
			return;
		}
		SetPixelFormat(device_context, res, &pfd);

		_handle = wglCreateContext(device_context);
		wglMakeCurrent(device_context, (HGLRC)_handle);

		glewExperimental = true;
		glewInit();
	}
}