#ifndef	PERSISTENT_TASK_ALLOCATOR_HPP
#define	PERSISTENT_TASK_ALLOCATOR_HPP

#include "server_object_allocator.hpp"

namespace cnc {
	template <typename Tdata, typename Tid>
	class PersistentTaskAllocator: public ServerObjectAllocator<Tdata> {
	public:
		virtual Tid id() const = 0;
	};
}

#endif // PERSISTENT_TASK_ALLOCATOR_HPP