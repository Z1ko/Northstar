#pragma once
#include "kernel.hpp"

namespace ns
{
	class platform_service;
	class input_service;
	class graphics_service;
	class scene_service;

	//Servizio principale, presenta funzioni che ne semplificano l'uso
	class application
	{
		friend class kernel;

	public:
		virtual void initialize() { };
		virtual void update() { };

		//Setta i servizi principali sempre presenti
		void set_core_services(platform_service* platform, input_service* input,
			graphics_service* graphics, scene_service* scene);

	protected:
		//Servizi principali essenziali
		platform_service*	_platform;
		input_service*		_input;
		graphics_service*	_graphics;
		scene_service*		_scene;
	};
}