#ifndef	__ITASK_HPP__
#define	__ITASK_HPP__

namespace common {

	class ITask {
	public:
		virtual ~ITask() noexcept = 0;
		virtual void execute() = 0;
	};
	ITask::~ITask() noexcept {

	}
}


#endif // __ITASK_HPP__