#ifndef	FACTORY_HPP
#define	FACTORY_HPP

namespace cnc_engine {
	template <class Tprod, class Tconf>
	class Factory {
	public:
		virtual ~Factory() noexcept = 0;
		virtual Tprod create(const Tconf& cfg) const = 0;
	};

	template <class Tprod, class Tconf>
	Factory<Tprod, Tconf>::~Factory() noexcept {

	}
}
#endif // FACTORY_HPP