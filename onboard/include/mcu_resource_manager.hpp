#ifndef	__MCU_RESOURCE_MANAGER_HPP__
#define	__MCU_RESOURCE_MANAGER_HPP__

#include "dispatcher.hpp"

namespace onboard {
	class McuResourceManager {
	public:
		McuResourceManager() = default;
		common::Dispatcher<const char&> uart_byte_received_dispatcher;
		common::Dispatcher<const char&> uart_byte_sent_dispatcher;
	}; // McuResourceManager

} // onboard

#endif // __MCU_RESOURCE_MANAGER_HPP__