#ifndef	CUSTOM_PERSISTENT_TASK_ALLOCATOR_HPP
#define	CUSTOM_PERSISTENT_TASK_ALLOCATOR_HPP

#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "persistent_task_allocator.hpp"
#include "stepper_motor_types.hpp"

namespace cnc {
	class CustomPersistentTaskAllocator: public PersistentTaskAllocator<mcu_server::Object, int> {
	public:
		CustomPersistentTaskAllocator(int task_id, const mcu_server::Object& task_data);
		CustomPersistentTaskAllocator(const CustomPersistentTaskAllocator& other) = default;
		CustomPersistentTaskAllocator& operator=(const CustomPersistentTaskAllocator& other) = delete;
		mcu_server::Object create_data() const override;
		mcu_server::Object delete_data() const override;
		int id() const override;
		PersistentTaskAllocator<mcu_server::Object, int> *clone() const override;
	private:
		int m_task_id;
		mcu_server::Object m_task_data;
	};

	inline CustomPersistentTaskAllocator::CustomPersistentTaskAllocator(int task_id, const mcu_server::Object& task_data): m_task_id(task_id), m_task_data(task_data) {

	}

	inline mcu_server::Object CustomPersistentTaskAllocator::create_data() const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::CREATE_PERSISTENT_TASK)));
		task_data.add("task_id", Integer(m_task_id));
		task_data.add("task_data", m_task_data);
		return task_data;
	}

	inline mcu_server::Object CustomPersistentTaskAllocator::delete_data() const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::DELETE_PERSISTENT_TASK)));
		task_data.add("task_id", Integer(m_task_id));
		return task_data;
	}

	inline int CustomPersistentTaskAllocator::id() const {
		return m_task_id;
	}

	inline PersistentTaskAllocator<mcu_server::Object, int> *CustomPersistentTaskAllocator::clone() const {
		return new CustomPersistentTask(*this);
	}
}

#endif // CUSTOM_PERSISTENT_TASK_ALLOCATOR_HPP