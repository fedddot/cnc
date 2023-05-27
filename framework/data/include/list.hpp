#ifndef	__LIST_HPP__
#define	__LIST_HPP__

#include <stddef.h>

namespace data {
	template <class T>
	class List {
	private:
		class Node;
	public:
		class Iter {
		public:
			Iter(Node *node);
			~Iter() noexcept = default;
			Iter(const Iter& other) = default;
			Iter& operator=(const Iter& other) = default;

			inline T& get();
			inline const T& get() const;
			inline bool operator==(const Iter& other) const;
			inline bool operator!=(const Iter& other) const;
			inline Iter& operator++();
			inline bool isEndIter() const;
			inline bool isLastIter() const;
			inline Iter insert_before(const T& value);
			inline void erase();
		private:
			Node *m_node;
		};

		List();
		List(const List& other);
		virtual List& operator=(const List& other);
		virtual ~List() noexcept;

		inline virtual Iter begin();
		inline virtual Iter last();
		inline virtual Iter end();

		inline virtual void clear();
		inline virtual void push_front(const T& value);
		inline virtual T pop_front();
		inline virtual void push_back(const T& value);
		inline virtual T pop_back();
		inline virtual size_t size() const;
		inline virtual bool empty() const;

		inline bool operator==(const List& other) const;

	private:
		class Node {
		public:
			Node(); // Dummy node Ctor
			Node(const T& value, Node *next = nullptr); // Data node Ctor
			Node(const Node& other);
			Node& operator=(const Node& other);
			~Node() noexcept;
			T *m_value;
			Node *m_next;
		};

		Node *m_head;
	}; // List

	template <class T>
	List<T>::List(): m_head(new Node) {
	}

	template <class T>
	List<T>::List(const List& other): m_head(new Node) {
		auto other_casted = const_cast<List *>(&other);
		auto iter = other_casted->begin();
		auto iter_end = other_casted->end();
		while (iter != iter_end) {
			push_back(iter.get());
			++iter;
		}
	}

	template <class T>
	typename List<T>::List& List<T>::operator=(const List& other) {
		clear();
		auto other_casted = const_cast<List *>(&other);
		auto iter = other_casted->begin();
		auto iter_end = other_casted->end();
		while (iter != iter_end) {
			push_back(iter.get());
			++iter;
		}
		return *this;
	}

	template <class T>
	List<T>::~List() noexcept {
		clear();
		delete m_head;
	}

	template <class T>
	inline void List<T>::clear() {
		while (!empty()) {
			auto iter = begin();
			iter.erase();
		}
	}

	template <class T>
	inline typename List<T>::Iter List<T>::begin() {
		return Iter(m_head);
	}

	template <class T>
	inline typename List<T>::Iter List<T>::end() {
		auto iter = begin();
		while (!(iter.isEndIter())) {
			++iter;
		}
		return iter;
	}

	template <class T>
	inline typename List<T>::Iter List<T>::last() {
		if (empty()) {
			// handle it
		}
		auto iter = begin();
		while (!(iter.isLastIter())) {
			++iter;
		}
		return iter;
	}

	template <class T>
	void List<T>::push_front(const T& value) {
		auto iter = begin();
		iter.insert_before(value);
	}

	template <class T>
	T List<T>::pop_front() {
		if (empty()) {
			// TODO: handle it
		}
		auto iter = begin();
		T value(iter.get());
		iter.erase();
		return value;
	}

	template <class T>
	void List<T>::push_back(const T& value) {
		auto iter = end();
		iter.insert_before(value);
	}

	template <class T>
	T List<T>::pop_back() {
		if (empty()) {
			// TODO: handle it
		}
		auto iter = last();
		T value(iter.get());
		iter.erase();
		return value;
	}

	template <class T>
	size_t List<T>::size() const {
		size_t list_size = 0;
		auto iter = const_cast<List<T> *>(this)->begin();
		auto iter_end = const_cast<List<T> *>(this)->end();
		while (iter != iter_end) {
			++list_size;
			++iter;
		}
		return list_size;
	}

	template <class T>
	bool List<T>::empty() const {
		auto iter = const_cast<List<T> *>(this)->begin();
		return iter.isEndIter();
	}

	template <class T>
	inline bool List<T>::operator==(const List& other) const {
		if (size() != other.size()) {
			return false;
		}
		auto this_iter = const_cast<List&>(*this).begin();
		auto other_iter = const_cast<List&>(other).begin();
		while (!(this_iter.isEndIter()) && !(other_iter.isEndIter())) {
			if (this_iter.get() != other_iter.get()) {
				return false;
			}
			++this_iter;
			++other_iter;
		}
		return true;
	}
} // namespace data

#include "list_node.hpp"
#include "list_iter.hpp"

#endif // __LIST_HPP__