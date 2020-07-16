#pragma once

#include <functional>

namespace bon {
	template <typename T, class Compare = std::less<T>>
	class AVLTree {
	protected:
		class node {
		public:
			node(const T &value, node *parent) {
				this->value = value;
				this->parent = parent;
				this->left = nullptr;
				this->right = nullptr;
				this->height = 1;
			}

			int getBalance() {
				int leftHeight = (left != nullptr) ? left->height : 0;
				int rightHeight = (right != nullptr) ? right->height : 0;

				return rightHeight - leftHeight;
			}
			void recalcHeight() {
				int leftHeight = (left != nullptr) ? left->height : 0;
				int rightHeight = (right != nullptr) ? right->height : 0;

				height = (leftHeight > rightHeight) ? leftHeight+1 : rightHeight+1;
			}

			T value;
			node *parent;
			node *left, *right;
			int height;
		};

	public:
		class iterator {
		public:
			iterator() : iterator(nullptr, nullptr, nullptr) {
			}
			iterator(node *curr, node *prev, node *next) {
				this->curr = curr;
				this->prev = prev;
				this->next = next;
			}
			
			iterator &operator++() {
				prev = curr;
				curr = next;

				if (curr == nullptr)
					next = nullptr;
				else if (curr->right != nullptr)
					next = GetMinNode(curr->right);
				else {
					node *target = curr;
					node *parent = target->parent;

					while (parent != nullptr) {
						if (parent->right == target) {
							target = parent;
							parent = target->parent;
						}
						else
							break;
					}

					next = parent;
				}

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

				if (curr == nullptr)
					prev = nullptr;
				else if (curr->left != nullptr)
					prev = GetMaxNode(curr->left);
				else {
					node *target = curr;
					node *parent = target->parent;

					while (parent != nullptr) {
						if (parent->left == target) {
							target = parent;
							parent = target->parent;
						}
						else
							break;
					}

					prev = parent;
				}

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
				return curr->value;
			}
			T *operator->() const {
				return &(curr->value);
			}

		public:
			node *prev, *next;
			node *curr;
		};

	public:
		AVLTree() : compare(Compare()) {
			root = nullptr;
			length = 0;
		}

		void insert(const T &value) {
			if (root == nullptr) {
				root = new node(value, nullptr);
				return;
			}

			node *curr = nullptr;
			node *next = root;
			while (next != nullptr) {
				curr = next;
				if (value == curr->value) return; // 동일한 값이 있으면 종료
				next = (compare(value, curr->value)) ? curr->left : curr->right;
			}

			node *newNode = new node(value, curr);
			if (compare(value, curr->value))
				curr->left = newNode;
			else
				curr->right = newNode;
			length++;

			node *target = curr;
			while ((target = GetUnbalancedNodeWithRecalcHeight(target)) != nullptr) {
				target = MaintainBalance(target);
				if (target->parent == nullptr)
					root = target;
			}
		}
		void erase(const T &value) {
			node *curr = root;
			while (curr != nullptr) {
				if (curr->value == value) break;
				curr = compare(value, curr->value) ? curr->left : curr->right;
			}

			if (curr == nullptr) // 찾는 값이 없으면 종료
				return;

			while (true) {
				bool isLeaf = curr->left == nullptr && curr->right == nullptr;
				if (isLeaf) break;

				node *swapTarget;
				if (curr->left != nullptr)
					swapTarget = GetMaxNode(curr->left);
				else
					swapTarget = GetMinNode(curr->right);

				SwapNode(curr, swapTarget);
				if (curr == root)
					root = swapTarget;
			}

			node *parent = curr->parent;
			((parent->left == curr) ? parent->left : parent->right) = nullptr;

			delete curr;
			length--;

			node *target = parent;
			while ((target = GetUnbalancedNodeWithRecalcHeight(target)) != nullptr) {
				target = MaintainBalance(target);
				if (target->parent == nullptr)
					root = target;
			}
		}

		iterator begin() const {
			if (!empty()) {
				node *curr = GetMinNode(root);
				node *next = (curr->right != nullptr) ? curr->right : curr->parent;
				return iterator(curr, nullptr, next);
			}
			else
				return end();
		}
		iterator end() const {
			node *prev = GetMaxNode(root);
			return iterator(nullptr, prev, nullptr);
		}

		inline int size() const {
			return length;
		}
		inline bool empty() const {
			return length == 0;
		}
		void clear() {
			length = 0;
		}

	private:
		static node *MaintainBalance(node *target) {
			node *x, *y, *z;
			node *a, *b, *c;
			node *t0, *t1, *t2, *t3;

			z = target;
			if (z->getBalance() < 0) {
				y = z->left;
				if (y->getBalance() < 0) {
					x = y->left;

					a = x;
					b = y;
					c = z;

					t0 = x->left;
					t1 = x->right;
					t2 = y->right;
					t3 = z->right;
				} else {
					x = y->right;

					a = y;
					b = x;
					c = z;

					t0 = y->left;
					t1 = x->left;
					t2 = x->right;
					t3 = z->right;
				}
			} else {
				y = z->right;
				if (y->getBalance() < 0) {
					x = y->left;

					a = z;
					b = x;
					c = y;

					t0 = z->left;
					t1 = x->left;
					t2 = x->right;
					t3 = y->right;
				} else {
					x = y->right;

					a = z;
					b = y;
					c = x;

					t0 = z->left;
					t1 = y->left;
					t2 = x->left;
					t3 = x->right;
				}
			}
			
			node *parent = z->parent;

			a->parent = b;
			a->left = t0;
			a->right = t1;
			a->recalcHeight();
			if (t0 != nullptr) t0->parent = a;
			if (t1 != nullptr) t1->parent = a;

			c->parent = b;
			c->left = t2;
			c->right = t3;
			c->recalcHeight();
			if (t2 != nullptr) t2->parent = c;
			if (t3 != nullptr) t3->parent = c;

			b->parent = parent;
			b->left = a;
			b->right = c;
			b->recalcHeight();

			if (parent != nullptr) {
				if (parent->left == z)
					parent->left = b;
				else
					parent->right = b;
			}

			return b;
		}
		static node *GetUnbalancedNodeWithRecalcHeight(node *target) {
			int leftHeight, rightHeight;

			while (target != nullptr) {
				leftHeight = (target->left != nullptr) ? target->left->height : 0;
				rightHeight = (target->right != nullptr) ? target->right->height : 0;

				int newHeight = (leftHeight > rightHeight) ? leftHeight+1 : rightHeight+1;
				target->height = newHeight;
				if (abs(leftHeight - rightHeight) >= 2)
					break;
				target = target->parent;
			}

			return target;
		}

		static node *GetMinNode(node *target) {
			while (target->left != nullptr)
				target = target->left;
			return target;
		}
		static node *GetMaxNode(node *target) {
			while (target->right != nullptr)
				target = target->right;
			return target;
		}

		static void SwapNode(node *n1, node *n2) {
			// n1과 n2이 부모-자식 관계면, n1이 부모가 되도록 변경
			if (n1->parent == n2) {
				node *temp = n2;
				n2 = n1;
				n1 = temp;
			}
			bool isLeftChild = n1->left == n2;
			bool isRightChild = n1->right == n2;

			node *n1_p = n1->parent, *n1_lc = n1->left, *n1_rc = n1->right;
			node *n2_p = n2->parent, *n2_lc = n2->left, *n2_rc = n2->right;

			if (n2_p != nullptr)
				((n2 == n2_p->left) ? n2_p->left : n2_p->right) = n1;
			if (n1_p != nullptr)
				((n1 == n1_p->left) ? n1_p->left : n1_p->right) = n2;

			n1->parent = (isLeftChild || isRightChild) ? n2 : n2_p;
			n2->parent = n1_p;

			n1->left = n2_lc;
			n1->right = n2_rc;

			n2->left = (isLeftChild) ? n1 : n1_lc;
			n2->right = (isRightChild) ? n1 : n1_rc;

			if (n2_lc != nullptr) n2_lc->parent = n1;
			if (n2_rc != nullptr) n2_rc->parent = n1;

			if (!isLeftChild && n1_lc != nullptr) n1_lc->parent = n2;
			if (!isRightChild && n1_rc != nullptr) n1_rc->parent = n2;
		}

	protected:
		const Compare &compare;
		node *root;
		int length;
	};
}