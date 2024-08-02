#ifndef	CUSTOM_TASK_EXECUTOR_HPP
#define	CUSTOM_TASK_EXECUTOR_HPP

#include "task_executor.hpp"
#include <functional>

namespace cnc_utl {
	template <typename Signature>
	class CustomTaskExecutor;

	template <typename Tdata, typename... Args>
	class CustomTaskExecutor<Tdata(Args...)>: public cnc::TaskExecutor<Tdata(Args...)> {
	public:
		using ExecuteAction = std::function<Tdata(Args...)>;
		CustomTaskExecutor(const ExecuteAction& action);
		CustomTaskExecutor(const CustomTaskExecutor& other) = default;
		CustomTaskExecutor& operator=(const CustomTaskExecutor& other) = default;
		
		Tdata execute(Args...) override;
		cnc::TaskExecutor<Tdata(Args...)> *clone() const override;
	private:
		ExecuteAction m_action;
	};
}

#endif // CUSTOM_TASK_EXECUTOR_HPP