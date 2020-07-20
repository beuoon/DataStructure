#pragma once

#include <utility>
#include "AVLTree.hpp"

namespace bon {
	template <typename K, typename V>
	class lessPair {
	public:
		using T = std::pair<K, V>;

		bool operator()(const T& left, const T& right) const {
			return left.first < right.first;
		}
	};

	template <typename K, typename V, class Compare = lessPair<K, V>>
	class map : public AVLTree<std::pair<K, V>, Compare> {
	public:
		using T = std::pair<K, V>;
		using Tree = AVLTree<T, Compare>;

		using iterator = Tree::iterator;

		map() : Tree(ALLOW_DUPLICATION) {
		}
		map(std::initializer_list<T> initList) : Tree(initList.begin(), initList.end()) {
		}
		template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
		map(const _Iter &first, const _Iter &last) : Tree(first, last) {
		}
		map(const map &tree) : Tree(tree) {
		}

		iterator find(const K &value) const {
			return Tree::find(std::make_pair(value, V()));
		}
		iterator lower_bound(const K &value) const {
			return Tree::lower_bound(std::make_pair(value, V()));
		}
		iterator upper_bound(const K &value) const {
			return Tree::upper_bound(std::make_pair(value, V()));
		}

		void erase(const K &value) {
			erase(find(value));
		}
		iterator erase(const iterator where) {
			return Tree::erase(where);
		}
		iterator erase(const iterator &first, const iterator &last) {
			return Tree::erase(first, last);
		}

		const V &at(const K &key) const {
			iterator iter = find(key);
			return iter->second;
		}
		V &operator[](const K &key) const {
			iterator iter = find(key);
			return iter->second;
		}

	private:
		static const bool ALLOW_DUPLICATION = false;
	};
}