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

#include "server_message_listener.hpp"

using namespace cnc;
using namespace data;
using namespace json;

ServerMessageListener::ServerMessageListener(data::IDataSender<const std::vector<char>&> *sender): m_sender(init_sender(sender)) {
}

static Object parse_config_object(const std::vector<char>& event) {
	try {
		JsonParser parser;
		std::stringstream data_stream;
		std::for_each(event.begin(), event.end(), [&](const auto& iter) { data_stream << iter; });
		auto event_parsed = parser.parse(data_stream);
	

	}
	Object& event_object = dynamic_cast<Object&>(*event_parsed);
	return Object(const_cast<const Object&>(event_object));
}

void ServerMessageListener::onEvent(const std::vector<char>& event) {
	Object task_config(parse_config_object(event));
	auto type_iter = task_config.find("type");
	if (task_config.end() == type_iter) {
		return;
	}
	if (distance)
	String distance_str(task_config)
}

IDataSender<const std::vector<char>&> *ServerMessageListener::init_sender(IDataSender<const std::vector<char>&> *sender) {
	if (nullptr == sender) {
		throw std::invalid_argument("sender must not be initialized with nullptr");
	}
	return sender;
}