#pragma once

class DataStructure {
public:
	DataStructure() : length(0) {

	}

	inline int size() const {
		return length;
	}
	inline bool empty() const {
		return length == 0;
	}

	virtual void clear() = 0;

protected:
	int length;
};