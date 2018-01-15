#pragma once
#include "platform.hpp"

#include <Windows.h>

namespace ns
{
	//Evento di windows
	struct w32_event_data 
		: public os_event_data
	{
		UINT message;

		WPARAM wParam;
		LPARAM lParam;
	};
}