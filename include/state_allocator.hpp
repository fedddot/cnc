#ifndef	STATE_ALLOCATOR_HPP
#define	STATE_ALLOCATOR_HPP

#include "array.hpp"
#include "custom_persistent_task_allocator.hpp"
#include "gpo_allocator.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "persistent_task_allocator.hpp"
#include "server_object_allocator.hpp"
#include "stepper_motor_types.hpp"

namespace cnc {
	class StateAllocator: public PersistentTaskAllocator<mcu_server::Object, int> {
	public:
		StateAllocator(int id, const Shoulders& shoulders, const MotorState& state);
		StateAllocator(const StateAllocator& other) = default;
		StateAllocator& operator=(const StateAllocator& other) = delete;
		
		mcu_server::Object create_data() const override;
		mcu_server::Object delete_data() const override;
		int id() const override;
		ServerObjectAllocator<mcu_server::Object> *clone() const override;
	private:
		CustomPersistentTaskAllocator m_allocator;

		static mcu_server::Object get_apply_task_data(const Shoulders& shoulders, const MotorState& state);
	};

	inline StateAllocator::StateAllocator(int id, const Shoulders& shoulders, const MotorState& state): m_allocator(id, get_apply_task_data(shoulders, state)) {
		
	}

	inline mcu_server::Object StateAllocator::create_data() const {
		return m_allocator.create_data();
	}
	inline mcu_server::Object StateAllocator::delete_data() const {
		return m_allocator.delete_data();
	}
	
	inline int StateAllocator::id() const {
		return m_allocator.id();
	}

	inline ServerObjectAllocator<mcu_server::Object> *StateAllocator::clone() const {
		return new StateAllocator(*this);
	}

	inline mcu_server::Object StateAllocator::get_apply_task_data(const Shoulders& shoulders, const MotorState& state) {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Array set_tasks;
		for (const auto [shoulder, gpo_id]: shoulders) {
			GpoAllocator allocator(gpo_id);
			set_tasks.push_back(allocator.set_data(state.at(shoulder)));
		}
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::SEQUENCE)));
		task_data.add("tasks", set_tasks);
		return task_data;
	}
}

#endif // STATE_ALLOCATOR_HPP