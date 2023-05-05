#ifndef	__MESSAGE_HPP__
#define	__MESSAGE_HPP__

#include <cstddef>
#include <string>

namespace message {
	class Message {
	public:
		Message(const std::string& msg);
		virtual ~Message() noexcept = default;
		virtual const char *data() const;
		virtual std::size_t length() const;
	private:
		std::string m_msg;
	}; // Message

} // message

#endif // __MESSAGE_HPP__