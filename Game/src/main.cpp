#include <engine.hpp>

class debug_service : public ns::service
{
public:
	debug_service(ns::kernel* kernel)
		: service(kernel, "test", NS_CORE_SERVICE_PRIORITY(30, 30))
	{
	}

	void initialize() override {
		//Registra evento tastiera
		_input = (ns::input_service*)_kernel->get_service("input");

		NS_EVENT_SUBSCRIBE(_input->key_event,   this, &debug_service::on_key_event );
		NS_EVENT_SUBSCRIBE(_input->text_event,  this, &debug_service::on_text_event);
		NS_EVENT_SUBSCRIBE(_input->mouse_event, this, &debug_service::on_mouse_move);
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
	ns::input_service* _input;
};


int main(int argc, char** argv)
{
	//Cuore dell'engine
	ns::kernel kernel(argc, argv);

	//ns::surface spritesheet("@(root)logo.png");

	ns::input_service input(&kernel);
	kernel.add_service(&input);

	ns::graphics_service graphics(&kernel);
	kernel.add_service(&graphics);

	debug_service debug(&kernel);
	kernel.add_service(&debug);

	kernel.start();
	return 0;
}