#ifndef	__ICNC_TASK_HPP__
#define	__ICNC_TASK_HPP__

#include <string>
#include "itask.hpp"

namespace common {
	class ICncTask: public ITask {
	public:
		enum class Priority: Byte {
			HIGH = 0,
			WORKFLOW = 1,
			LOW = 2
		};

		enum class Type: Byte {
			MOVEMENT = 0,
			GET_CONFIG = 1,
			SET_CONFIG = 2
		};

		virtual Priority getPriority() const = 0;
		virtual Type getType() const = 0;
		virtual std::string serialize() const = 0;
	};
}

#endif // __ICNC_TASK_HPP__