#ifndef	__MCU_EVENTS_HPP__
#define	__MCU_EVENTS_HPP__

#include "dispatcher.hpp"
#include "common_types.hpp"

namespace cnc_server {

	class McuEvents {
	public:
		McuEvents() = default;
		~McuEvents() = default;
		common::Dispatcher<common::Byte> uart_byte_received_dispatcher;
		common::Dispatcher<common::Byte> uart_byte_transmitted_dispatcher;
	};

}

#endif // __MCU_EVENTS_HPP__