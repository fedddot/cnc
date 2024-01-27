#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <map>
#include <string>

#include "creator.hpp"
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
		using TaskCreator = basics::Creator<basics::Task *, data::Data>;

		TaskFactory(const std::map<TaskType, TaskCreator&>& creators);
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		virtual basics::Task *create(const data::Data& cfg) const override;
	private:
		std::map<TaskType, TaskCreator&> m_creators;
		static const std::string s_task_type_field_name;
		static TaskType retrieve_task_type(const data::Data& cfg);
	};
}
#endif // TASK_FACTORY_HPP