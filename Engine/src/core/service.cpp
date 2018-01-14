#include "service.hpp"

#include "kernel.hpp"

namespace ns
{
	service::service(kernel* kernel, const string& name, u32 updatePriority, u32 renderPriority)
		: _kernel(kernel), _name(name), _updatePriority(updatePriority), _renderPriority(renderPriority)
	{
	}

	//Usato nei set per ordinare i servizi in base alla priorità di rendering
	bool render_comp::operator() (service* left, service* right) const {
		return left->_renderPriority < right->_renderPriority;
	}

	//Usato nei set per ordinare i servizi in base alla priorità di updating
	bool update_comp::operator() (service* left, service* right) const {
		return left->_updatePriority < right->_updatePriority;
	}
}