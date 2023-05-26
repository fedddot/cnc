#ifndef	__DISPATCHER_HPP__
#define	__DISPATCHER_HPP__

#include "list.hpp"
#include "shared_ptr.hpp"
#include "ilistener.hpp"

namespace common {
	template <class Event>
	class Dispatcher: public IListener<Event> {
	public:
		Dispatcher() = default;
		Dispatcher(const Dispatcher<T>& other) = delete;
		Dispatcher& operator=(const Dispatcher<T>& other) = delete;
		~Dispatcher() noexcept = default;
		
		virtual void onEvent(const Event& event) override;
		virtual void subscribe(const data::SharedPtr<IListener<Event>> listener);
	private:
		data::List<data::SharedPtr<IListener<Event>>> m_listeners;
	};

	template <class Event>
	void Dispatcher<Event>::onEvent(const Event& event) {
		// std::for_each(
		// 	m_listeners.begin(),
		// 	m_listeners.end(),
		// 	[&](IListener<Event> *curr_listener) {
		// 		curr_listener->onEvent(event);
		// 	}
		// );
	}

	template <class Event>
	void Dispatcher<Event>::subscribe(const data::SharedPtr<IListener<Event>> listener) {
		// m_listeners.insert(const_cast<IListener<Event> *>(listener));
	}
}

#endif // __DISPATCHER_HPP__