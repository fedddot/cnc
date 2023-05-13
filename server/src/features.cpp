#include <stdlib.h>
#include <stddef.h>

#include "features.hpp"

void *operator new(size_t size) {
	void *dataPtr = malloc(size);
	if (nullptr == dataPtr) {
		// TODO: report a problem into the log
	}
	return dataPtr;
}

void operator delete(void *ptr) noexcept {
	if (nullptr != ptr) {
		free(ptr);
	}
}