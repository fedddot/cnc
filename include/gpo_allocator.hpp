#ifndef	GPO_ALLOCATOR_HPP
#define	GPO_ALLOCATOR_HPP

#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "server_object_allocator.hpp"
#include "stepper_motor_types.hpp"

namespace cnc {
	class GpoAllocator: ServerObjectAllocator<mcu_server::Object> {
	public:
		GpoAllocator(GpoId gpio_id);
		GpoAllocator(const GpoAllocator& other);
		GpoAllocator& operator=(const GpoAllocator& other) = delete;
		mcu_server::Object create_data() const override;
		mcu_server::Object delete_data() const override;
		mcu_server::Object set_data(const GpioState& state) const;
		ServerObjectAllocator<mcu_server::Object> *clone() const override;
	private:
		GpoId m_gpio_id;
	};

	inline GpoAllocator::GpoAllocator(GpoId gpio_id): m_gpio_id(gpio_id) {

	}

	inline mcu_server::Object GpoAllocator::create_data() const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::CREATE_GPIO)));
		task_data.add("gpio_id", Integer(m_gpio_id));
		task_data.add("gpio_dir", Integer(static_cast<int>(mcu_platform::Gpio::Direction::OUT)));
		return task_data;
	}

	inline mcu_server::Object GpoAllocator::delete_data() const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::DELETE_GPIO)));
		task_data.add("gpio_id", Integer(m_gpio_id));
		return task_data;
	}

	inline mcu_server::Object GpoAllocator::set_data(const GpioState& state) const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<GpoId, int>::TaskType;
		Object task_data;
		task_data.add("task_type", Integer(static_cast<int>(TaskType::SET_GPIO)));
		task_data.add("gpio_id", Integer(m_gpio_id));
		task_data.add("gpio_state", Integer(static_cast<int>(state)));
		return task_data;
	}

	inline ServerObjectAllocator<mcu_server::Object> *GpoAllocator::clone() const {
		return new GpoAllocator(*this);
	}
}

#endif // GPO_ALLOCATOR_HPP