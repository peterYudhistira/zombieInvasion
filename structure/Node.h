#pragma once
#include <iostream>
#include <string>

using namespace std;
//menentukan yang bakal di spawn itu human atau zombie DAN dia spawn di baris mana
	//ada ini supaya kerjaan inisialisasi nggak langsung numpuk
	//pertama gini, error
class Blueprint {
private:
	char type = ' ';
	int position = 0;
public:
	Blueprint() {

	}

	Blueprint(char type_) {
		type = type_;
		position = rand() % 3 + 1; //1 2 3
	}

	Blueprint(char type_, int position_) {
		type = type_;
		position = position_;
	}

	void setType(char type_) {
		type = type_;
	}

	char getType() {
		return type;
	}

	void setPos(int position_) {
		position = position_;
	}

	int getPos() {
		return position;
	}

	//spawn location changer to prevent zombie from spawning at same lane as hooman
	void spawnChange(int pos) {
		if (type == 'h') {
			return;
		}

		while (position == pos) {
			position = rand() % 3 + 1;
		}
	}
};
	
	//Node of Blueprints
class Node {
private:
	Blueprint value_; //data field
	Node* next_; //pointer to next node
public:
	//constructor
	Node() {
		next_ = NULL; //first node has nowhere to go.
	}

	//PAM
	void setValue(Blueprint value) {
		value_ = value;
	}
	Blueprint getValue() {
		return value_;
	}

	void setNext(Node* next) {
		next_ = next;
	}
	Node* getNext() {
		return next_;
	}
};

