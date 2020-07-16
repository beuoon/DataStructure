#pragma once

namespace bon {
	template <typename T>
	class list {
	private:
		class node {
		public:
			node(const T& element)
				: node(element, nullptr, nullptr) {
			}
			node(const T& element, node *prev, node *next)
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
			iterator() :
				iterator(nullptr, nullptr, nullptr) {
			}
			iterator(node *curr) :
				iterator(curr, curr->prev, curr->next) {
			}
			iterator(node *curr, node *prev, node *next) {
				this->curr = curr;
				this->prev = prev;
				this->next = next;
			}
			
			iterator &operator++() {
				prev = curr;
				curr = next;
				if (curr != nullptr) next = curr->next;
				return *this;
			}
			const iterator operator++(int) {
				iterator result(*this);
				++(*this);
				return result;
			}
			iterator &operator--() {
				next = curr;
				curr = prev;
				if (curr != nullptr) prev = curr->prev;
				return *this;
			}
			const iterator operator--(int) {
				iterator result(*this);
				--(*this);
				return result;
			}

			iterator operator+=(size_t amount) {

			}
			iterator operator+(size_t amount) const {

			}
			iterator operator-=(size_t amount) {

			}
			iterator operator-(size_t amount) const {

			}

			bool operator==(const iterator &iter) const {
				return this->curr == iter.curr;
			}
			bool operator!=(const iterator &iter) const {
				return !(*this == iter);
			}

			T &operator*() const {
				return curr->element;
			}
			T *operator->() const {
				return &(curr->element);
			}

		public:
			node *prev, *next;
			node *curr;
		};

	public:
		list() : length(0), head(nullptr), tail(nullptr) {
		}
		list(std::initializer_list<T> initList) : list() {
			assign(initList);
		}
		template <class _Iter>
		list(const _Iter &first, const _Iter &last) : list() {
			assign(first, last);
		}

		const T &front() const {
			if (empty())
				throw new std::runtime_error("front() called on empty list");
			return head->element;
		}
		const T &back() const {
			if (empty())
				throw new std::runtime_error("back() called on empty list");
			return tail->element;
		}

		void assign(size_t count, const T &value) {
			clear();

			while (count--)
				push_back(value);
		}
		template <class _Iter>
		void assign(const _Iter &first, const _Iter &last) {
			clear();

			insert(begin(), first, last);
		}
		void assign(std::initializer_list<T> initList) {
			clear();

			auto iter = initList.begin();
			while (iter != initList.end()) {
				push_back(*iter);
				++iter;
			}
		}

		void push_front(const T &value) {
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
		void push_back(const T &value) {
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
			delete target;

			if (tail != nullptr)
				tail->next = nullptr;
			else
				head = nullptr;

			length--;
		}

		iterator begin() const {
			return !empty() ? iterator(head) : end();
		}
		iterator end() const {
			return iterator(nullptr, tail, nullptr);
		}

		iterator find(const T &value) const {
			iterator iter;
			for (iter = begin(); iter != end(); iter++) {
				if (*iter == value)
					break;
			}

			return iter;
		}
		iterator find(const iterator &where, const T &value) const {
			iterator iter = where;

			while (iter != end()) {
				if (*iter == value)
					break;
				iter++;
			}

			return iter;
		}
		void remove(const T &value) {
			iterator iter = begin();
			iter = find(iter, value);

			while (iter != end()) {
				iter = erase(iter);
				iter = find(iter, value);
			}
		}

		iterator insert(const iterator &where, const T &value) {
			node *newNode = new node(value, where.prev, where.curr);
			node *nextNode = where.curr;
			iterator nextIter;

			if (where.prev != nullptr)
				where.prev->next = newNode;
			if (where.curr != nullptr)
				where.curr->prev = newNode;

			if (where == begin())
				head = newNode;
			if (where == end())
				tail = newNode;

			nextIter = nextNode != nullptr ? iterator(nextNode) : end();
			length++;

			return nextIter;
		}
		template <class _Iter>
		iterator insert(const iterator &where, const _Iter &first, const _Iter &last) {
			iterator iter = where;

			for (auto targetIter = first; targetIter != last; targetIter++)
				iter = insert(iter, *targetIter);

			return iter;
		}
		iterator erase(const iterator where) {
			node *target = where.curr, *nextNode = where.next;
			iterator nextIter;

			if (where.prev != nullptr)
				where.prev->next = target->next;
			if (where.next != nullptr)
				where.next->prev = target->prev;

			if (target == head)
				head = target->next;
			else if (target == tail)
				tail = target->prev;

			nextIter = nextNode != nullptr ? iterator(nextNode) : end();

			delete target;
			length--;

			return nextIter;
		}
		iterator erase(const iterator &first, const iterator &last) {
			iterator iter = first;

			while (iter != last)
				iter = erase(iter);

			return iter;
		}

		inline int size() const {
			return length;
		}
		inline bool empty() const {
			return length == 0;
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
		int length;
	};
}