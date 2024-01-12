#ifndef	REPORT_HPP
#define	REPORT_HPP

#include <memory>

#include "data.hpp"

namespace cnc_engine {
	class Report {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE
		};
		inline Report(const Result& result, const Data& data);
		inline Report(const Report& other);
		inline Report& operator=(const Report& other);
		~Report() noexcept = default;
		inline virtual Result result() const;
		inline virtual const Data& data() const;
	private:
		Result m_result;
		std::unique_ptr<Data> m_data;
	};

	inline Report::Report(const Result& result, const Data& data): m_result(result), m_data(data.copy()) {

	}

	inline Report::Report(const Report& other): m_result(other.m_result), m_data(other.m_data->copy()) {

	}

	inline Report& Report::operator=(const Report& other) {
		m_result = other.m_result;
		m_data = std::unique_ptr<Data>(other.m_data->copy());
		return *this;
	}

	inline Report::Result Report::result() const {
		return m_result;
	}

	inline const Data& Report::data() const {
		return std::ref(*m_data);
	}
}

#endif // REPORT_HPP