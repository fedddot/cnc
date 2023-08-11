#include <string>
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <list>
#include <algorithm>

#include "ilistener.hpp"
#include "ireceiver.hpp"

#include "data_receiver.hpp"

using namespace communication;
using namespace common;

RawDataReceiver::RawDataReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size): m_header(header), m_length_field_size(init_length_field_size(length_field_size)), m_max_data_size(max_data_size), m_state(ReceiverState::RECEIVING_HEADER), m_data_size(0), m_data_buff() {

}

void RawDataReceiver::onEvent(char event) {
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
		throw std::runtime_error("RawDataReceiver::onEvent: unsupported ReceiverState");
	}
}

void RawDataReceiver::receive_header(char event) {
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

void RawDataReceiver::receive_size(char event) {
	m_data_buff.push_back(event);
	if (m_data_buff.size() < m_length_field_size) {
		return;
	}
	m_data_size = deserialize_data_length(m_data_buff);
	if ((m_data_size == 0) || (m_data_size > m_max_data_size)) {
		reset_receiver();
		return;
	}
	m_data_buff.clear();
	m_state = ReceiverState::RECEIVING_DATA;
}

void RawDataReceiver::receive_data(char event) {
	m_data_buff.push_back(event);
	if (m_data_buff.size() < m_data_size) {
		return;
	}
	std::vector<char> data(m_data_buff);
	reset_receiver();
	if (m_data_listener_ptr) {
		m_data_listener_ptr->onEvent(data);
	}
}

void RawDataReceiver::reset_receiver() {
	m_state = ReceiverState::RECEIVING_HEADER;
	m_data_size = 0;
	m_data_buff.clear();
}

std::size_t RawDataReceiver::init_length_field_size(const std::size_t& length_field_size) {
	if (length_field_size > sizeof(std::size_t)) {
		throw std::invalid_argument("received length_field_size is too large");
	}
	return length_field_size;
}

std::size_t RawDataReceiver::deserialize_data_length(const std::vector<char>& data) {
	enum { BITS_IN_BITE = 8 };
	std::size_t data_length(0UL);
	std::for_each(
		data.begin(),
		data.end(),
		[&](const auto& iter) {
			data_length <<= BITS_IN_BITE;
			data_length |= static_cast<std::size_t>(iter);
		}
	);
	return data_length;
}