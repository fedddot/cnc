#ifndef	DELAY_ALLOCATOR_HPP
#define	DELAY_ALLOCATOR_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "task_executor.hpp"

namespace cnc {
	class DelayAllocator {
	public:
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;
		using TaskIdGenerator = mcu_server::Creator<int(void)>;
		
		DelayAllocator(
			int delay_ms,
			const TaskIdGenerator& task_id_generator,
			const Executor& executor
		);
		DelayAllocator(const DelayAllocator& other);
		DelayAllocator& operator=(const DelayAllocator& other) = delete;
		
		virtual ~DelayAllocator() noexcept;

		int delay_id() const;
	private:
		const std::unique_ptr<TaskIdGenerator> m_task_id_generator;
		const std::unique_ptr<Executor> m_executor;
		int m_delay_id;

		int allocate_delay(int delay_ms) const;
		void deallocate_task(int task_id) const;
	};

	inline DelayAllocator::DelayAllocator(
		int delay_ms,
		const TaskIdGenerator& task_id_generator,
		const Executor& executor
	): m_task_id_generator(task_id_generator.clone()), m_executor(executor.clone()), m_delay_id(allocate_delay(delay_ms)) {
		
	}

	inline DelayAllocator::~DelayAllocator() noexcept {
		deallocate_task(m_delay_id);
	}

	inline int DelayAllocator::delay_id() const {
		return m_delay_id;
	}

	inline int DelayAllocator::allocate_delay(int delay_ms) const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;
		Object delay_task;
		delay_task.add("task_type", Integer(static_cast<int>(TaskType::DELAY)));
		delay_task.add("delay_ms", Integer(delay_ms));

		int task_id = m_task_id_generator->create();
		Object create_persistent_task;
		create_persistent_task.add("task_type", Integer(static_cast<int>(TaskType::CREATE_PERSISTENT_TASK)));
		create_persistent_task.add("task_id", Integer(task_id));
		create_persistent_task.add("task_data", delay_task);
		
		m_executor->execute(create_persistent_task);
		
		return task_id;
	}

	inline void DelayAllocator::deallocate_task(int task_id) const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;
		Object delete_persistent_task;
		delete_persistent_task.add("task_type", Integer(static_cast<int>(TaskType::DELETE_PERSISTENT_TASK)));
		delete_persistent_task.add("task_id", Integer(task_id));
		
		m_executor->execute(delete_persistent_task);
	}
}

#endif // DELAY_ALLOCATOR_HPP