#ifndef	__LIST_HPP__
#define	__LIST_HPP__

#include <stddef.h>

#include "iiterable.hpp"

namespace data {
	template <class T>
	class List {
	public:
		List();
		virtual ~List() noexcept = 0;
		virtual size_t size() const = 0;
		virtual T& operator[](const size_t& index) = 0;
		virtual const T& operator[](const size_t& index) const = 0;
		virtual void push_back(const T& value) = 0;
	private:
		class Node {
		public:
			Node(const T& value, Node *next);
			T& getValue();
			const T& getValue() const;
			T& getNext();
			const T& getNext() const;
		private:
			T m_value;
			Node *m_next;
		};
	
		Node *m_root;
		size_t m_size;
	};
}

#endif // __LIST_HPP__