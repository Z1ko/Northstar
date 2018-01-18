#pragma once

namespace ns
{
	class graphics_service;
	class scene_service;

	//Classe incaricata di renderizzare
	class renderer
	{
	public:
		renderer(graphics_service* graphics, scene_service* scene);

		//Invocata subito dopo l'aggiornamento dell'applicazione,
		//in questo istante bisogna raccogliere tutte le informazioni
		//necessarie a renderizzare il frame dagli altri servizi
		virtual void pre_render() = 0;

		//Rendering vero e proprio
		virtual void render() = 0;

	protected:
		//Servizio grafico e scena
		graphics_service* _graphics;
		scene_service*    _scene;
	};
}