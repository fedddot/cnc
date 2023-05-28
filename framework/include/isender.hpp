#ifndef	__ISENDER_HPP__
#define	__ISENDER_HPP__

#include <vector>

namespace common {
	class ISender {
	public:
		virtual ~ISender() noexcept = 0;
		virtual void send(const std::vector<char>& message) = 0;
	}; // class ISender
} // namespace common

#endif // __ISENDER_HPP__