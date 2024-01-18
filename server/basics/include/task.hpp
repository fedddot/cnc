#ifndef	TASK_HPP
#define	TASK_HPP

namespace basics {
	template <class Treport>
	class Task {
	public:
		virtual ~Task() noexcept = default;
		virtual Treport execute() = 0;
	};

}

#endif // TASK_HPP