#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "message_parser.hpp"
#include "test_message_listener.hpp"
#include "json_utils.hpp"

using namespace testing;
using namespace cnc_system;

TEST(SanityTests, MessageParserOnEvent) {
	TestMessageListener listener;

	std::vector<char> signature = json::stringToVector("sign");

	std::string msg_str("Hey Parser!");
	std::vector<char> message(json::stringToVector(msg_str));

	std::vector<char> size = {0, static_cast<char>(message.size())};

	MessageParser parser(signature, size.size());
	parser.setMessageListener(&listener);
	parser.onEvent('a');
	parser.onEvent('a');
	parser.onEvent('a');
	for (auto it = signature.begin(); it != signature.end(); ++it) {
		parser.onEvent(*it);
	}
	for (auto it = size.begin(); it != size.end(); ++it) {
		parser.onEvent(*it);
	}
	for (auto it = message.begin(); it != message.end(); ++it) {
		parser.onEvent(*it);
	}
}
