#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include "client_data_sender.hpp"

using namespace communication;

ClientDataSender::ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size): DataSender(header, length_field_size) {

}

void ClientDataSender::send(const std::vector<char>& data) {
	// TODO: implement (Serial port)
}