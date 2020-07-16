#pragma once

#include "list.hpp"

namespace bon {
	template <typename T>
	class queue {
	public:
		const T &front() {
			return container.front();
		}
		const T &back() {
			return container.back();
		}

		void push(const T& value) {
			container.push_back(value);
		}
		void pop() {
			container.pop_front();
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

