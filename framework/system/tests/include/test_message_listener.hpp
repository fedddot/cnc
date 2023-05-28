#ifndef	__TEST_MESSAGE_LISTENER_HPP__
#define	__TEST_MESSAGE_LISTENER_HPP__

#include <vector>
#include "ilistener.hpp"

namespace testing {
	class TestMessageListener: public common::IListener<const std::vector<char>&> {
public:
	TestMessageListener() = default;
	virtual void onEvent(const std::vector<char>& event) override;
}; // TestMessageListener

} // testing

#endif // __TEST_MESSAGE_LISTENER_HPP__