#pragma once
#include "../common.hpp"

namespace ns
{
	//Contesto di opengl
	class gl_context
	{
	public:
		//Collega il contesto ad una finestra
		void attach_window(void* platform_window);

	private:
		//Handle della piattaforma per il contesto
		void* _handle;
	};
}