#ifndef	__TEST_MESSAGE_LISTENER_HPP__
#define	__TEST_MESSAGE_LISTENER_HPP__

#include <vector>

#include "message.hpp"
#include "ilistener.hpp"

namespace testing {
	class TestMessageListener: public common::IListener<const message::Message&> {
public:
	TestMessageListener() = default;
	virtual void onEvent(const message::Message& event) override;
}; // TestMessageListener

} // testing

#endif // __TEST_MESSAGE_LISTENER_HPP__