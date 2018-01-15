#pragma once
#include "../common.hpp"

//Ottiene due indici di priorità che sono multipli di dieci
#define NS_CORE_SERVICE_PRIORITY(UpdatingPriority, RenderingPriority) \
(UpdatingPriority * 10), (RenderingPriority * 10)

//Indici di priorità per i servizi base

#define NS_CORE_SERVICE_GRAPHICS \
NS_CORE_SERVICE_PRIORITY(10, 0)

#define NS_CORE_SERVICE_AUDIO \
NS_CORE_SERVICE_PRIORITY(2, 10)

#define NS_CORE_SERVICE_ASSETS \
NS_CORE_SERVICE_PRIORITY(3, 11)

#define NS_CORE_SERVICE_INPUT \
NS_CORE_SERVICE_PRIORITY(1, 12)

#define NS_CORE_SERVICE_PLATFORM \
NS_CORE_SERVICE_PRIORITY(0, 13)

//Etc

namespace ns
{
	//NB: I SERVIZI STANDARD DEVONO AVERE UNA DIFFERENZA DI PRIORITA' MINIMA DI 10 PUNTI
	//    PER PERMETTERE L'AGGIUNTA FUTURA DI NUOVI SERVIZI INTERMEDI

	class service
	{
		friend class kernel;

		//Comparatori usato nel set
		friend struct render_comp;
		friend struct update_comp;

	public:
		service(kernel* kernel, const string& name, u32 updatePriority, u32 renderPriority);

	public:
		//Chiamato quando tutti i servizi sono stati registrati
		virtual void initialize() = 0;

		virtual void update() { };
		virtual void render() { };

	protected:
		//Manager
		kernel* _kernel;

		//Identificativo
		string _name;

		//Ordine con cui verrà chiamato questo servizio durante un tick del kernel
		u32 _updatePriority, _renderPriority;
	};

	//Usato nei set per ordinare i servizi in base alla priorità di rendering
	struct render_comp {
		bool operator() (service* left, service* right) const;
	};

	//Usato nei set per ordinare i servizi in base alla priorità di updating
	struct update_comp {
		bool operator() (service* left, service* right) const;
	};
}