#ifndef	__IPARSABLE_HPP__
#define	__IPARSABLE_HPP__

#include "list.hpp"

namespace common
{
	class IParsable {
	public:
		virtual ~IParsable() noexcept = 0;
		virtual data::List<char>::Iter parse(const data::List<char>::Iter& start) = 0;
	}; // IParsable
} // common

#endif // __IPARSABLE_HPP__