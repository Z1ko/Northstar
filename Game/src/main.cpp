#include <engine.hpp>

using namespace ns;

class sprite : public component<sprite>
{
public:
	sprite(ident entity_id, f32 x, f32 y, f32 w, f32 h)
		: component(entity_id), x(x), y(y), w(w), h(h)
	{
	}

public:
	f32 x, y, w, h;
};

class sprite_renderer : public renderer
{
public:
	sprite_renderer(graphics_service* graphics, scene_service* scene)
		: renderer(graphics, scene)
	{
	}

	void pre_render() override {
		_sprites.clear();
		for (sprite& sprite : _scene->bucket<sprite>()->components())
			_sprites.push_back(&sprite);
	}

	void render() override {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, _graphics->get_window().width(), _graphics->get_window().height(), 0, -0.1, 100.0);

		glBegin(GL_QUADS);
		for (auto* sprite : _sprites) {
			glVertex2f(sprite->x,  sprite->y);
			glVertex2f(sprite->x + sprite->w,  sprite->y);
			glVertex2f(sprite->x + sprite->w,  sprite->y + sprite->h);
			glVertex2f(sprite->x,  sprite->y + sprite->h);
		}
		glEnd();
	}

private:
	vector<sprite*> _sprites;
};

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

		_scene = (ns::scene_service*)_kernel->get_service("scene");
		_scene->register_template("human", [](ns::entity* base) -> ns::entity* {
			base->assign<sprite>(30.0f, 20.0f, 100.0f, 100.0f);
			return base;
		});

		player = _scene->create_template_entity("human", "player");
		sprite* pos = player->get<sprite>();

		_graphics = (graphics_service*)_kernel->get_service("graphics");
		_renderer = new sprite_renderer(_graphics, _scene);
		_graphics->add_renderer(_renderer);

		//ns::surface spritesheat("@(root)logo.png");
		//_logo = ns::texture(&spritesheat);

		//shader::include("lights", "@(shaders)includes/lights.glsl");
		//shader shad("@(shaders)standard.glsl");
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
	ns::input_service*    _input;
	ns::scene_service*    _scene;
	ns::graphics_service* _graphics;

	ns::entity* player;

	sprite_renderer* _renderer;

	//Logo dell'engine
	//ns::texture _logo;
};

class sandbox : public ns::application
{
	const u32 tile_w = 16;
	const u32 tile_h = 16;

public:
	void initialize() override {
		_window = _platform->get_window();
	}

private:
	window _window;
};

int main(int argc, char** argv)
{
	//Cuore dell'engine
	ns::kernel kernel(argc, argv);

	//ns::surface spritesheet("@(root)logo.png");

	debug_service debug(&kernel);
	kernel.add_service(&debug);

	sandbox app;
	kernel.start(&app);

	return 0;
}