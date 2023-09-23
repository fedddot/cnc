#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "factory.hpp"
#include "object.hpp"
#include "string.hpp"
#include "json_parser.hpp"
#include "json_serializer.hpp"
#include "package_manager.hpp"
#include "ilistener.hpp"
#include "server_task.hpp"

// Creators
#include "register_gpio_task_creator.hpp"

#include "server_task_manager.hpp"

using namespace task;
using namespace data;
using namespace json;
using namespace common;
using namespace communication;

ServerTaskManager::ServerTaskManager(communication::PackageManager& package_manager): m_package_manager(package_manager) {
	init_creators();
	m_package_manager.receiver().subscribe(this);
}

ServerTaskManager::~ServerTaskManager() noexcept {
	m_package_manager.receiver().unsubscribe(this);
}

void ServerTaskManager::on_event(const std::vector<char>& event) {
	try {
		Object config_data = parse_raw_data(event);
		const std::string task_type = get_task_type(config_data);
		auto task_ptr = m_task_factory.create(task_type, config_data);
		m_tasks.push_back(task_ptr);
	} catch (const std::exception& e) {
		report_exception("on_event", e.what());
	}
}

bool ServerTaskManager::is_task_pending() const {
	return !(m_tasks.empty());
}

void ServerTaskManager::run_pending_task() {
	if (!is_task_pending()) {
		report_exception("run_pending_task", "there is no task pending");
		return;
	}
	auto task_iter = m_tasks.begin();
	task_iter->get()->execute();
	Object report = task_iter->get()->report();
	JsonSerializer serializer;
	m_package_manager.sender().send(serializer.serialize(report));
	m_tasks.erase(task_iter);
}

void ServerTaskManager::init_creators() {
	m_task_factory.register_creator("RegisterGpioTask", std::shared_ptr<TasksCreator>(new RegisterGpioTaskCreator(m_gpio_registry)));
}

Object ServerTaskManager::parse_raw_data(const std::vector<char>& raw_data) {
	JsonParser parser;
	auto parsed_data_ptr = parser.parse(raw_data);
	if (nullptr == parsed_data_ptr) {
		throw std::runtime_error("failed to parse raw data");
	}
	return Object(dynamic_cast<const Object&>(*parsed_data_ptr));
}

std::string ServerTaskManager::get_task_type(const Object& config_data) {
	const std::string task_type_field("type");
	auto iter = config_data.find(task_type_field);
	if (config_data.end() == iter) {
		throw std::invalid_argument("field " + task_type_field + " was not found in received config_data");
	}
	auto task_type_ptr = iter->second;
	if (nullptr == task_type_ptr) {
		throw std::invalid_argument("field " + task_type_field + " initialized to nullptr");
	}
	return String(dynamic_cast<const String&>(*task_type_ptr));
}

void ServerTaskManager::report_exception(const std::string& where, const std::string& what) {
	Object report;
	report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
	report.insert({"where", std::shared_ptr<IData>(new String("ServerTaskManager::" + where))});
	report.insert({"what", std::shared_ptr<IData>(new String(what))});
	JsonSerializer serializer;
	m_package_manager.sender().send(serializer.serialize(report));
}