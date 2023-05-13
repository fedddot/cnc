#ifndef	__ISTRING_HPP__
#define	__ISTRING_HPP__

namespace common {
	class IString {
	public:
		virtual ~IString() noexcept = 0;
		virtual void execute() = 0;
	};
}


#endif // __ISTRING_HPP__