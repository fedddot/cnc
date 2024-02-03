#ifndef	TASK_HPP
#define	TASK_HPP

#include "data.hpp"

namespace engine {
	class Task {
	public:
		virtual ~Task() noexcept = default;
		virtual void execute() = 0;
		virtual const data::Data& report() const = 0;
	};
}

#endif // TASK_HPP