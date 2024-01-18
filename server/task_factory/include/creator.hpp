#ifndef	CREATOR_HPP
#define	CREATOR_HPP

namespace task_factory {
	template <class Tprod, class Tconf>
	class Creator {
	public:
		virtual ~Creator() noexcept = default;
		virtual Tprod create(const Tconf& cfg) const = 0;
	};
}

#endif // CREATOR_HPP