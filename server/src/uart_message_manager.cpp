#include "list.hpp"
#include "message_manager.hpp"
#include "uart_message_manager.hpp"

using namespace cnc_system;

UartMessageManager::UartMessageManager(const data::List<char>& start_signature, const size_t& length_field_size): MessageManager(start_signature, length_field_size) {

}

void UartMessageManager::send(const data::List<char>& message) {

}