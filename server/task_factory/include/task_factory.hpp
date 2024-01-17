#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include "factory.hpp"
#include "report.hpp"
#include "task.hpp"
#include "data.hpp"

namespace task_factory {
	using Task = cnc_engine::Task<cnc_engine::Report>;
	using TaskCfg = cnc_engine::Data;
	using Report = cnc_engine::Report;

	class TaskFactory: public cnc_engine::Factory<Task *, TaskCfg> {
	public:
		TaskFactory();
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;
		virtual Task *create(const TaskCfg& cfg) const override;
	};
}
#endif // TASK_FACTORY_HPP