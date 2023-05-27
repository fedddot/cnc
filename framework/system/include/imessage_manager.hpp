#ifndef	__IMESSAGE_MANAGER_HPP__
#define	__IMESSAGE_MANAGER_HPP__

#include "list.hpp"

namespace system {
	class IMessageManager {
	public:
		virtual ~IMessageManager() noexcept = 0;
	}; // class IMessageManager
} // namespace system

#endif // __IMESSAGE_MANAGER_HPP__