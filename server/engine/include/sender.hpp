#ifndef	SENDER_HPP
#define	SENDER_HPP

#include "data.hpp"

namespace engine {

	class Sender {
	public:
		virtual ~Sender() noexcept = default;
		virtual void send(const data::Data& data) const = 0;
	};

}
#endif // SENDER_HPP