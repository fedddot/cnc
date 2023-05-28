#include <cstddef>
#include <vector>
#include <stdio.h>

#include "uart_message_manager.hpp"
#include "json_utils.hpp"

#define BAUD_RATE 115200

using namespace cnc_system;
using namespace json;

int main() {
    UartMessageManager msg_manager(stringToVector("sign"), 2, BAUD_RATE);
}
