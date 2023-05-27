#ifndef	__SHARED_PTR_HPP__
#define	__SHARED_PTR_HPP__

#include <stddef.h>

namespace data {
	template <class T>
	class SharedPtr {
	public:
		SharedPtr(T *object_ptr = nullptr);
		SharedPtr(const SharedPtr<T>& other);
		SharedPtr<T>& operator=(const SharedPtr<T>& other);
		~SharedPtr() noexcept;
		
		T *get();
		const T *get() const;
	private:
		T *m_object_ptr;
		size_t *m_reference_counter;
	}; // SharedPtr

	template <class T>
	SharedPtr<T>::SharedPtr(T *object_ptr): m_object_ptr(object_ptr), m_reference_counter(new size_t(1UL)) {

	}

	template <class T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T>& other): m_object_ptr(other.m_object_ptr), m_reference_counter(other.m_reference_counter) {
		++(*m_reference_counter);
	}

	template <class T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
		T *old_obj_ptr = m_object_ptr;
		size_t *old_ref_cntr = m_reference_counter;

		m_object_ptr = other.m_object_ptr;
		m_reference_counter = other.m_reference_counter;
		++(*m_reference_counter);

		--(*old_ref_cntr);
		if (0 == *old_ref_cntr) {
			delete old_obj_ptr;
			delete old_ref_cntr;
		}

		return *this;
	}

	template <class T>
	SharedPtr<T>::~SharedPtr() noexcept {
		--(*m_reference_counter);
		if (0 == *m_reference_counter) {
			delete m_object_ptr;
			delete m_reference_counter;
		}
	}
	
	template <class T>
	T *SharedPtr<T>::get() {
		return m_object_ptr;
	}

	template <class T>
	const T *SharedPtr<T>::get() const {
		return m_object_ptr;
	}
}

#endif // __SHARED_PTR_HPP__