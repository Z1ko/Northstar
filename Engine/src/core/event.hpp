#pragma once
#include "../common.hpp"

//Crea delegato di un metodo
#define NS_MAKE_DELEGATE(Object, Method) \
ns::make_delegate(Method).bind<Method>(Object)

//Iscrive un metodo ad un evento
#define NS_EVENT_SUBSCRIBE(Event, Object, Method) \
Event.subscribe(NS_MAKE_DELEGATE(Object, Method));

//Disiscrive un metodo da un evento
#define NS_EVENT_UNSUBSCRIBE(Event, Object, Method) \
Event.unsubscribe(NS_MAKE_DELEGATE(Object, Method))

namespace ns
{
	template<typename ... params>
	class delegate
	{
		typedef void(*type)(void* callee, params...);

	public:
		delegate(void* callee, type function)
			: _callee(callee), _function(function)
		{
		}

		inline void operator() (params... args) {
			(*_function)(_callee, args...);
		}

		bool operator == (const delegate<params...>& other) {
			return _callee == other._callee && _function == other._function;
		}

	private:
		void* _callee;
		type  _function;
	};

	template<typename T, typename ... params>
	struct delegate_builder
	{
		template<void(T::*method)(params...)>
		static void method_caller(void* callee, params... args) {
			return (static_cast<T*>(callee)->*method)(args...);
		}

		template<void(T::*method)(params...)>
		inline static delegate<params...> bind(T* callee) {
			return delegate<params...>(callee, &delegate_builder::method_caller<method>);
		}
	};

	template<typename T, typename... params>
	delegate_builder<T, params... > make_delegate(void(T::*)(params...)) {
		return delegate_builder<T, params...>();
	}

	template<typename ... params>
	class event
	{
	public:
		//Aggiunge un delegato
		void subscribe(const delegate<params...>& delegate) {
			_delegates.push_back(delegate);
		}

		//Rimuove un delegato
		void unsubscribe(const delegate<params...>& delegate) {
			for (u32 i = 0; i < _delegates.size(); ++i) {
				if (_delegates[i] == delegate) {
					_delegates.erase(_delegates.begin() + i);
					return;
				}
			}
		}

		//Propaga evento fra tutti gli iscritti
		void operator() (params ... args) {
			for (auto& delegate : _delegates)
				delegate(args...);
		}

	private:
		//Iscritti all'evento
		vector<delegate<params...>> _delegates;
	};
}