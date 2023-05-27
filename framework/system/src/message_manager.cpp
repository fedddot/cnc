
#include "list.hpp"

#include "message_manager.hpp"

using namespace cnc_system;

MessageManager::MessageManager(const data::List<char>& start_signature, const size_t& length_field_size): MessageParser(start_signature, length_field_size) {

}