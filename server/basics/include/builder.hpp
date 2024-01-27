#ifndef	BUILDER_HPP
#define	BUILDER_HPP

namespace basics {
	template <class Tprod>
	class Builder {
	public:
		virtual ~Builder() noexcept = default;
		virtual Tprod build() const = 0;
	};
}

#endif // BUILDER_HPP