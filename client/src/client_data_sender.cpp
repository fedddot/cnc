#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include "client_raw_data_sender.hpp"

using namespace communication;

ClientRawDataSender::ClientRawDataSender(const std::vector<char>& header, const std::size_t& length_field_size): RawDataSender(header, length_field_size) {

}

void ClientRawDataSender::send(const std::vector<char>& data) {
	// TODO: implement (Serial port)
}