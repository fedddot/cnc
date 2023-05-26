#ifndef	__EXCEPTION_HPP__
#define	__EXCEPTION_HPP__

#include "string.hpp"

namespace except {

	class Exception {
	public:
		Exception(const data::String& what = "");
		virtual ~Exception() noexcept = default;
		virtual data::String what() const;
	private:
		data::String m_what;
	}; // class Exception
} // namespace except


#endif // __EXCEPTION_HPP__