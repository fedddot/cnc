#ifndef	__IPARSABLE_HPP__
#define	__IPARSABLE_HPP__

namespace common
{
	class IParsable {
	public:
		virtual ~IParsable() noexcept = 0;
		virtual const char *parse(const char * const from) = 0;
	}; // IParsable
} // common

#endif // __IPARSABLE_HPP__