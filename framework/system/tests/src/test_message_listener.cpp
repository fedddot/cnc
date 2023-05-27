#include <stdexcept>
#include <iostream>

#include "test_message_listener.hpp"
#include "ilistener.hpp"
#include "list.hpp"

using namespace testing;
using namespace data;

void TestMessageListener::onEvent(const List<char>& event) {
	auto iter = const_cast<List<char>&>(event).begin();
	while (!iter.isEndIter()) {
		std::cout << iter.get();
		++iter;
	}
	std::cout << std::endl;
}
