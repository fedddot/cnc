#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstddef>

#include "server_movement_task.hpp"
#include "server_data_sender.hpp"
#include "data_receiver.hpp"
#include "server_message_listener.hpp"

#ifndef IPC_C2S_FILE
#	error "IPC_C2S_FILE path is not defined"
#endif

#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)


using namespace cnc;
using namespace data;
using namespace common;

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

	ServerDataSender sender(header, length_field_size);
	DataReceiver receiver(header, length_field_size, 200UL);
	ServerMessageListener listener(&sender);
	receiver.set_data_listener(&listener);
	
	std::ifstream data_stream(STR(IPC_C2S_FILE), std::ios_base::openmode::_S_in);
	int token = 0;
	while (EOF != (token = data_stream.get())) {
		receiver.onEvent(static_cast<char>(token));
	}
	return 0;
}