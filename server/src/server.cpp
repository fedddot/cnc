#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>

#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "server_sender.hpp"
#include "server_receiver.hpp"
#include "server_task_manager.hpp"

#include "idata.hpp"
#include "string.hpp"
#include "object.hpp"

#include "uart_handler.hpp"

using namespace task;
using namespace data;
using namespace communication;

static inline std::vector<char> strToVector(const std::string& str) {
	std::vector<char> result;
	result.insert(result.end(), str.begin(), str.end());
	return result;
}

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::size_t length_field_size(2UL);
	const std::size_t length_max(200UL);

	UartHandler uart_handler(115200);
	
	ServerTaskManager task_manager(ServerTaskManager::ServerSenderSmartPtr(new ServerDataSender(header, length_field_size, uart_handler)), "type");

	ServerDataReceiver receiver(header, length_field_size, length_max);
	uart_handler.set_byte_listener(&receiver);

	receiver.set_data_listener(&task_manager);

	uart_handler.start();

	while (true) {
		sleep_ms(1000);
		// uart_handler.send(msg);
	}	

	return 0;
}