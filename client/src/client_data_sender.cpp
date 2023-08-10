#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include "client_data_sender.hpp"

using namespace data;

ClientDataSender::ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size): DataSender(header, length_field_size) {

}

void ClientDataSender::send(const std::vector<char>& data) {
	const std::vector<char> serialized_data_length(get_serial_data_length(data.size()));
	auto header = get_header();
	
	// TODO: implement it properly
	if (!m_receiver) {
		return;
	}
	// send header
	std::for_each(
		header.cbegin(), 
		header.cend(), 
		[&](const auto& iter) { 
			m_receiver->onEvent(iter); 
		}
	);

	// send size
	std::for_each(
		serialized_data_length.cbegin(), 
		serialized_data_length.cend(), 
		[&](const auto& iter) { 
			m_receiver->onEvent(iter); 
		}
	);

	// send data
	std::for_each(
		data.cbegin(), 
		data.cend(), 
		[&](const auto& iter) { 
			m_receiver->onEvent(iter); 
		}
	);
}