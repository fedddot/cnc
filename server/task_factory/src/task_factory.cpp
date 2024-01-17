#include <map>
#include <stdexcept>

#include "integer.hpp"
#include "object.hpp"
#include "data.hpp"
#include "creator.hpp"
#include "task_factory.hpp"

using namespace task_factory;

const std::string task_factory::TaskFactory::s_task_type_field_name("task_type");


TaskFactory::TaskFactory(): m_creators({
	{TaskType::INIT_GPIO, nullptr}, 
	{TaskType::INIT_STEP_MOTOR, nullptr}, 
	{TaskType::MOVE, nullptr}
}) {

}

TaskFactory::TaskType TaskFactory::get_task_type(const TaskCfg& cfg) {
	const cnc_engine::Object& cfg_object = TaskCfg::cast<cnc_engine::Object>(cfg);
	if (!cfg_object.contains(s_task_type_field_name)) {
		throw std::invalid_argument("received cfg object doesn't contain field " + s_task_type_field_name);
	}
	const cnc_engine::Integer& int_type = TaskCfg::cast<cnc_engine::Integer>(cfg_object.access(s_task_type_field_name));
	
	switch (int_type.get()) {
	case static_cast<int>(TaskType::INIT_GPIO):
		return TaskType::INIT_GPIO;
	case static_cast<int>(TaskType::INIT_STEP_MOTOR):
		return TaskType::INIT_STEP_MOTOR;
	case static_cast<int>(TaskType::MOVE):
		return TaskType::MOVE;
	}
	throw std::invalid_argument("failed to retrieve task type from the config");
}

Task *TaskFactory::create(const TaskCfg& cfg) const {
	auto iter = m_creators.find(get_task_type(cfg));
	if (m_creators.end() == iter) {
		throw std::invalid_argument("creator matching received cfg is not registered");
	}
	return iter->second->create(cfg);
}