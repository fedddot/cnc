#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "server_sender.hpp"
#include "server_task_manager.hpp"

#include "string.hpp"
#include "object.hpp"

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

	
	ServerTaskManager task_manager(ServerTaskManager::ServerSenderSmartPtr(new ServerDataSender(header, length_field_size)), "type");

	while (true) {
		Object task_conf;
		task_conf["type"] = std::shared_ptr<IData>(new String("movement"));
		task_conf["distance"] = std::shared_ptr<IData>(new String("1.49"));
		task_conf["speed"] = std::shared_ptr<IData>(new String("4.41"));
		task_conf["axis"] = std::shared_ptr<IData>(new String("1"));

		task_manager.onEvent(task_conf);
	}	

	return 0;
}