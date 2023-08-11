#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstddef>

#include "server_movement_task.hpp"
#include "server_movement_task_creator.hpp"

#include "server_data_sender.hpp"
#include "data_receiver.hpp"
#include "server_task_manager.hpp"

#include "idata.hpp"
#include "string.hpp"
#include "object.hpp"
#include "array.hpp"

#ifndef IPC_C2S_FILE
#	error "IPC_C2S_FILE path is not defined"
#endif

#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

#define MSG_HEADER			"msg_header"
#define MSG_LEN_FIELD_SIZE	2UL
#define MSG_LEN_MAX			200UL

using namespace cnc;
using namespace data;
using namespace common;

static inline std::vector<char> strToVector(const std::string& str) {
	std::vector<char> result;
	std::for_each(
		str.begin(),
		str.end(), 
		[&](const auto& iter) { 
			result.push_back(iter); 
		}
	);
	return result;
}

int main(void) {
	// Create sender
	ServerDataSender sender(strToVector(MSG_HEADER), MSG_LEN_FIELD_SIZE);

	// Create task factory and register creators for all tasks
	TaskFactory factory;
	factory.register_creator("movement", std::shared_ptr<TaskFactory::ITaskCreator>(new MovementTaskCreator(&sender)));

	// Create task manager
	ServerTaskManager task_manager(&factory);

	// Create receiver and subscribe task_manager for received data
	DataReceiver receiver(strToVector(MSG_HEADER), MSG_LEN_FIELD_SIZE, MSG_LEN_MAX);
	receiver.set_data_listener(&task_manager);


	
	// Simulation of task reading. TODO - reimplement it wit UART interrupts
	std::ifstream data_stream(STR(IPC_C2S_FILE), std::ios_base::openmode::_S_in);
	int token = 0;
	while (EOF != (token = data_stream.get())) {
		receiver.onEvent(static_cast<char>(token));
	}
	return 0;
}