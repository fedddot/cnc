#ifndef	TASK_HPP
#define	TASK_HPP

namespace cnc_engine {
	template <class Treport>
	class Task {
	public:
		virtual ~Task() noexcept = 0;
		virtual Treport execute() = 0;
	};

	template <class Treport>
	Task<Treport>::~Task() noexcept {

	}
}

#endif // TASK_HPP