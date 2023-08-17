#include <stdexcept>
#include <vector>
#include <cstddef>

#include "json_parser.hpp"
#include "byte_receiver.hpp"
#include "server_receiver.hpp"

using namespace communication;
using namespace data;
using namespace json;

ServerDataReceiver::ServerDataReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size): ByteReceiver(header, length_field_size, max_data_size, ByteReceiver::ParserSmartPointer(new JsonParser)) {

}

ServerDataReceiver::~ServerDataReceiver() noexcept {

}