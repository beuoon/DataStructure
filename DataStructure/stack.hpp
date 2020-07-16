#pragma once

#include "list.hpp"

namespace bon {
	template <typename T>
	class stack {
	public:
		const T &top() {
			return container.back();
		}

		void push(const T &value) {
			container.push_back(value);
		}
		void pop() {
			container.pop_back();
		}

		inline bool empty() {
			return container.empty();
		}
		inline int size() {
			return container.size();
		}

	private:
		list<T> container;
	};
}

