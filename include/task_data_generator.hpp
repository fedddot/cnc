#ifndef	TASK_DATA_GENERATOR_HPP
#define	TASK_DATA_GENERATOR_HPP

#include "array.hpp"
#include "gpio.hpp"

namespace cnc {
	template <typename Tdata, typename Tgpio_id>
	class TaskDataGenerator {
	public:
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;

		virtual ~TaskDataGenerator() noexcept = default;
		
		virtual Tdata generate_create_gpio_data(const Tgpio_id& id, const GpioDirection& dir) const = 0;
		virtual Tdata generate_delete_gpio_data(const Tgpio_id& id) const = 0;
		virtual Tdata generate_set_gpio_data(const Tgpio_id& id, const GpioState& state) const = 0;
		virtual Tdata generate_delay_data(const unsigned int delay_ms) const = 0;
		virtual Tdata generate_tasks_data(const mcu_server::Array& tasks) const = 0;
		virtual TaskDataGenerator *clone() const = 0;
	};
}

#endif // TASK_DATA_GENERATOR_HPP