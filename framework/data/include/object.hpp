#ifndef	__OBJECT_HPP__
#define	__OBJECT_HPP__

#include <string>
#include <map>
#include <memory>
#include "idata.hpp"
#include "composite.hpp"

namespace data {
	class Object: public Composite, public std::map<std::string, std::shared_ptr<IData>> {
	public:
		Object() = default;
		Object(const Object& other) = default;
		Object& operator=(const Object& other) = default;

		Object(const IData& other);
		Object& operator=(const IData& other);

		virtual inline CompositeType getCompositeType() const override;
	private:
		typedef std::map<std::string, std::shared_ptr<IData>> UnderlyingClass;
	}; // Object
	
	inline Composite::CompositeType Object::getCompositeType() const {
		return CompositeType::OBJECT;
	}

} // namespace data

#endif // __OBJECT_HPP__