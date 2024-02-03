#ifndef	TASK_HPP
#define	TASK_HPP

#include "data.hpp"
namespace basics {
	class Task {
	public:
		virtual ~Task() noexcept = default;
		virtual void execute() = 0;
		virtual data::Data *report() const = 0;
	};
}

#endif // TASK_HPP