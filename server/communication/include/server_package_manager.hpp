#ifndef	__SERVER_PACKAGE_MANAGER_HPP__
#define	__SERVER_PACKAGE_MANAGER_HPP__

#include "package_manager.hpp"
#include "package_descriptor.hpp"

namespace communication {
	class ServerPackageManager: public PackageManager {
	public:
		ServerPackageManager(const PackageDescriptor& package_descriptor);
		ServerPackageManager(const ServerPackageManager& other) = delete;
		ServerPackageManager& operator=(const ServerPackageManager& other) = delete;
	private:

	}; // ServerPackageManager
} // namespace communication
#endif // __SERVER_PACKAGE_MANAGER_HPP__