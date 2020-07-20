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
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type		= T;
			using difference_type	= int;
			using pointer			= T *;
			using reference			= T &;

			iterator() : iterator(nullptr, nullptr, nullptr) {
			}
			iterator(node *curr) {
				this->curr = curr;

				if (curr == nullptr) {
					prev = nullptr;
					next = nullptr;
					return;
				}

				if (curr->left != nullptr)
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

				if (curr->right != nullptr)
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
			}
			iterator(node *curr, node *prev, node *next)
				: curr(curr), prev(prev), next(next) {
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
		AVLTree(bool bAllowDuplication = true) : compare(Compare()), bAllowDuplication(bAllowDuplication) {
			root = nullptr;
			length = 0;
		}
		AVLTree(std::initializer_list<T> initList, bool bAllowDuplication = true) : AVLTree(initList.begin(), initList.end(), bAllowDuplication) {
		}
		template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
		AVLTree(const _Iter &first, const _Iter &last, bool bAllowDuplication = true) : AVLTree(bAllowDuplication) {
			insert(first, last);
		}
		AVLTree(const AVLTree &tree) : AVLTree(tree.begin(), tree.end(), tree.bAllowDuplication) {
		}
		~AVLTree() {
			clear();
		}

		const AVLTree &operator=(const AVLTree &target) {
			clear();
			bAllowDuplication = target.bAllowDuplication;
			insert(target.begin(), target.end());
			return *this;
		}

		iterator begin() const {
			return !empty() ? iterator(GetMinNode(root)) : end();
		}
		iterator end() const {
			node *prev = GetMaxNode(root);
			return iterator(nullptr, prev, nullptr);
		}

		virtual iterator find(const T &value) const {
			node *curr = nullptr;
			node *next = root;

			while (next != nullptr) {
				curr = next;

				bool bEqualCurr = areEqual(curr->value, value);

				if (bEqualCurr) {
					bool bEqualLeftChild = (curr->left != nullptr && areEqual(curr->left->value, value));

					if (bEqualLeftChild)
						next = curr->left;
					else
						break;
				}
				else {
					next = (compare(value, curr->value)) ? curr->left : curr->right;
				}
			}

			bool bExistValue = (curr != nullptr && areEqual(curr->value, value));

			return bExistValue ? iterator(curr) : end();
		}
		virtual iterator lower_bound(const T &value) const {
			node *curr = nullptr;
			node *next = root;

			while (next != nullptr) {
				curr = next;

				bool bEqualCurr = areEqual(curr->value, value);

				if (bEqualCurr) {
					bool bEqualRightChild = (curr->right != nullptr && areEqual(curr->right->value, value));

					if (bEqualRightChild)
						next = curr->right;
					else
						break;
				}
				else
					next = (compare(value, curr->value)) ? curr->left : curr->right;
			}

			iterator iter;
			if (curr != nullptr) {
				iter = iterator(curr);
				if (!areEqual(curr->value, value) && compare(curr->value, value)) // curr->value < value
					++iter;
			}
			else
				iter = end();

			return iter;
		}
		virtual iterator upper_bound(const T &value) const {
			auto iter = lower_bound(value);
			if (areEqual(*iter, value))
				++iter;
			return iter;
		}

		virtual void insert(const T &value) {
			if (root == nullptr) {
				root = new node(value, nullptr);
				return;
			}

			// 삽입할 위치 탐색
			node *curr = nullptr;
			node *next = root;
			while (next != nullptr) {
				curr = next;
				if (!bAllowDuplication && areEqual(curr->value, value)) return; // 중복 방지
				next = (compare(value, curr->value)) ? curr->left : curr->right;
			}

			// 노드 생성 및 삽입
			node *newNode = new node(value, curr);
			if (compare(value, curr->value))
				curr->left = newNode;
			else
				curr->right = newNode;
			length++;

			// 트리 밸런스 유지
			node *target = curr;
			while ((target = GetUnbalancedNodeWithRecalcHeight(target)) != nullptr) {
				target = MaintainBalance(target);
				if (target->parent == nullptr)
					root = target;
			}
		}
		template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
		void insert(const _Iter &first, const _Iter &last) {
			for (auto targetIter = first; targetIter != last; targetIter++)
				insert(*targetIter);
		}

		virtual void erase(const T &value) {
			iterator iter = find(value);

			while (iter != end() && areEqual(*iter, value))
				iter = erase(iter);
		}
		virtual iterator erase(const iterator where) {
			node *nextNode = where.next;
			node *curr = where.curr;

			// 삭제할 노드를 단말 노드로 이동
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

			// 삭제할 노드의 부모 자식값 설정
			node *parent = curr->parent;
			if (parent == nullptr)
				root = nullptr;
			else {
				node *&child = ((parent->left == curr) ? parent->left : parent->right);
				child = nullptr;
			}

			// 노드 삭제
			delete curr;
			length--;

			// 트리 밸런스 유지
			node *target = parent;
			while ((target = GetUnbalancedNodeWithRecalcHeight(target)) != nullptr) {
				target = MaintainBalance(target);
				if (target->parent == nullptr)
					root = target;
			}

			return iterator(nextNode);
		}
		virtual iterator erase(const iterator &first, const iterator &last) {
			iterator iter;
			for (iter = first; iter != last; )
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
			erase(begin(), end());
		}

	protected:
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
			if (target == nullptr)
				return nullptr;

			while (target->left != nullptr)
				target = target->left;
			return target;
		}
		static node *GetMaxNode(node *target) {
			if (target == nullptr)
				return nullptr;

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

		bool areEqual(const T &value1, const T &value2) const {
			return !(compare(value1, value2) ^ compare(value2, value1));
		}

	protected:
		const Compare compare;
		node *root;
		int length;
		bool bAllowDuplication;
	};
}