#pragma once

#include "AVLTree.hpp"

namespace bon {
	template <typename T, class Compare = std::less<T>>
	class set : public AVLTree<T, Compare> {
	public:
		using Tree = AVLTree<T, Compare>;

		set() : Tree(ALLOW_DUPLICATION) {
		}

	private:
		static const bool ALLOW_DUPLICATION = false;
	};
}