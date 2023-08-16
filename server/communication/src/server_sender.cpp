#include <stdexcept>
#include <vector>
#include <cstddef>

#include <stdio.h>
#include "pico/stdlib.h"

#include "idata.hpp"
#include "iserializer.hpp"
#include "json_serializer.hpp"
#include "byte_sender.hpp"
#include "server_sender.hpp"

using namespace communication;
using namespace data;
using namespace json;

ServerDataSender::ServerDataSender(const std::vector<char>& header, const std::size_t& length_field_size): ByteSender(header, length_field_size, ByteSender::SerializerSmartPtr(new JsonSerializer)) {
    setup_default_uart();
}

ServerDataSender::~ServerDataSender() noexcept {
	
}

void ServerDataSender::send(const IData& data) {
	auto prepared_data = prepare_data(data);
	std::string str_data;
	str_data.insert(str_data.end(), prepared_data.begin(), prepared_data.end());
	fwrite(str_data.c_str(), 1, str_data.size(), stdout);
}