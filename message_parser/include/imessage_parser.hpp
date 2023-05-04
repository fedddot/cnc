#ifndef	__IMESSAGE_PARSER_HPP__
#define	__IMESSAGE_PARSER_HPP__

#include <memory>
#include "ilistener.hpp"
#include "imessage.hpp"

namespace message
{

	class IMessageParser: public common::IListener<const char&>
	{
	public:
		virtual ~IMessageParser() noexcept = 0;
		virtual void setMessageListener(common::IListener<const IMessage&> *message_listener_ptr) = 0;
	}; // IMessageParser

} // message

#endif // __IMESSAGE_PARSER_HPP__