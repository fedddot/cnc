#ifndef	INTEGER_HPP
#define	INTEGER_HPP

#include "data.hpp"
#include "value.hpp"

namespace data {
	class Integer: public Value<int> {
	public:
		inline Integer(int val);
		Integer(const Integer& other) = default;
		Integer& operator=(const Integer& other) = default;
		inline Type type() const override;
		virtual inline Data *copy() const override;
	};

	inline Integer::Integer(int val): Value<int>(val) {

	}

	inline Integer::Type Integer::type() const {
		return Type::INT;
	}

	inline Data *Integer::copy() const {
		return new Integer(*this);
	}
}

#endif // INTEGER_HPP