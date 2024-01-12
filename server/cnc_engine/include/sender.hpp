#ifndef	SENDER_HPP
#define	SENDER_HPP

namespace cnc_engine {
	template <class Tdata>
	class Sender {
	public:
		virtual ~Sender() noexcept = 0;
		virtual void send(const Tdata& data) const = 0;
	};

	template <class Tdata>
	Sender<Tdata>::~Sender() noexcept {

	}
}
#endif // SENDER_HPP