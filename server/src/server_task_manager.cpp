#include <string>
#include <memory>
#include <stdexcept>

#include "factory.hpp"
#include "icreator.hpp"
#include "isender.hpp"
#include "idata.hpp"
#include "ilistener.hpp"
#include "itask.hpp"

#include "object.hpp"
#include "array.hpp"
#include "string.hpp"

#include "server_movement_task.hpp"
#include "server_movement_task_creator.hpp"


#include "server_task_manager.hpp"

using namespace cnc;
using namespace data;
using namespace common;

ServerTaskManager::ServerTaskManager(const std::string& task_type_key_field): m_task_type_key_field(task_type_key_field), m_sender(nullptr) {
	init_creators();
}

void ServerTaskManager::onEvent(const data::IData& event) {
	try {
		const std::string task_type = get_task_type(event);
		auto task_ptr = m_task_factory.create(task_type, event);
		task_ptr->execute();
		auto task_result = task_ptr->getResult();
		report_task_result(*task_result);
	} catch (const std::exception& e) {
		report_exception("onEvent", e.what());
	}
}

void ServerTaskManager::init_creators() {
	m_task_factory.register_creator("movement", std::shared_ptr<ICreator<std::shared_ptr<ICncTask>, IData>>(new MovementTaskCreator("distance", "speed", "axis")));
}

std::string ServerTaskManager::get_task_type(const data::IData& event) {
	data::Object& cfg_obj = dynamic_cast<data::Object&>(const_cast<data::IData&>(event));
	auto iter = cfg_obj.find(m_task_type_key_field);
	if (cfg_obj.end() == iter) {
		throw std::invalid_argument("field " + m_task_type_key_field + " was not found in received data object");
	}
	auto task_type_ptr = iter->second;
	if (nullptr == task_type_ptr) {
		throw std::invalid_argument("field " + m_task_type_key_field + " initialized to nullptr");
	}
	return data::String(dynamic_cast<const data::String&>(*task_type_ptr));
}

void ServerTaskManager::report_exception(const std::string& where, const std::string& what) {
	Object report;
	report.insert({"result", std::shared_ptr<IData>(new String("exception catched"))});
	report.insert({"where", std::shared_ptr<IData>(new String("ServerTaskManager::" + where))});
	report.insert({"what", std::shared_ptr<IData>(new String(what))});
	m_sender->send(report);
}

void ServerTaskManager::report_task_result(const data::IData& task_result) {
	m_sender->send(task_result);
}