#ifndef	TASK_HPP
#define	TASK_HPP

namespace basics {
	class Task {
	public:
		virtual ~Task() noexcept = default;
		virtual void execute() = 0;
	};
}

#endif // TASK_HPP