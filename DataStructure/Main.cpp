#include <iostream>
#include <list>
#include <stack>

#include "list.hpp"

using namespace std;

class MyClass {
public:
	MyClass(int value) : value(value) {

	}

	const int &operator=(const int &value) {
		this->value = value;
		return this->value;
	}
	int value;
};

int main() {
	list<MyClass> myList;
	myList.push_back(1);
	myList.push_back(2);

	myList.erase(myList.begin(), ++myList.begin());
	cout << myList.size() << endl;


	try {
		bon::list<MyClass> bonList;
		bonList.push_back(1);
		bonList.push_back(2);

		cout << "before clear" << endl;
		for (auto iter = bonList.begin(); iter != bonList.end(); iter++) {
			cout << iter->value << endl;
		}

		bonList.clear();

		cout << "after clear" << endl;
		for (auto iter = bonList.begin(); iter != bonList.end(); iter++) {
			cout << iter->value << endl;
		}

		bonList.push_front(0);
		bonList.push_front(-1);
		bonList.push_front(0);
		bonList.push_back(1);
		bonList.push_front(0);
		bonList.push_back(2);
		bonList.push_back(0);
		cout << "before erase" << endl;
		for (auto iter = bonList.begin(); iter != bonList.end(); iter++) {
			cout << iter->value << endl;
		}

		bonList.erase(bonList.begin(), bonList.end());

		cout << "after erase" << endl;
		for (auto iter = bonList.begin(); iter != bonList.end(); iter++) {
			cout << iter->value << endl;
		}
	}
	catch (runtime_error *error) {
		cerr << "error: " << error->what() << endl;
	}

	return 0;
}
