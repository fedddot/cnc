#ifndef	__FEATURES_HPP__
#define	__FEATURES_HPP__

#include <stddef.h>

extern void *operator new(size_t size);
extern void operator delete(void *ptr) noexcept;

#endif // __FEATURES_HPP__