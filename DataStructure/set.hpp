#pragma once

#include "AVLTree.hpp"

namespace bon {
	template <typename T, class Compare = std::less<T>>
	class set : public AVLTree<T, Compare> {
	public:
		using Tree = AVLTree<T, Compare>;

		set() : Tree(ALLOW_DUPLICATION) {
		}
		set(std::initializer_list<T> initList) : Tree(initList, ALLOW_DUPLICATION) {
		}
		template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
		set(const _Iter &first, const _Iter &last) : Tree(first, last, ALLOW_DUPLICATION) {
		}
		set(const set &right) : Tree(right.begin(), right.end(), ALLOW_DUPLICATION) {
		}

	private:
		static const bool ALLOW_DUPLICATION = false;
	};
}