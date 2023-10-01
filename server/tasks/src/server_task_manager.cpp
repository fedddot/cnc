#include <list>
#include <stdexcept>

#include <string>
#include <vector>
#include <memory>

// Tasks
#include "server_task.hpp"
#include "factory.hpp"
#include "register_gpio_task_creator.hpp"
#include "register_stepper_motor_task_creator.hpp"

// Data
#include "object.hpp"
#include "string.hpp"
#include "json_parser.hpp"
#include "json_serializer.hpp"

// Hardware
#include "gpio.hpp"
#include "stepper_motor.hpp"

#include "package_manager.hpp"
#include "ilistener.hpp"

#include "server_task_manager.hpp"

namespace task {
	typedef common::Factory<std::string, std::shared_ptr<ServerTask>, data::Object> TasksFactory;

	typedef common::ICreator<std::shared_ptr<ServerTask>, data::Object> TasksCreator;

	typedef common::Registry<hardware::Gpio::PinNumber, std::shared_ptr<hardware::Gpio>> GpioRegistry;
	typedef common::Registry<std::string, std::shared_ptr<hardware::StepperMotor>> StepperMotorRegistry;

	typedef std::list<std::shared_ptr<ServerTask>> TasksList;

	class ServerTaskManagerResources {
	public:
		ServerTaskManagerResources();
		ServerTaskManagerResources(const ServerTaskManagerResources& other) = delete;
		ServerTaskManagerResources& operator=(const ServerTaskManagerResources& other) = delete;
		~ServerTaskManagerResources() noexcept = default;

		inline GpioRegistry& gpio_registry();
		inline StepperMotorRegistry& stepper_motor_registry();
		inline TasksFactory& task_factory();
		inline TasksList& task_list();
		inline json::JsonParser& json_parser();
		inline json::JsonSerializer& json_serializer();
	private:
		GpioRegistry m_gpio_registry;
		StepperMotorRegistry m_stepper_motor_registry;
		TasksFactory m_task_factory;
		TasksList m_task_list;
		json::JsonParser m_json_parser;
		json::JsonSerializer m_json_serializer;
	};
}

using namespace task;
using namespace data;
using namespace json;
using namespace common;
using namespace communication;

ServerTaskManager::ServerTaskManager(communication::PackageManager& package_manager): m_package_manager(package_manager), m_resources(new ServerTaskManagerResources()) {
	m_package_manager.receiver().subscribe(this);
}

ServerTaskManager::~ServerTaskManager() noexcept {
	m_package_manager.receiver().unsubscribe(this);
}

template <class T>
static T getField(const Object& data_object, const std::string& field_name) {
	auto iter = data_object.find(field_name);
	if (data_object.end() == iter) {
		throw std::invalid_argument("field \"" + field_name + "\" not found");
	}
	if (nullptr == iter->second) {
		throw std::invalid_argument("nullptr is stored under field \"" + field_name + "\"");
	}
	return T(*(iter->second));
}

void ServerTaskManager::on_event(const std::vector<char>& event) {
	try {
		auto parsed_data = (m_resources->json_parser()).parse(event);
		Object& config_data = dynamic_cast<Object&>(*parsed_data);
		String task_type = getField<String>(config_data, "type");
		auto task_ptr = (m_resources->task_factory()).create(task_type, config_data);
		(m_resources->task_list()).push_back(task_ptr);
	} catch (const std::exception& e) {
		report_exception("on_event", e.what());
	}
}

bool ServerTaskManager::is_task_pending() const {
	return !((m_resources->task_list()).empty());
}

void ServerTaskManager::run_pending_task() {
	if (!is_task_pending()) {
		report_exception("run_pending_task", "there is no task pending");
		return;
	}
	auto task_iter = (m_resources->task_list()).begin();
	task_iter->get()->execute();
	Object report = task_iter->get()->report();
	m_package_manager.sender().send((m_resources->json_serializer()).serialize(report));
	(m_resources->task_list()).erase(task_iter);
}

void ServerTaskManager::report_exception(const std::string& where, const std::string& what) {
	Object report;
	report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
	report.insert({"where", std::shared_ptr<IData>(new String("ServerTaskManager::" + where))});
	report.insert({"what", std::shared_ptr<IData>(new String(what))});
	m_package_manager.sender().send((m_resources->json_serializer()).serialize(report));
}

ServerTaskManagerResources::ServerTaskManagerResources() {
	m_task_factory.register_creator("RegisterGpioTask", std::shared_ptr<TasksCreator>(new RegisterGpioTaskCreator(m_gpio_registry)));
	m_task_factory.register_creator("RegisterStepperMotorTask", std::shared_ptr<TasksCreator>(new RegisterStepperMotorTaskCreator(m_stepper_motor_registry, m_gpio_registry)));
}

inline GpioRegistry& ServerTaskManagerResources::gpio_registry() {
	return m_gpio_registry;
}

inline StepperMotorRegistry& ServerTaskManagerResources::stepper_motor_registry() {
	return m_stepper_motor_registry;
}

inline TasksFactory& ServerTaskManagerResources::task_factory() {
	return m_task_factory;
}

inline TasksList& ServerTaskManagerResources::task_list() {
	return m_task_list;
}

inline json::JsonParser& ServerTaskManagerResources::json_parser() {
	return m_json_parser;
}

inline json::JsonSerializer& ServerTaskManagerResources::json_serializer() {
	return m_json_serializer;
}