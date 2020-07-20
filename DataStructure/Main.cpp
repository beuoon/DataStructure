#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>

#include "list.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "set.hpp"
#include "map.hpp"

using namespace std;

template <class list>
void testList(list &tl) {
	int expectedArr[] = { 2, 4, 6, 10 };
	tl.assign({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

	for (auto iter = tl.begin(); iter != tl.end(); ) {
		if (*iter % 2 == 1)
			iter = tl.erase(iter);
		else
			iter++;
	}

	tl.remove(8);

	int idx = 0;
	for (auto element : tl) {
		if (element != expectedArr[idx])
			break;
		idx++;
	}

	if (idx != 4)
		throw new exception("testList - failed");
}

template <class stack>
void testStack(stack &ts) {
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int size = 10;

	for (int i = 0; i < size; i++)
		ts.push(arr[i]);

	for (int i = size-1; i >= 0; i--) {
		if (ts.top() != arr[i])
			break;
		ts.pop();
	}

	if (!ts.empty())
		throw new exception("testStack - failed");
}

template <class queue>
void testQueue(queue &tq) {
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int size = 10;

	for (int i = 0; i < size; i++)
		tq.push(arr[i]);

	for (int i = 0; i < size; i++) {
		if (tq.front() != arr[i])
			break;
		tq.pop();
	}

	if (!tq.empty())
		throw new exception("testQueue - failed");
}

template <class set>
void testSet(set &ts) {
	// Test insert array
	{
		int initArr[] = { 1, 6, 7, 3, 2, 4, 8, 9, 5, 3, 0 };
		ts.insert(&initArr[0], &initArr[10]);

		int num = 1;
		for (auto element : ts) {
			if (element != num)
				break;
			++num;
		}

		if (num != 10)
			throw new exception("testSet - failed insert array");
	}

	// Test erase
	ts.erase(3);
	ts.erase(6);

	// Test lower_bound, upper_bound
	{
		int expectedArray[] = { 4, 5, 7 };
		int idx = 0;

		for (auto iter = ts.lower_bound(3); iter != ts.upper_bound(7); iter++, idx++) {
			if (*iter != expectedArray[idx])
				break;
		}

		if (idx != 3)
			throw new exception("testSet - failed erase and lower_bound, upper_bound");
	}
}

template <class map>
void testMap(map &tm) {
	// Test insert array
	{
		pair<int, string> initArr[] = { {3, "p"}, {1, "h"}, { 2, "a"}, {5, "y"}, {4, "p"}, {3, "b"}, {0, ""} };
		tm.insert(&initArr[0], &initArr[6]);

		string expectedStr = "happy";
		int idx = 0;
		for (auto element : tm) {
			if (element.second[0] != expectedStr.at(idx))
				break;
			++idx;
		}

		if (idx != 5)
			throw new exception("testMap - failed insert array");
	}

	// Test operator[], At
	if (tm[3] != "p" || tm.at(3) != "p")
		throw new exception("testMap - failed operator[], at");

	// Test erase
	tm.erase(3);

	// Test lower_bound, upper_bound
	{
		string expectedStr = "apy";
		int idx = 0;

		for (auto iter = tm.lower_bound(2); iter != tm.upper_bound(5); iter++, idx++) {
			if (iter->second[0] != expectedStr[idx])
				break;
		}

		if (idx != 3)
			throw new exception("testMap - failed erase and lower_bound, upper_bound");
	}
}

void test() {
	try {
		std::list<int> stdList;
		bon::list<int> bonList;
		testList(stdList);
		testList(bonList);

		std::stack<int> stdStack;
		bon::stack<int> bonStack;
		testStack(stdStack);
		testStack(bonStack);

		std::queue<int> stdQueue;
		bon::queue<int> bonQueue;
		testQueue(stdQueue);
		testQueue(bonQueue);

		std::set<int> stdSet;
		bon::set<int> bonSet;
		testSet(stdSet);
		testSet(bonSet);

		std::map<int, string> stdMap;
		bon::map<int, string> bonMap;
		testMap(stdMap);
		testMap(bonMap);

		cout << "succeed" << endl;
	}
	catch (exception *except) {
		cerr << except->what() << endl;
		cout << "failed" << endl;
	}
}

int main() {
	test();

	return 0;
}