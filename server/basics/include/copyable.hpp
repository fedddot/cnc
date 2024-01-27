#ifndef	COPYABLE_HPP
#define	COPYABLE_HPP

namespace basics {
	template <class T>
	class Copyable {
	public:
		virtual ~Copyable() noexcept = default;
		virtual T *copy() const = 0;
	};
}

#endif // COPYABLE_HPP