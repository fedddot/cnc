#include <stdexcept>
#include <iostream>

#include "test_message_listener.hpp"
#include "ilistener.hpp"

using namespace testing;

void TestMessageListener::onEvent(const message::Message& event) {
	std::cout << event.data() << std::endl;
}
