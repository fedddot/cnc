#include <stdexcept>
#include <iostream>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "message.hpp"
#include "message_parser.hpp"
#include "test_message_listener.hpp"

using namespace testing;

TEST(SanityTests, MessageParserOnEvent) {
	TestMessageListener listener;
	std::vector<char> signature = {1, 2, 3, 4};
	std::vector<char> size = {0, 0, 0, 10};
	std::vector<char> message = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
	message::MessageParser parser(signature, size.size());
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
