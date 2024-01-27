#ifndef	INVENTORY_HPP
#define	INVENTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

namespace inventory {
	template <class Tkey, class Titem>
	class Inventory {
	public:
		Inventory() = default;
		Inventory(const Inventory& other) = delete;
		Inventory& operator=(const Inventory& other) = delete;
		
		~Inventory() noexcept = default;

		void put(const Tkey& key, const std::shared_ptr<Titem>& item);
		Titem *get(const Tkey& key);
		void remove(const Tkey& key);
		bool contains(const Tkey& key) const;
	private:
		std::map<Tkey, std::shared_ptr<Titem>> m_items;
	};

	template <class Tkey, class Titem>
	inline void Inventory<Tkey, Titem>::put(const Tkey& key, const std::shared_ptr<Titem>& item) {
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
		return (iter->second).get();
	}

	template <class Tkey, class Titem>
	void Inventory<Tkey, Titem>::remove(const Tkey& key) {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		m_items.erase(iter);
	}

	template <class Tkey, class Titem>
	inline bool Inventory<Tkey, Titem>::contains(const Tkey& key) const {
		return m_items.end() != m_items.find(key);
	}
}
#endif // INVENTORY_HPP