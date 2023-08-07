#include <stdexcept>
#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>

#include "data_receiver.hpp"

using namespace data;
using namespace common;

DataReceiver::DataReceiver(std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size): m_header(header), m_length_field_size(init_length_field_size(length_field_size)), m_max_data_size(max_data_size), m_state(ReceiverState::RECEIVING_HEADER), m_data_size(0), m_data_buff() {

}

void DataReceiver::onEvent(char event) {
	switch (m_state) {
	case ReceiverState::RECEIVING_HEADER:
		receive_header(event);
		break;
	case ReceiverState::RECEIVING_SIZE:
		receive_size(event);
		break;
	case ReceiverState::RECEIVING_DATA:
		receive_data(event);
		break;
	default:
		throw DataReceiverException("DataReceiver::onEvent", "unsupported ReceiverState", *this);
	}
}

void DataReceiver::receive_header(char event) {
	m_data_buff.push_back(event);
	if (m_data_buff.size() < m_header.size()) {
		return;
	}
	if (m_header != m_data_buff) {
		m_data_buff.erase(m_data_buff.begin());
		return;
	}
	m_data_buff.clear();
	m_state = ReceiverState::RECEIVING_SIZE;
}

void DataReceiver::receive_size(char event) {
	m_data_buff.push_back(event);
	if (m_data_buff.size() < m_length_field_size) {
		return;
	}
	m_data_size = deserialize_data_length();
	if ((m_data_size == 0) || (m_data_size > m_max_data_size)) {
		reset_receiver();
		return;
	}
	m_data_buff.clear();
	m_state = ReceiverState::RECEIVING_DATA;
}

void DataReceiver::receive_data(char event) {
	m_data_buff.push_back(event);
	if (m_data_buff.size() < m_length_field_size) {
		return;
	}
	std::vector<char> data(m_data_buff);
	reset_receiver();
	if (m_data_listener_ptr) {
		m_data_listener_ptr->onEvent(data);
	}
}

void DataReceiver::reset_receiver() {
	m_state = ReceiverState::RECEIVING_HEADER;
	m_data_size = 0;
	m_data_buff.clear();
}

std::size_t DataReceiver::init_length_field_size(const std::size_t& length_field_size) {
	if (length_field_size > sizeof(std::size_t)) {
		throw std::invalid_argument("received length_field_size is too large");
	}
	return length_field_size;
}

std::size_t DataReceiver::deserialize_data_length() const {

	return 0;
}

DataReceiver::DataReceiverException::DataReceiverException(const std::string& where, const std::string& what, DataReceiver& receiver): m_msg("") {
	receiver.reset_receiver();
	std::stringstream msg_stream;
	msg_stream << "A DataReceiverException exception catched at " << where;
	if ("" != what) {
		msg_stream << " (note: " << what << ")";
	}
	m_msg = msg_stream.str();
}
