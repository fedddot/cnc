#include <stdexcept>
#include <iostream>

#include <vector>
#include "ilistener.hpp"
#include "json_utils.hpp"
#include "test_message_listener.hpp"

using namespace testing;

void TestMessageListener::onEvent(const std::vector<char>& event) {
	std::cout << json::vectorToString(event) << std::endl;
}
