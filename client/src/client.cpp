#include <cstddef>
#include <vector>
#include <algorithm>

#include "movement_task.hpp"
#include "data_receiver.hpp"
#include "ilistener.hpp"

using namespace task;
using namespace data;

int main(void) {
	const std::string header_str("msg_header");
	std::vector<char> header;
	std::for_each(header_str.begin(), header_str.end(), [&](const auto& iter) { header.push_back(iter); });
	
	DataReceiver receiver(header, 2UL, 200UL);

	// send header
	std::for_each(header.begin(), header.end(), [&](const auto& iter) { receiver.onEvent(iter); });

	// send size
	std::vector<char> size_vect {0, 10};
	std::for_each(size_vect.begin(), size_vect.end(), [&](const auto& iter) { receiver.onEvent(iter); });

	// send data
	const std::string data("0123456789");
	std::for_each(data.begin(), data.end(), [&](const auto& iter) { receiver.onEvent(iter); });
	return 0;
}