#ifndef	__SERVER_RECEIVER_HPP__
#define	__SERVER_RECEIVER_HPP__

#include <vector>
#include <cstddef>

#include "byte_receiver.hpp"

namespace communication {
	class ServerDataReceiver: public ByteReceiver {
	public:
		ServerDataReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size);
		ServerDataReceiver(const ServerDataReceiver& other) = delete;
		ServerDataReceiver& operator=(const ServerDataReceiver& other) = delete;
		virtual ~ServerDataReceiver() noexcept override;
	private:

	}; // ServerDataReceiver
} // namespace communication
#endif // __SERVER_RECEIVER_HPP__