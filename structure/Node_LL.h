#pragma once
#include <iostream>
#include <string>

using namespace std;


class Node {
private:
	Spawn value_; //data field
	Node* next_; //pointer to next node
public:
	//constructor
	Node() {
		next_ = NULL; //first node has nowhere to go.
	}

	//PAM
	void setValue(Spawn value) {
		value_ = value;
	}
	Spawn getValue() {
		return value_;
	}

	void setNext(Node* next) {
		next_ = next;
	}
	Node* getNext() {
		return next_;
	}
};

