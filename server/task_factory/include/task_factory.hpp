#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <map>
#include <memory>

#include "factory.hpp"
#include "report.hpp"
#include "task.hpp"
#include "data.hpp"
#include "creator.hpp"

namespace task_factory {
	using Task = cnc_engine::Task<cnc_engine::Report>;
	using TaskCfg = cnc_engine::Data;
	using Report = cnc_engine::Report;

	class Gpio;
	class StepperMotor;

	class TaskFactory: public cnc_engine::Factory<Task *, TaskCfg> {
	public:
		TaskFactory();
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		virtual Task *create(const TaskCfg& cfg) const override;

	private:
		enum class TaskType: int {
			INIT_GPIO,
			INIT_STEP_MOTOR,
			MOVE
		};
		std::map<TaskType, std::unique_ptr<Creator<Task *, TaskCfg>>> m_creators;

		static const std::string s_task_type_field_name;

		static TaskType get_task_type(const TaskCfg& cfg);
	};
}
#endif // TASK_FACTORY_HPP