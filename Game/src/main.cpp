#include <engine.hpp>

/*
class graphic_service : public ns::service
{
public:
	graphic_service(ns::kernel* kernel)
		: service(kernel, "input", NS_CORE_SERVICE_GRAPHICS)
	{
	}

	void initialize() override {

		//Registra evento tastiera
		_input = (ns::w32_input_service*)_kernel->get_service("input");
		NS_EVENT_SUBSCRIBE(_input->key_event,   this, &graphic_service::on_key_event );
		NS_EVENT_SUBSCRIBE(_input->text_event,  this, &graphic_service::on_text_event);
		NS_EVENT_SUBSCRIBE(_input->mouse_event, this, &graphic_service::on_mouse_move);
	}

	//Risponde agli eventi del sistema operativo
	void on_key_event(ns::key_event_data* data) {

		if (data->state == ns::key_states::pressed && data->key == ns::key_codes::Enter) {
			//_reset = true;
		}
	}

	//Quando serve un testo
	void on_text_event(ns::text_event_data* data) {

		if (data->value == '\r')
			printf("\n");
		else 
			printf("%c", data->value);
	}

	//Quando viene mosso il cursore
	void on_mouse_move(ns::mouse_event_data* data)
	{
		printf("\nMouse:\t\tP(%i, %i)\t\tD(%i, %i)", 
			data->position.x, data->position.y, data->delta.x, data->delta.y);
	}

	void update() override { }
	void render() override { }

private:
	ns::w32_input_service* _input;
};
*/

int main(int argc, char** argv)
{
	//Cuore dell'engine
	ns::kernel kernel(argc, argv);

	ns::w32_input_service input(&kernel);
	kernel.add_service(&input);

	ns::graphics_service graphics(&kernel);
	kernel.add_service(&graphics);

	kernel.start();
	return 0;
}