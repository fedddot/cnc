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
		Dispatcher(const Dispatcher<Event>& other) = default;
		Dispatcher& operator=(const Dispatcher<Event>& other) = default;
		virtual ~Dispatcher() noexcept override = default;
		
		virtual void onEvent(const Event& event) override;
		virtual void subscribe(const memory::SharedPtr<IListener<Event>>& listener);
	private:
		data::List<memory::SharedPtr<IListener<Event>>> m_listeners;
	};

	template <class Event>
	void Dispatcher<Event>::onEvent(const Event& event) {
		auto iter = m_listeners.begin();
		while (!iter.isEndIter()) {
			iter.get().get()->onEvent(event);
		}
	}

	template <class Event>
	void Dispatcher<Event>::subscribe(const memory::SharedPtr<IListener<Event>>& listener) {
		m_listeners.push_back(listener);
	}
}

#endif // __DISPATCHER_HPP__