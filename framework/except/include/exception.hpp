#ifndef	__EXCEPTION_HPP__
#define	__EXCEPTION_HPP__

namespace except {
	class Exception {
	public:
		enum class ExceptionType: int {
			BAD_ALLOC,
			BAD_ARGUMENT,
			OTHER
		};
		Exception(const ExceptionType& what);
		virtual ~Exception() noexcept = default;
		virtual ExceptionType what() const;
	private:
		ExceptionType m_what;
	}; // class Exception
} // namespace except


#endif // __EXCEPTION_HPP__