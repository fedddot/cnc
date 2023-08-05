#include <stdexcept>
#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>

#include "data_receiver.hpp"

using namespace data;

DataReceiver::DataReceiver(const std::string& header, const std::size_t& length_field_size, const std::size_t& max_data_size): m_header(header), m_length_field_size(init_length_field_size(length_field_size)), m_max_data_size(max_data_size) {

}

std::size_t DataReceiver::init_length_field_size(const std::size_t& length_field_size) {
	if (length_field_size > sizeof(std::size_t)) {
		throw std::invalid_argument("received length_field_size is too large");
	}
	return length_field_size;
}

std::size_t DataReceiver::deserialize_data_length(const std::list<char>& serial_data_length) {
	enum {BITS_IN_BYTE = 8};
	std::size_t length(0UL);
	for_each(
		serial_data_length.begin(),
		serial_data_length.end(),
		[&](const auto& iter) {
			length <<= BITS_IN_BYTE;
			length |= iter;
		}
	);
	return length;
}