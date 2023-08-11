#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>

#include "raw_data_communication_manager.hpp"

using namespace communication;

static inline std::vector<char> strToVector(const std::string& str) {
	std::vector<char> result;
	std::for_each(
		str.begin(),
		str.end(), 
		[&](const auto& iter) { 
			result.push_back(iter); 
		}
	);
	return result;
}

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::size_t length_field_size(2UL);
	const std::size_t length_max(200UL);

	RawDataCommunicationManager comm_manager(header, length_field_size, length_max);
	comm_manager.onEvent('c');

	return 0;
}