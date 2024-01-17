#ifndef	FACTORY_HPP
#define	FACTORY_HPP

namespace cnc_engine {
	template <class Tprod, class Tconf>
	class Factory {
	public:
		virtual ~Factory() noexcept = default;
		virtual Tprod create(const Tconf& cfg) const = 0;
	};
}
#endif // FACTORY_HPP