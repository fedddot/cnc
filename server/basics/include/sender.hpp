#ifndef	SENDER_HPP
#define	SENDER_HPP

namespace basics {
	template <class Tdata>
	class Sender {
	public:
		virtual ~Sender() noexcept = default;
		virtual void send(const Tdata& data) const = 0;
	};

}
#endif // SENDER_HPP