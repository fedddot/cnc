#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include "data_sender.hpp"

using namespace communication;

DataSender::DataSender(const std::vector<char>& header, const std::size_t& length_field_size): m_header(header), m_length_field_size(init_length_field_size(length_field_size)) {

}

std::size_t DataSender::init_length_field_size(const std::size_t& length_field_size) {
	if (length_field_size > sizeof(std::size_t)) {
		throw std::invalid_argument("received length_field_size is too large");
	}
	return length_field_size;
}

std::vector<char> DataSender::serialize_data_size(const std::size_t& data_size) const {
	enum {BITS_IN_BYTE = 8, BYTE_MASK = 0xFF};
	
	std::size_t length(data_size);
	std::list<char> data_size_bytewise;
	while (data_size_bytewise.size() < m_length_field_size) {
		char curr_char = static_cast<char>(length & static_cast<std::size_t>(BYTE_MASK));
		data_size_bytewise.push_front(curr_char);
		length >>= BITS_IN_BYTE;
	}
	std::vector<char> result;
	std::for_each(
		data_size_bytewise.begin(),
		data_size_bytewise.end(),
		[&](const auto& iter) {
			result.push_back(iter);
		}
	);
	return result;
}