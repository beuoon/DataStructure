#pragma once

#include "DataStructure.hpp"

namespace bon {
	template <typename T>
	class list : DataStructure {
	private:
		class node {
		public:
			node(T element)
				: node(element, nullptr, nullptr) {
			}
			node(T element, node *prev, node *next)
				: element(element) {
				this->prev = prev;
				this->next = next;
			}

			node *prev, *next;
			T element;
		};

	public:
		class iterator {
		public:
			iterator() {

			}
			iterator(node *target) :
				iterator(target, target->prev, target->next) {
			}
			iterator(node *target, node *prev, node *next) {
				this->target = target;
				this->prev = prev;
				this->next = next;
			}
			
			iterator& operator++() {
				target = next;
				setPtrs();
				return *this;
			}
			const iterator operator++(int) {
				iterator result = iterator(target);
				target = next;
				setPtrs();
				return result;
			}
			iterator &operator--() {
				target = prev;
				setPtrs();
				return *this;
			}
			const iterator operator--(int) {
				iterator result = *this;
				target = prev;
				setPtrs();
				return result;
			}

			bool operator==(const iterator &iter) const {
				return this->target == iter.target;
			}
			bool operator!=(const iterator &iter) const {
				return !(*this == iter);
			}

			T &operator*() const {
				return target->element;
			}
			T *operator->() const {
				return &(target->element);
			}

		private:
			inline void setPtrs() {
				if (target != nullptr) {
					prev = target->prev;
					next = target->next;
				}
			}

		public:
			node *prev, *next;
			node *target;
		};

	public:
		list() {
			head = tail = nullptr;
		}

		T &front() const {
			if (empty())
				throw new std::runtime_error("front() called on empty list");
			return head->element;
		}
		T &back() const {
			if (empty())
				throw new std::runtime_error("back() called on empty list");
			return tail->element;
		}

		void push_front(T value) {
			if (empty()) {
				node *newNode = new node(value);
				head = tail = newNode;
			}
			else {
				node *newNode = new node(value, nullptr, head);
				head->prev = newNode;
				head = newNode;
			}

			length++;
		}
		void push_back(T value) {
			if (empty()) {
				node *newNode = new node(value);
				head = tail = newNode;
			}
			else {
				node *newNode = new node(value, tail, nullptr);
				tail->next = newNode;
				tail = newNode;
			}

			length++;
		}
		void pop_front() {
			if (empty())
				throw new std::runtime_error("pop_front() called on empty list");

			node *target = head;
			head = head->next;
			delete target;

			if (head != nullptr)
				head->prev = nullptr;
			else
				tail = nullptr;

			length--;
		}
		void pop_back() {
			if (empty())
				throw new std::runtime_error("pop_back() called on empty list");

			node *target = tail;
			tail = tail->prev;
			tail->next = nullptr;
			delete target;

			if (tail != nullptr)
				tail->next = nullptr;
			else
				head = nullptr;

			length--;
		}

		iterator begin() {
			return !empty() ? iterator(head) : end();
		}
		iterator end() {
			return iterator(nullptr, tail, nullptr);
		}

		iterator insert() {

		}
		iterator erase(const iterator where) {
			node *target = where.target, *nextNode = target->next;
			iterator nextIter;

			if (target->prev != nullptr)
				target->prev->next = target->next;
			if (target->next != nullptr)
				target->next->prev = target->prev;

			if (target == head)
				head = target->next;
			else if (target == tail)
				tail = target->prev;

			nextIter = nextNode != nullptr ? iterator(nextNode) : end();

			delete target;
			length--;

			return nextIter;
		}
		iterator erase(const iterator first, const iterator last) {
			iterator iter = first;

			while (iter != last)
				iter = erase(iter);

			return iter;
		}

		iterator find(const T &value) {

		}

		void clear() {
			node *curr = head, *next;
			while (curr != nullptr) {
				next = curr->next;
				delete curr;
				curr = next;
			}

			head = tail = nullptr;
			length = 0;
		}

	private:
		node *head, *tail;
	};
}