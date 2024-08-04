#ifndef	GPO_ALLOCATOR_HPP
#define	GPO_ALLOCATOR_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "task_executor.hpp"

namespace cnc {
	class GpoAllocator {
	public:
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;
		using TaskIdGenerator = mcu_server::Creator<int(void)>;
		using GpioState = typename mcu_platform::Gpio::State;
		
		GpoAllocator(
			int gpio_id,
			const TaskIdGenerator& task_id_generator,
			const Executor& executor
		);
		GpoAllocator(const GpoAllocator& other);
		GpoAllocator& operator=(const GpoAllocator& other) = delete;
		
		virtual ~GpoAllocator() noexcept;

		int set_task_id(const GpioState& to_state) const;
	private:
		int m_gpio_id;
		const std::unique_ptr<Executor> m_executor;
		int m_set_high_id;
		int m_set_low_id;

		int allocate_set_task(const GpioState& to_state) const;
		void deallocate_task(int task_id) const;
	};

	inline GpoAllocator::GpoAllocator(
		int gpio_id,
		const TaskIdGenerator& task_id_generator,
		const Executor& executor
	): m_gpio_id(gpio_id), m_executor(executor.clone()) {
		
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;
		using GpioDirection = mcu_platform::Gpio::Direction;
		
		Object create_gpio_task;
		create_gpio_task.add("task_type", Integer(static_cast<int>(TaskType::CREATE_GPIO)));
		create_gpio_task.add("gpio_dir", Integer(static_cast<int>(GpioDirection::OUT)));
		create_gpio_task.add("gpio_id", Integer(m_gpio_id));
		m_executor->execute(create_gpio_task);

		m_set_high_id = allocate_set_task(GpioState::HIGH);
		m_set_low_id = allocate_set_task(GpioState::LOW);
	}

	inline GpoAllocator::~GpoAllocator() noexcept {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;
		
		Object delete_gpio_task;
		delete_gpio_task.add("task_type", Integer(static_cast<int>(TaskType::DELETE_GPIO)));
		delete_gpio_task.add("gpio_id", Integer(m_gpio_id));
		m_executor->execute(delete_gpio_task);

		deallocate_task(m_set_high_id);
		deallocate_task(m_set_low_id);
	}

	inline int GpoAllocator::set_task_id(const GpioState& to_state) const {
		switch (to_state) {
		case GpioState::HIGH:
			return m_set_high_id;
		case GpioState::LOW:
			return m_set_low_id;
		default:
			throw std::invalid_argument("unsupported gpio state received");
		}
	}

	inline int GpoAllocator::allocate_set_task(const GpioState& to_state) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline void GpoAllocator::deallocate_task(int task_id) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // GPO_ALLOCATOR_HPP