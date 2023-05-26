#ifndef	__PAIR_HPP__
#define	__PAIR_HPP__

#include <stddef.h>

namespace data {
	template <class Tfirst, class Tsecond>
	class Pair {
	public:
		Pair(const Tfirst& first, const Tsecond& second);
		Pair(const Pair& other) = default;
		Pair& operator=(const Pair& other) = default;
		virtual ~Pair() noexcept = default;
		virtual inline Tfirst& first();
		virtual inline const Tfirst& first() const;
		virtual inline Tsecond& second();
		virtual inline const Tsecond& second() const;
	private:
		Tfirst m_first;
		Tsecond m_second;
	}; // Pair

	template <class Tfirst, class Tsecond>
	Pair<Tfirst, Tsecond>::Pair(const Tfirst& first, const Tsecond& second): m_first(first), m_second(second) {

	}

	template <class Tfirst, class Tsecond>
	inline Tfirst& Pair<Tfirst, Tsecond>::first() {
		return m_first;
	}

	template <class Tfirst, class Tsecond>
	inline const Tfirst& Pair<Tfirst, Tsecond>::first() const {
		return m_first;
	}

	template <class Tfirst, class Tsecond>
	inline Tsecond& Pair<Tfirst, Tsecond>::second() {
		return m_second;
	}

	template <class Tfirst, class Tsecond>
	inline const Tsecond& Pair<Tfirst, Tsecond>::second() const {
		return m_second;
	}

} // namespace data

#endif // __PAIR_HPP__