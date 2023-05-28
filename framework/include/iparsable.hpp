#ifndef	__IPARSABLE_HPP__
#define	__IPARSABLE_HPP__

#include <vector>

namespace common
{
	class IParsable {
	public:
		virtual ~IParsable() noexcept = 0;
		virtual std::vector<char>::iterator parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) = 0;
	}; // IParsable
} // common

#endif // __IPARSABLE_HPP__