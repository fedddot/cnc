#ifndef	__LIST_HPP__
#define	__LIST_HPP__

#include <stddef.h>

namespace data {
	template <class T>
	class List {
	public:
		List();
		List(const List<T>& other) = delete;
		List<T>& operator=(const List<T>& other) = delete;
		virtual ~List() noexcept;

		virtual void push_front(const T& value);
		virtual T pop_front();
		virtual void push_back(const T& value);
		virtual T pop_back();
		virtual size_t size() const;
	private:
		class Node {
		public:
			Node(const T& value, Node *next);
			Node(const Node& other) = delete;
			Node& operator=(const Node& other) = delete;
			~Node() noexcept = default;
			T& getValue();
			const T& getValue() const;
			Node *getNext();
			const Node *getNext() const;
		private:
			T m_value;
			Node *m_next;
		};
	
		Node *m_root;
	}; // List

	template <class T>
	List<T>::List(): m_root(nullptr) {

	}

	template <class T>
	List<T>::~List() noexcept {
		Node *curr = m_root;
		while (nullptr != curr) {
			Node *next = curr->getNext();
			delete curr;
			curr = next;
		}
	}

	template <class T>
	void List<T>::push_front(const T& value) {
		Node *new_root = new Node(value, m_root);
		m_root = new_root;
	}

	template <class T>
	T List<T>::pop_front() {
		Node *new_root = m_root->getNext();
		T value = m_root->getValue();
		delete m_root;
		m_root = new_root;
		return value;
	}

	// template <class T>
	// void List<T>::push_back(const T& value) {

	// }

	// template <class T>
	// T List<T>::pop_back() {

	// }

	template <class T>
	size_t List<T>::size() const {
		size_t num_of_nodes = 0;
		auto curr = m_root;
		while (nullptr != curr) {
			++num_of_nodes;
			curr = curr->getNext();
		}
		return num_of_nodes;
	}

	template <class T>
	List<T>::Node::Node(const T& value, Node *next): m_value(value), m_next(next) {
	
	}

	template <class T>
	T& List<T>::Node::getValue() {
		return m_value;
	}

	template <class T>
	const T& List<T>::Node::getValue() const {
		return m_value;
	}

	template <class T>		
	List<T>::Node *List<T>::Node::getNext() {
		return m_next;
	}

	template <class T>
	const List<T>::Node *List<T>::Node::getNext() const {
		return m_next;
	}

} // namespace data

#endif // __LIST_HPP__