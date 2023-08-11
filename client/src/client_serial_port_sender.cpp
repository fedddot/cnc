#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include "client_serial_port_sender.hpp"

#ifndef IPC_C2S_FILE
#	error "IPC_C2S_FILE path is not defined"
#endif

#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

using namespace data;

ClientDataSender::ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size): DataSender(header, length_field_size) {

}

void ClientDataSender::send(const std::vector<char>& data) {
	const std::vector<char> serialized_data_length(get_serial_data_length(data.size()));
	auto header = get_header();
	
	// TODO: implement it properly
	std::ofstream data_output(STR(IPC_C2S_FILE), std::ios_base::openmode::_S_out);
	// send header
	std::for_each(
		header.cbegin(), 
		header.cend(), 
		[&](const auto& iter) { 
			data_output << iter;
		}
	);

	// send size
	std::for_each(
		serialized_data_length.cbegin(), 
		serialized_data_length.cend(), 
		[&](const auto& iter) { 
			data_output << iter;
		}
	);

	// send data
	std::for_each(
		data.cbegin(), 
		data.cend(), 
		[&](const auto& iter) { 
			data_output << iter;
		}
	);
	data_output.close();
}