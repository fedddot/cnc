#ifndef	INVENTORY_HPP
#define	INVENTORY_HPP

#include <map>
#include <stdexcept>

namespace inventory {
	template <class Tkey, class Titem>
	class Inventory {
	public:
		Inventory() = default;
		Inventory(const Inventory& other) = delete;
		Inventory& operator=(const Inventory& other) = delete;
		
		inline ~Inventory() noexcept;

		inline void put(const Tkey& key, Titem *item);
		inline Titem *get(const Tkey& key);
		inline bool contains(const Tkey& key) const;
	private:
		std::map<Tkey, Titem *> m_items;
	};

	template <class Tkey, class Titem>
	inline Inventory<Tkey, Titem>::~Inventory() noexcept {
		try {
			for (auto iter: m_items) {
				delete iter->second;
			}
			m_items.clear();
		} catch (...) {

		}
	}

	template <class Tkey, class Titem>
	inline void Inventory<Tkey, Titem>::put(const Tkey& key, Titem *item) {
		if (m_items.end() != m_items.find(key)) {
			throw std::invalid_argument("received key is already registered");
		}
		if (nullptr == item) {
			throw std::invalid_argument("invalid item pointer received");
		}
		m_items.insert({ key, item });
	}

	template <class Tkey, class Titem>
	inline Titem *Inventory<Tkey, Titem>::get(const Tkey& key) {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		Titem *item = iter->second;
		m_items.erase(iter);
		return item;
	}

	template <class Tkey, class Titem>
	inline bool Inventory<Tkey, Titem>::contains(const Tkey& key) const {
		return m_items.end() != m_items.find(key);
	}
}
#endif // INVENTORY_HPP