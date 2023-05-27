#include <stdexcept>
#include <iostream>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-more-matchers.h"

#include "message_parser.hpp"
#include "test_message_listener.hpp"
#include "list.hpp"
#include "string.hpp"

using namespace testing;
using namespace data;
using namespace message;

static List<char> strToList(const String& str) {
	List<char> result;
	const char *char_iter = str.c_str();
	while ('\0' != *char_iter) {
		result.push_back(*char_iter);
		++char_iter;
	}
	return result;
}

TEST(SanityTests, MessageParserOnEvent) {
	TestMessageListener listener;

	List<char> signature;
	signature.push_back(1);
	signature.push_back(2);
	signature.push_back(3);
	signature.push_back(4);

	String msg_str("Hey Parser!");
	List<char> message(strToList(msg_str));

	List<char> size;
	size.push_back(0);
	size.push_back(msg_str.size());

	MessageParser parser(signature, size.size());
	parser.setMessageListener(&listener);
	parser.onEvent('a');
	parser.onEvent('a');
	parser.onEvent('a');
	for (auto it = signature.begin(); ~(it.isEndIter()); ++it) {
		parser.onEvent(it.get());
	}
	for (auto it = size.begin(); ~(it.isEndIter()); ++it) {
		parser.onEvent(it.get());
	}
	for (auto it = message.begin(); ~(it.isEndIter()); ++it) {
		parser.onEvent(it.get());
	}
}
