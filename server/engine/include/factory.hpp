#ifndef	FACTORY_HPP
#define	FACTORY_HPP

#include "data.hpp"
#include "task.hpp"

namespace engine {

	class Factory {
	public:
		virtual ~Factory() noexcept = default;
		virtual Task *create(const data::Data& cfg) const = 0;
	};

}

#endif // FACTORY_HPP