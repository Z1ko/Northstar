#pragma once

#include "../core/kernel.hpp"
#include "../core/service.hpp"
#include "../core/event.hpp"

#include "../platform/platform.hpp"

namespace ns
{
	enum class key_codes : u32 //VK_WIN32
	{ 
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		L = 0x4A,
		J = 0x4B,
		K = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		Enter = 0x0D
	};

	enum class key_states : u32
	{
		pressed, released, repeated
	};

	//Per quando viene premuto-rilasciato-ripetuto un tasto
	struct key_event_data
	{
		key_states state;
		key_codes  key;
	};

	//Per quando serve ottenere un testo dall'user
	struct text_event_data
	{
		char value;
	};

	//Per quando viene mosso il mouse
	struct mouse_event_data
	{
		//Delta dall'ultima posizione
		struct { u32 x, y; } delta;

		//Posizione relativa all'interno della finestra
		struct { u32 x, y; } position;
	};

	//Servizio che trasforma i messaggi della piattaforma in eventi interni.
	//Specifico per windows
	class input_service : public service
	{
	public:
		input_service(kernel* kernel);

		void initialize() override;

		//Risponde agli eventi del sistema operativo
		void handle_platform_event(os_event_data* event);

		//Genera eventi di ripetizione
		void update() override;

		//Ottiene lo stato di un tasto
		key_states key_state(key_codes key);

		//Setta la posizione del mouse (relativa alla finestra)
		void set_mouse_position(u32 x, u32 y);

	public: //Eventi divisi per tipo

		//Viene premuto un tasto
		ns::event<key_event_data*>   key_event;
		//Viene premuto un tasto (formato stringa)
		ns::event<text_event_data*>  text_event;
		//Il cursore viene mosso
		ns::event<mouse_event_data*> mouse_event;

	private:
		//Permette di generare eventi di repeat 
		//e per ottenere lo stato dei tasti fuori dagli eventi
		key_states _states[26];

		//Posizione attuale del mouse all'interno della finestra
		struct { u32 x, y; } _mouse_position;

		//Servizio che supervisiona il SO
		platform_service* _platform;
	};
}