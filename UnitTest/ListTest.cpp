#include "CppUnitTest.h"
#include "../DataStructure/list.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(ListTest) {
	public:
		template<typename T>
		static void AssertEqual(bon::list<T> expected, bon::list<T> actual) {
			Assert::AreEqual(expected.size(), actual.size());
			for (auto exptectedIter = expected.begin(), actualIter = actual.begin(); exptectedIter != expected.end() && actualIter != actual.end(); exptectedIter++, actualIter++)
				Assert::AreEqual(*exptectedIter, *actualIter);
		}

		TEST_METHOD(checkInit) {
			bon::list<int> actualList({ 1,2,3,4 });

			int number = 1;
			for (auto iter = actualList.begin(); iter != actualList.end(); iter++, number++)
				Assert::AreEqual(number, *iter);
		}
		TEST_METHOD(checkAssignCountValue) {
			bon::list<int> actualList;
			int count = 3, value = 1;

			actualList.assign(count, value);

			Assert::AreEqual(count, actualList.size());
			for (auto iter = actualList.begin(); iter != actualList.end(); iter++)
				Assert::AreEqual(value, *iter);
		}
		TEST_METHOD(checkAssignRange) {
			bon::list<int> expectedList({ 1, 2, 3, 4 });
			bon::list<int> actualList;

			actualList.assign(expectedList.begin(), expectedList.end());

			AssertEqual(expectedList, actualList);
		}
		TEST_METHOD(checkAssignInitializerList) {
			bon::list<int> actualList;

			actualList.assign({ 1, 2, 3, 4 });

			int number = 1;
			for (auto iter = actualList.begin(); iter != actualList.end(); iter++, number++)
				Assert::AreEqual(number, *iter);
		}

		TEST_METHOD(checkPushBack) {
			bon::list<int> actualList;
			int arr[] = { 1, 2, 3 };
			int size = sizeof(arr)/sizeof(int);

			for (int i = 0; i < size; i++)
				actualList.push_back(arr[i]);

			int i = 0;
			Assert::AreEqual(size, actualList.size());
			for (auto iter = actualList.begin(); iter != actualList.end(); iter++)
				Assert::AreEqual(arr[i++], *iter);
		}
		TEST_METHOD(checkPushFront) {
			bon::list<int> actualList;
			int arr[] = { 1, 2, 3 };
			int size = sizeof(arr)/sizeof(int);

			for (int i = size-1; i >= 0; i--)
				actualList.push_front(arr[i]);

			int i = 0;
			Assert::AreEqual(size, actualList.size());
			for (auto iter = actualList.begin(); iter != actualList.end(); iter++)
				Assert::AreEqual(arr[i++], *iter);
		}
		TEST_METHOD(checkPopBack) {
			bon::list<int> actualList;
			int arr[] = { 1, 2, 3 };
			int size = sizeof(arr)/sizeof(int);

			for (int i = 0; i < size; i++)
				actualList.push_back(arr[i]);
			actualList.pop_back();

			Assert::AreEqual(size-1, actualList.size());
			Assert::AreEqual(arr[size-2], actualList.back());

			actualList.pop_back();
			actualList.pop_back();

			Assert::AreEqual(0, actualList.size());
		}
		TEST_METHOD(checkPopFront) {
			bon::list<int> actualList;
			int arr[] = { 1, 2, 3 };
			int size = sizeof(arr)/sizeof(int);

			for (int i = 0; i < size; i++)
				actualList.push_back(arr[i]);
			actualList.pop_front();

			Assert::AreEqual(size-1, actualList.size());
			Assert::AreEqual(arr[1], actualList.front());

			actualList.pop_back();
			actualList.pop_back();

			Assert::AreEqual(0, actualList.size());
		}

		TEST_METHOD(checkFrontIfEmptyList) {
			bon::list<int> actualList;
			auto func = [actualList] { actualList.front(); };

			Assert::ExpectException<std::runtime_error *>(func);
		}
		TEST_METHOD(checkFrontIfNotEmptyList) {
			bon::list<int> actualList;

			actualList.push_back(1);
			actualList.push_back(2);

			Assert::AreEqual(1, actualList.front());
		}
		TEST_METHOD(checkBackIfEmptyList) {
			bon::list<int> actualList;
			auto func = [actualList] { actualList.back(); };

			Assert::ExpectException<std::runtime_error *>(func);
		}
		TEST_METHOD(checkBackIfNotEmptyList) {
			bon::list<int> actualList;

			actualList.push_back(1);
			actualList.push_back(2);

			Assert::AreEqual(2, actualList.back());
		}

		TEST_METHOD(checkFind) {
			bon::list<int> actualList({ 1, 2, 3, 4 });
			int expectedNum = 3;

			auto iter =  actualList.find(expectedNum);

			Assert::AreEqual(expectedNum, *iter);
		}
		TEST_METHOD(checkFindWhere) {
			bon::list<int> actualList({ 1, 2, 3, 4, 5, 6, 7 });
			int expectedNum = 3;

			auto iter = actualList.find(actualList.find(4), 3);

			Assert::IsTrue(actualList.end() == iter);
		}
		TEST_METHOD(checkRemove) {
			bon::list<int> actualList({ 1, 2, 3, 4, 5, 6, 7, 2, 3, 4, 5, 6, 2, 3, 4 });
			int expectedRemovedNum = 2;

			actualList.remove(expectedRemovedNum);
			auto iter = actualList.find(expectedRemovedNum);

			Assert::IsTrue(actualList.end() == iter);
		}

		TEST_METHOD(checkInsertWhereIfNotEmptyList) {
			bon::list<int> expectedList({ 1, 2, 3, 4 });
			bon::list<int> actualList({ 1, 2, 4 });

			auto iter = actualList.insert(actualList.find(4), 3);
			
			Assert::AreEqual(*iter, 4);
			AssertEqual(expectedList, actualList);
		}
		TEST_METHOD(checkInsertWhereIfEmptyList) {
			bon::list<int> expectedList({ 1, 2 });
			bon::list<int> actualList;

			actualList.insert(actualList.end(), 2);
			actualList.insert(actualList.begin(), 1);

			AssertEqual(expectedList, actualList);
		}
		TEST_METHOD(checkInsertCopyRange) {
			bon::list<int> expectedList({ 1, 2, 3, 4 });
			bon::list<int> actualList;

			actualList.insert(actualList.begin(), expectedList.begin(), expectedList.end());

			AssertEqual(expectedList, actualList);
		}
		TEST_METHOD(checkEraseWhere) {
			bon::list<int> expectedList({ 1, 2, 4 });
			bon::list<int> actualList({ 1, 2, 3, 4 });

			actualList.erase(actualList.find(3));

			AssertEqual(expectedList, actualList);
		}
		TEST_METHOD(checkEraseRange) {
			bon::list<int> expectedList({ 1, 4 });
			bon::list<int> actualList({ 1, 2, 3, 4 });

			actualList.erase(actualList.find(2), ++actualList.find(3));

			AssertEqual(expectedList, actualList);
		}
	};
}