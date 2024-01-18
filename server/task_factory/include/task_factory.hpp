#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <map>

#include "factory.hpp"
#include "report.hpp"
#include "task.hpp"
#include "data.hpp"
#include "creator.hpp"

namespace task_factory {
	class TaskFactory: public cnc_engine::Factory<cnc_engine::Task<cnc_engine::Report> *, cnc_engine::Data> {
	public:
		enum class TaskType: int {
			CREATE_INVENTORY_ITEM,
			DELETE_INVENTORY_ITEM
		};
		using Task = cnc_engine::Task<cnc_engine::Report>;
		using TaskConfigData = cnc_engine::Data;
		using TaskCreator = Creator<Task *, TaskConfigData>;

		TaskFactory(const std::map<TaskType, TaskCreator&>& creators);
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		virtual Task *create(const TaskConfigData& cfg) const override;
	private:
		std::map<TaskType, TaskCreator&> m_creators;
		static const std::string s_task_type_field_name;
		static TaskType retrieve_task_type(const TaskConfigData& cfg);
	};
}
#endif // TASK_FACTORY_HPP