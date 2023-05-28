#include <cstddef>
#include <vector>

#include "message_manager.hpp"

using namespace cnc_system;

MessageManager::MessageManager(const std::vector<char>& start_signature, const std::size_t& length_field_size): MessageParser(start_signature, length_field_size) {

}