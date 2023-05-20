#ifndef	__LIST_ITER_HPP__
#define	__LIST_ITER_HPP__

#include <stddef.h>
#include "list.hpp"

namespace data {
	template <class T>
	List<T>::Iter::Iter(Node *node): m_node(node) {
		if (nullptr == m_node) {
			// TODO: handle it
		}
	}

	template <class T>
	inline T& List<T>::Iter::get() {
		if (isEndIter()) {
			// TODO: handle it
		}
		return *(m_node->m_value);
	}

	template <class T>
	inline const T& List<T>::Iter::get() const {
		if (m_node->isEndNode()) {
			// TODO: handle it
		}
		return *(m_node->m_value);
	}

	template <class T>
	inline bool List<T>::Iter::operator==(const Iter& other) const {
		return (m_node == other.m_node);
	}

	template <class T>
	inline bool List<T>::Iter::operator!=(const Iter& other) const {
		return (m_node != other.m_node);
	}
	
	template <class T>
	inline typename List<T>::Iter& List<T>::Iter::operator++() {
		if (isEndIter()) {
			// TODO: handle it
		}
		m_node = m_node->m_next;
		return *this;
	}

	template <class T>
	bool List<T>::Iter::isEndIter() const {
		return (nullptr == m_node->m_next);
	}

	template <class T>
	bool List<T>::Iter::isLastIter() const {
		if (isEndIter()) {
			return false;
		}
		return (nullptr == m_node->m_next->m_next);
	}

	template <class T>
	inline typename List<T>::Iter List<T>::Iter::insert_before(const T& value) {
		Node *next_node = new Node(*(m_node));
		delete (m_node->m_value);
		m_node->m_value = new T(value);
		m_node->m_next = next_node;
		return *this;
	}

	template <class T>
	inline void List<T>::Iter::erase() {
		if (isEndIter()) {
			// TODO: handle it
		}
		Node *next_node = m_node->m_next;
		*(m_node) = *next_node;
		delete next_node;
		next_node = nullptr;
	}
} // namespace data

#endif // __LIST_ITER_HPP__