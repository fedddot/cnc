#ifndef	DATA_HPP
#define	DATA_HPP

namespace cnc_engine {
	class Data {
	public:
		enum class Type : int {
			INT,
			STR,
			OBJECT
		};
		virtual inline ~Data() noexcept = 0;
		virtual Type type() const = 0;
		virtual Data *copy() const = 0;
	};

	inline Data::~Data() noexcept {

	}
}
#endif // DATA_HPP