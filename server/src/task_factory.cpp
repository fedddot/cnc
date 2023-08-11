#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

#include "idata.hpp"
#include "object.hpp"
#include "array.hpp"
#include "string.hpp"
#include "composite.hpp"
#include "value.hpp"

#include "task_factory.hpp"

using namespace cnc;
using namespace data;
using namespace common;

TaskFactory::TaskFactory(const std::string& type_field_name): m_type_field_name(type_field_name) {

}

std::string TaskFactory::getType(const data::IData& config_data) const {
	Object& config_obj = const_cast<Object&>(dynamic_cast<const Object&>(config_data));
	auto type_iter = config_obj.find(m_type_field_name);
	if (config_obj.end() == type_iter) {
		throw std::invalid_argument("\"" + m_type_field_name + "\" field not found");
	}
	auto typePtr = type_iter->second;
	if (!typePtr) {
		throw std::invalid_argument("type field is nullptr initialized");
	}
	const String& type = dynamic_cast<const String&>(*typePtr);
	return type;
}

std::shared_ptr<ITask> TaskFactory::create(const IData& config_data) {
	std::string type(getType(config_data));
	auto iter = m_creators.find(type);
	if (m_creators.end() == iter) {
		throw std::invalid_argument("creator for received type was not registered");
	}
	return iter->second->create(config_data);
}

void TaskFactory::register_creator(const std::string& task_type, const std::shared_ptr<ITaskCreator>& creator) {
	if (nullptr == creator) {
		throw std::invalid_argument("creator can not be a nullptr");
	}
	m_creators[task_type] = creator;
}