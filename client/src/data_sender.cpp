#include <stdexcept>
#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>

#include "data_sender.hpp"

using namespace data;

DataSender::DataSender(const std::string& header, const std::size_t& length_field_size): m_header(header), m_length_field_size(init_length_field_size(length_field_size)) {

}

void DataSender::send(const std::string& data) {
	enum {BITS_IN_BYTE = 8};
	const std::list<char> serialized_data_length(serialize_data_length(data.size(), m_length_field_size));
	
	std::stringstream data_stream;
	data_stream << "[header: \"" << m_header << "\"]";
	data_stream << "[serialized_data_length (" << m_length_field_size << " bytes): ";
	auto iter = serialized_data_length.begin();
	while (serialized_data_length.end() != iter) {
		data_stream << static_cast<int>(*iter);
		++iter;
		if (serialized_data_length.end() != iter) {
			data_stream << ", ";
		}
	}
	data_stream << "]";
	data_stream << "[data: \"" << data << "\"]";
	
	// TODO: implement it properly
	std::cout << "DataSender::send: \"" << data_stream.rdbuf() << "\"" << std::endl;
}

std::size_t DataSender::init_length_field_size(const std::size_t& length_field_size) {
	if (length_field_size > sizeof(std::size_t)) {
		throw std::invalid_argument("received length_field_size is too large");
	}
	return length_field_size;
}

std::list<char> DataSender::serialize_data_length(const std::size_t& data_length, const std::size_t& length_field_size) {
	enum {BITS_IN_BYTE = 8, BYTE_MASK = 0xFF};
	
	std::size_t length(data_length);
	std::list<char> data_size_bytewise;
	while (data_size_bytewise.size() < length_field_size) {
		char curr_char = static_cast<char>(length & static_cast<std::size_t>(BYTE_MASK));
		data_size_bytewise.push_front(curr_char);
		length >>= BITS_IN_BYTE;
	}
	return data_size_bytewise;
}