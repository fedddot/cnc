#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "factory.hpp"
#include "task.hpp"

namespace task_factory {
	class TaskFactory: public basics::Factory<basics::Task *, data::Data> {
	public:
		enum class TaskType: int {
			CREATE_INVENTORY_ITEM,
			DELETE_INVENTORY_ITEM
		};
		using TaskCreator = std::function<basics::Task *(const data::Data&)>;

		TaskFactory() = default;
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		virtual basics::Task *create(const data::Data& cfg) const override;
		void set_creator(const TaskType& type, const TaskCreator& creator);
		void remove_creator(const TaskType& type);
	private:
		std::map<TaskType, TaskCreator> m_creators;
		static const std::string s_task_type_field_name;
		static TaskType retrieve_task_type(const data::Data& cfg);
	};

	inline void TaskFactory::set_creator(const TaskType& type, const TaskCreator& creator) {
		m_creators[type] = creator;
	}

	inline void TaskFactory::remove_creator(const TaskType& type) {
		auto iter = m_creators.find(type);
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		m_creators.erase(iter);
	}
}
#endif // TASK_FACTORY_HPP