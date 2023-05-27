
#include "list.hpp"

#include "message_manager.hpp"

using namespace system;

MessageManager::MessageManager(const data::List<char>& start_signature, const size_t& length_field_size): MessageParser(start_signature, length_field_size) {

}

MessageManager::~MessageManager() noexcept {

}
