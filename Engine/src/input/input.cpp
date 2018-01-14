#include "input.hpp"

#include <Windows.h>
#include <Windowsx.h>

namespace ns
{
	w32_input_service::w32_input_service(kernel* kernel)
		: service(kernel, "input", NS_CORE_SERVICE_INPUT)
	{
		_mouse_position = { 0,0 };

		//Resetta tutto
		for (u32 i = 0; i < 26; ++i) {
			_states[i] = key_states::released;
		}
	}

	//Registra evento piattaforma
	void w32_input_service::initialize() {
		NS_EVENT_SUBSCRIBE(_kernel->platform_event, this, &w32_input_service::handle_platform_event);
	}

	//Evento di windows
	struct w32_event_data
	{
		UINT message;

		WPARAM wParam;
		LPARAM lParam;
	};

	//Risponde agli eventi del sistema operativo
	void w32_input_service::handle_platform_event(void* platform_data)
	{
		w32_event_data* w32data = (w32_event_data*)platform_data;

		static u32 key_offset = 0x41;
		switch (w32data->message)
		{
			case WM_KEYDOWN: {
				u32 key_index = w32data->wParam - (u32)key_codes::A;

				//Processa i tasti delle lettere
				if (key_index < 26) {
					//Impedisce l'invio di molteplici messaggi "keydown" quando si sa già
					if (_states[key_index] != key_states::released)
						break;

					//Genera evento
					key_event_data data;
					data.key = (key_codes)(w32data->wParam);
					data.state = key_states::pressed;
					key_event(&data);

					//Aggiorna stato
					_states[key_index] = key_states::pressed;
				}
				else { //Processa testi speciali
					switch ((key_codes)w32data->wParam) {
						case key_codes::Enter:

							//Genera evento
							key_event_data data;
							data.key = (key_codes)w32data->wParam;
							data.state = key_states::pressed;
							key_event(&data);

						break;
					}
				}
			} break;
			case WM_KEYUP: {
				u32 key_index = w32data->wParam - (u32)key_codes::A;

				//Processa i tasti delle lettere
				if (key_index < 26) {
					//Genera evento
					key_event_data data;
					data.key = (key_codes)w32data->wParam;
					data.state = key_states::released;
					key_event(&data);

					//Aggiorna stato
					_states[key_index] = key_states::released;
				}
				else { //Processa tasti speciali
					switch ((key_codes)w32data->wParam)
					{
						case key_codes::Enter:

							//Genera evento
							key_event_data data;
							data.key = (key_codes)w32data->wParam;
							data.state = key_states::released;
							key_event(&data);

						break;
					}
				}
			} break;
			case WM_CHAR: {
				//Genera evento
				text_event_data data;
				data.value = (char)w32data->wParam;
				text_event(&data);

			} break;
			case WM_MOUSEMOVE: {
				mouse_event_data data = { 0 };

				data.position.x = GET_X_LPARAM(w32data->lParam);
				data.position.y = GET_Y_LPARAM(w32data->lParam);

				data.delta.x = data.position.x - _mouse_position.x;
				data.delta.y = data.position.y - _mouse_position.y;

				_mouse_position.x = data.position.x;
				_mouse_position.y = data.position.y;

				mouse_event(&data);
			} break;
		}
	}

	//Controlla lo stato dei tasti
	void w32_input_service::update()
	{
		static u32 key_offset = 0x41;
		for(u32 i = 0; i < 26; ++i)
		{
			if (_states[i] == key_states::pressed || _states[i] == key_states::repeated) {

				//Genera evento
				key_event_data data;
				data.key = (key_codes)(i + key_offset);
				data.state = key_states::repeated;
				key_event(&data);

				//Aggiorna stato
				_states[i] = key_states::repeated;
			}
		}
	}

	//Ottiene lo stato di un tasto
	key_states w32_input_service::key_state(key_codes key)
	{
		u32 key_offset = (u32)key - (u32)key_codes::A;
		return _states[key_offset];
	}

	//Setta la posizione del mouse (relativa alla finestra)
	void w32_input_service::set_mouse_position(u32 x, u32 y)
	{
		HWND window = (HWND)_kernel->window_handle();
		POINT point{ x, y };

		ScreenToClient(window, &point);
		SetCursorPos(point.x, point.y);
	}
}