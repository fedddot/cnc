#ifndef	__TEST_MESSAGE_LISTENER_HPP__
#define	__TEST_MESSAGE_LISTENER_HPP__

#include "list.hpp"
#include "ilistener.hpp"

namespace testing {
	class TestMessageListener: public common::IListener<const data::List<char>&> {
public:
	TestMessageListener() = default;
	virtual void onEvent(const data::List<char>& event) override;
}; // TestMessageListener

} // testing

#endif // __TEST_MESSAGE_LISTENER_HPP__