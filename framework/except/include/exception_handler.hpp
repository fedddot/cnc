#ifndef	__EXCEPTION_HANDLER_HPP__
#define	__EXCEPTION_HANDLER_HPP__

#include "exception.hpp"
#include "ilistener.hpp"

namespace except {

	class ExceptionHandler: public common::IListener<const Exception&> {
	public:
		virtual void onEvent(const Exception& event) override;
		static ExceptionHandler *getInstance();
		virtual ~ExceptionHandler() override = default;
	private:
		ExceptionHandler() = default;
	}; // class IException
} // namespace except


#endif // __EXCEPTION_HANDLER_HPP__