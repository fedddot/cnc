#ifndef	PERSISTENT_TASK_HPP
#define	PERSISTENT_TASK_HPP

#include <memory>
#include "creator.hpp"
#include "task_executor.hpp"

namespace cnc {
	template <typename Ttask_id, typename Tdata>
	class PersistentTask {
	public:
		using AllocationDataCreator = mcu_server::Creator<Tdata(const Ttask_id&, const Tdata&)>;
		using DeallocationDataCreator = mcu_server::Creator<Tdata(const Ttask_id&)>;
		using TaskIdCreator = mcu_server::Creator<Ttask_id(void)>;
		using PersistentTaskExecutor = TaskExecutor<void(const Tdata&)>;
		
		PersistentTask(
			const Tdata& task_data,
			const AllocationDataCreator& allocation_data_ctor,
			const DeallocationDataCreator& deallocation_data_ctor,
			const TaskIdCreator& task_id_ctor,
			const PersistentTaskExecutor& executor
		);
		PersistentTask(const PersistentTask& other) = delete;
		PersistentTask& operator=(const PersistentTask& other) = delete;
		
		virtual ~PersistentTask() noexcept;

		Ttask_id id() const;
	private:
		const Ttask_id m_id;
		const std::unique_ptr<DeallocationDataCreator> m_deallocation_data_ctor;
		const std::unique_ptr<PersistentTaskExecutor> m_executor;
	};

	template <typename Ttask_id, typename Tdata>
	inline PersistentTask<Ttask_id, Tdata>::PersistentTask(
		const Tdata& task_data,
		const AllocationDataCreator& allocation_data_ctor,
		const DeallocationDataCreator& deallocation_data_ctor,
		const TaskIdCreator& task_id_ctor,
		const PersistentTaskExecutor& executor
	): m_id(task_id_ctor.create()), m_deallocation_data_ctor(deallocation_data_ctor.clone()), m_executor(executor.clone()) {
		m_executor->execute(allocation_data_ctor.create(m_id, task_data));
	}

	template <typename Ttask_id, typename Tdata>
	inline PersistentTask<Ttask_id, Tdata>::~PersistentTask() noexcept {
		m_executor->execute(m_deallocation_data_ctor->create(m_id));
	}

	template <typename Ttask_id, typename Tdata>
	inline Ttask_id PersistentTask<Ttask_id, Tdata>::id() const {
		return m_id;
	}
}

#endif // PERSISTENT_TASK_HPP