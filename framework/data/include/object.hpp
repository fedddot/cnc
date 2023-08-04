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
		typedef std::map<std::string, std::shared_ptr<IData>> ContainerType;

		Object() = default;
		Object(const Object& other);
		Object& operator=(const Object& other);

		Object(const IData& other);
		Object& operator=(const IData& other);

		Object(const ContainerType& other);
		Object& operator=(const ContainerType& other);

		virtual inline CompositeType getCompositeType() const override;
	}; // Object
	
	inline Object::CompositeType Object::getCompositeType() const {
		return CompositeType::OBJECT;
	}

} // namespace data

#endif // __OBJECT_HPP__