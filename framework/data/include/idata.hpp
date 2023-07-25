#ifndef	__IDATA_HPP__
#define	__IDATA_HPP__

namespace data {
	class IData {
	public:
		enum class DataType : int {
			COMPOSITE,
			VALUE
		};
		virtual DataType getDataType() const = 0;
		virtual ~IData() noexcept = 0;
	}; // IData
} // namespace data

#endif // __IDATA_HPP__