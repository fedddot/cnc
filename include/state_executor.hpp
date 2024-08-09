#ifndef	STATE_EXECUTOR_HPP
#define	STATE_EXECUTOR_HPP

#include <map>
#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "delay_allocator.hpp"
#include "gpo_allocator.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "stepper_motor_types.hpp"
#include "task_executor.hpp"

namespace cnc {
	class StatesExecutor {
	public:
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;
		using TaskId = int;
		using TaskIdCreator = mcu_server::Creator<TaskId(void)>;

		StatesExecutor(
			const Shoulders& shoulders,
			const Executor& executor,
			const TaskIdCreator& task_id_ctor
		);

		StatesExecutor(const StatesExecutor& other) = delete;
		StatesExecutor& operator=(const StatesExecutor& other) = delete;
		virtual ~StatesExecutor() noexcept = default;

		void execute(const MotorStates& states, int timeout_ms) const;
	private:
		std::unique_ptr<Executor> m_executor;
		std::unique_ptr<TaskIdCreator> m_pers_task_id_ctor;
		std::map<Shoulder, std::unique_ptr<GpoAllocator>> m_gpo_allocators;
		
		using TaskTypes = typename mcu_factory::McuFactory<GpoId, TaskId>::TaskType;
	};

	inline StatesExecutor::StatesExecutor(
		const Shoulders& shoulders,
		const Executor& executor,
		const TaskIdCreator& task_id_ctor
	): m_executor(executor.clone()), m_pers_task_id_ctor(task_id_ctor.clone()) {
		for (auto [shoulder, gpio_id]: shoulders) {
			m_gpo_allocators.insert(
				{
					shoulder,
					std::make_unique<GpoAllocator>(
						gpio_id, 
						*m_pers_task_id_ctor, 
						*m_executor
					)
				}
			);
		}
	}

	inline void StatesExecutor::execute(const MotorStates& states, int timeout_ms) const {
		using namespace mcu_server;
		Array steps_ids;
		DelayAllocator delay(timeout_ms, *m_pers_task_id_ctor, *m_executor);
		for (auto state: states) {
			for (auto [shoulder, gpo_state]: state) {
				auto step_id = m_gpo_allocators.at(shoulder)->set_task_id(gpo_state);
				steps_ids.push_back(Integer(step_id));
			}
			steps_ids.push_back(Integer(delay.delay_id()));
		}
		Object execute_tasks;
		execute_tasks.add("task_type", Integer(static_cast<int>(TaskTypes::EXECUTE_PERSISTENT_TASKS)));
		execute_tasks.add("tasks", steps_ids);
		m_executor->execute(execute_tasks);
	}

}

#endif // STATE_EXECUTOR_HPP