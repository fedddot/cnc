#ifndef	__IDATA_SENDER_HPP__
#define	__IDATA_SENDER_HPP__

namespace data {
	template <class T>
	class IDataSender {
	public:
		virtual ~IDataSender() noexcept = 0;
		virtual void send(const T& data) = 0;
	}; // IDataSender

	template <class T>
	IDataSender<T>::~IDataSender() noexcept {
		
	}
} // namespace data
#endif // __IDATA_SENDER_HPP__