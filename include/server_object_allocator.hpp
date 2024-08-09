#ifndef	SERVER_OBJECT_ALLOCATOR_HPP
#define	SERVER_OBJECT_ALLOCATOR_HPP

namespace cnc {
	template <typename Tdata>
	class ServerObjectAllocator {
	public:
		virtual ~ServerObjectAllocator() noexcept = default;
		virtual Tdata create_data() const = 0;
		virtual Tdata delete_data() const = 0;
		virtual ServerObjectAllocator *clone() const = 0;
	};
}

#endif // SERVER_OBJECT_ALLOCATOR_HPP