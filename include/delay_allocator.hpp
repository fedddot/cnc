#ifndef	DELAY_ALLOCATOR_HPP
#define	DELAY_ALLOCATOR_HPP

#include "custom_persistent_task_allocator.hpp"
#include "object.hpp"
#include "persistent_task_allocator.hpp"

namespace cnc {
	class DelayAllocator: public PersistentTaskAllocator<mcu_server::Object, int> {
	public:
		DelayAllocator(int id, int delay_ms);
		DelayAllocator(const DelayAllocator& other) = default;
		DelayAllocator& operator=(const DelayAllocator& other) = delete;
		
		mcu_server::Object create_data() const override;
		mcu_server::Object delete_data() const override;
		int id() const override;
		ServerObjectAllocator<mcu_server::Object> *clone() const override;
	private:
		CustomPersistentTaskAllocator m_allocator;

		static mcu_server::Object get_delay_task_data(int delay_ms);
	};

	inline DelayAllocator::DelayAllocator(int id, int delay_ms): m_allocator(id, get_delay_task_data(delay_ms)) {
		
	}

	inline mcu_server::Object DelayAllocator::create_data() const {
		return m_allocator.create_data();
	}
	inline mcu_server::Object DelayAllocator::delete_data() const {
		return m_allocator.delete_data();
	}
	
	inline int DelayAllocator::id() const {
		return m_allocator.id();
	}

	inline ServerObjectAllocator<mcu_server::Object> *DelayAllocator::clone() const {
		return new DelayAllocator(*this);
	}

	inline mcu_server::Object DelayAllocator::get_delay_task_data(int delay_ms) {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::DELAY)));
		task_data.add("delay_ms", Integer(delay_ms));
		return task_data;
	}
}

#endif // DELAY_ALLOCATOR_HPP