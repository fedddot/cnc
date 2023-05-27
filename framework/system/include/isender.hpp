#ifndef	__ISENDER_HPP__
#define	__ISENDER_HPP__

#include "list.hpp"

namespace system {
	class ISender {
	public:
		virtual ~ISender() noexcept = 0;
		virtual void send(const data::List<char>& message) = 0;
	}; // ISender
} // common

#endif // __ISENDER_HPP__