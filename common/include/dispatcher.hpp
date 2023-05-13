#ifndef	__DISPATCHER_HPP__
#define	__DISPATCHER_HPP__

#include <set>
#include <algorithm>
#include "ilistener.hpp"

namespace common {

	template <class Event>
	class Dispatcher: public IListener<Event>
	{
	public:
		virtual void onEvent(const Event& event) override;
		virtual void subscribe(const IListener<Event> *listener);
		virtual void unsubscribe(const IListener<Event> *listener);
	private:
		std::set<IListener<Event> *> listeners_;
	};

	template <class Event>
	void Dispatcher<Event>::onEvent(const Event& event) {
		std::for_each(
			listeners_.begin(),
			listeners_.end(),
			[&](IListener<Event> *curr_listener) {
				curr_listener->onEvent(event);
			}
		);
	}

	template <class Event>
	void Dispatcher<Event>::subscribe(const IListener<Event> *listener) {
		listeners_.insert(const_cast<IListener<Event> *>(listener));
	}

	template <class Event>
	void Dispatcher<Event>::unsubscribe(const IListener<Event> *listener) {
		listeners_.erase(const_cast<IListener<Event> *>(listener));
	}

}

#endif