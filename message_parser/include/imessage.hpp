#ifndef	__IMESSAGE_HPP__
#define	__IMESSAGE_HPP__

#include <cstddef>

namespace message
{
	class IMessage {
	public:
		virtual ~IMessage() noexcept = 0;
		virtual std::size_t getLength() const = 0;
		virtual const char *getData() const = 0;
	}; // IMessage

} // message

#endif // __IMESSAGE_HPP__