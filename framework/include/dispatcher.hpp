#ifndef	__DISPATCHER_HPP__
#define	__DISPATCHER_HPP__

#include <list>
#include <vector>
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
		virtual void subscribe(const std::shared_ptr<IListener<Event>>& listener);
	private:
		std::vector<std::shared_ptr<IListener<Event>>> m_listeners;
	};

	template <class Event>
	void Dispatcher<Event>::onEvent(const Event& event) {
		auto iter = m_listeners.begin();
		while (m_listeners.end() != iter) {
			iter.get().get()->onEvent(event);
			++iter;
		}
	}

	template <class Event>
	void Dispatcher<Event>::subscribe(const std::shared_ptr<IListener<Event>>& listener) {
		m_listeners.push_back(listener);
	}
}

#endif // __DISPATCHER_HPP__