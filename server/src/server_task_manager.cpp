#include <vector>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <sstream>

#include "movement_task.hpp"
#include "idata_sender.hpp"
#include "idata.hpp"
#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "json_serializer.hpp"
#include "json_parser.hpp"
#include "server_movement_task.hpp"


#include "server_task_manager.hpp"

using namespace cnc;
using namespace data;
using namespace json;

ServerTaskManager::ServerTaskManager(TaskFactory *task_factory): m_task_factory(init_task_factory(task_factory)) {
}

static std::shared_ptr<IData> parseData(const std::vector<char>& data) {
	std::stringstream data_stream;
	std::for_each(
		data.begin(),
		data.end(),
		[&](const auto& iter) {
			data_stream << iter;
		}
	);
	JsonParser parser;
	auto parsed_data = parser.parse(data_stream);
	return parsed_data;
}

void ServerTaskManager::onEvent(const std::vector<char>& event) {
	auto parsed_data_ptr = parseData(event);
	if (nullptr == parsed_data_ptr) {
		throw std::runtime_error("failed to parse raw data");
	}

	auto task_ptr = m_task_factory->create(*parsed_data_ptr);
	if (nullptr == task_ptr) {
		throw std::runtime_error("failed to create task");
	}
	task_ptr->execute();
}

TaskFactory *ServerTaskManager::init_task_factory(TaskFactory *task_factory) {
	if (nullptr == task_factory) {
		throw std::invalid_argument("task_factory must not be initialized with nullptr");
	}
	return task_factory;
}