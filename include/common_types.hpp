#ifndef	__COMMON_TYPES_HPP__
#define	__COMMON_TYPES_HPP__

#include <cstdint>
#include <vector>

namespace common {
	typedef std::uint8_t Byte;
	enum {BITS_IN_BYTE = 8};

	typedef std::vector<Byte> Message;
	typedef std::vector<Byte> Signature;
} // common

#endif // __COMMON_TYPES_HPP__