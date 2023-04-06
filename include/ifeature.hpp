#ifndef	__IFEATURE_HPP__
#define	__IFEATURE_HPP__

#include <string>

namespace common {
	class IFeature {
	public:
		virtual ~IFeature() noexcept = 0;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual bool isEnabled() const = 0;
		virtual const std::string& getId() const = 0;
	}; // IFeature

	IFeature::~IFeature() noexcept {
		
	}
	
} // common

#endif // __IFEATURE_HPP__