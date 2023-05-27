#ifndef	__LIST_NODE_HPP__
#define	__LIST_NODE_HPP__

#include <stddef.h>
#include "list.hpp"

namespace data {
	
	template <class T>
	List<T>::Node::Node(): m_value(nullptr), m_next(nullptr) {
	}

	template <class T>
	List<T>::Node::Node(const T& value, Node *next): m_value(new T(value)), m_next(next) {
	}

	template <class T>
	List<T>::Node::Node(const Node& other): m_value(nullptr), m_next(other.m_next) {
		if (nullptr != other.m_value) {
			m_value = new T(*(other.m_value));
		}
	}

	template <class T>
	typename List<T>::Node& List<T>::Node::operator=(const Node& other) {
		T *updated_value = nullptr;
		if (nullptr != other.m_value) {
			updated_value = new T(*(other.m_value));
		}
		delete m_value;
		m_value = updated_value;
		m_next = other.m_next;
		return *this;
	}
	
	template <class T>
	List<T>::Node::~Node() noexcept {
		delete m_value;
		m_value = nullptr;
		m_next = nullptr;
	}

} // namespace data

#endif // __LIST_NODE_HPP__