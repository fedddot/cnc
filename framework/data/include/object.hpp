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
		class Accessor: public IAccessor {
		public:
			Accessor(const std::string& name);
			virtual inline AccessorType getType() const override;
			inline std::string getName() const;
		private:
			const std::string m_name;
		};

		Object() = default;
		Object& operator=(const Object& other) = default;

		Object(const IData& other);
		Object& operator=(const IData& other);

		virtual IData& access(const IAccessor& where) override;
		virtual const IData& access(const IAccessor& where) const override;
	private:
		typedef std::map<std::string, std::shared_ptr<IData>> UnderlyingClass;

		IData& accessAt(const IAccessor& where);
	}; // Composite

	inline Object::Accessor::AccessorType Object::Accessor::getType() const {
		return AccessorType::NAME;
	}

	inline std::string Object::Accessor::getName() const {
		return m_name;
	}
} // namespace data

#endif // __OBJECT_HPP__