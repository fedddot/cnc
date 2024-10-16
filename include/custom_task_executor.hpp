#ifndef	CUSTOM_TASK_EXECUTOR_HPP
#define	CUSTOM_TASK_EXECUTOR_HPP

#include <functional>
#include <stdexcept>

#include "task_executor.hpp"

namespace cnc {
	template <typename Signature>
	class CustomTaskExecutor;

	template <typename Tdata, typename... Args>
	class CustomTaskExecutor<Tdata(Args...)>: public cnc::TaskExecutor<Tdata(Args...)> {
	public:
		using ExecuteAction = std::function<Tdata(Args...)>;
		CustomTaskExecutor(const ExecuteAction& action);
		CustomTaskExecutor(const CustomTaskExecutor& other) = default;
		CustomTaskExecutor& operator=(const CustomTaskExecutor& other) = default;
		
		Tdata execute(Args... args) override;
		cnc::TaskExecutor<Tdata(Args...)> *clone() const override;
	private:
		ExecuteAction m_action;
	};

	template <typename Tdata, typename... Args>
	inline CustomTaskExecutor<Tdata(Args...)>::CustomTaskExecutor(const ExecuteAction& action): m_action(action) {
		if (!m_action) {
			throw std::invalid_argument("invalid action received");
		}
	}

	template <typename Tdata, typename... Args>
	inline Tdata CustomTaskExecutor<Tdata(Args...)>::execute(Args... args) {
		return m_action(std::forward<Args>(args)...);
	}

	template <typename Tdata, typename... Args>
	inline cnc::TaskExecutor<Tdata(Args...)> *CustomTaskExecutor<Tdata(Args...)>::clone() const {
		return new CustomTaskExecutor(*this);
	}
}

#endif // CUSTOM_TASK_EXECUTOR_HPP