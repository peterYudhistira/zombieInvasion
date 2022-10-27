#pragma once
#include <iostream>
#include <string>

using namespace std;

template <class Type>
class Node {
private:
	Type value_; //data field
	Node* next_; //pointer to next node
public:
	//constructor
	Node() {
		next_ = NULL; //first node has nowhere to go.
	}

	//PAM
	void setValue(Type value) {
		value_ = value;
	}
	Type getValue() {
		return value_;
	}

	void setNext(Node* next) {
		next_ = next;
	}
	Node* getNext() {
		return next_;
	}
};

template <class Type>
class LinkedList {
private:
	Node<Type>* head_;
	Node<Type>* tail_;
public:
	//constructor
	LinkedList<Type>() {
		//is NULL at first because empty
		head_ = NULL;
		tail_ = NULL;
	}

	void add_value(Type value) {
		Node<Type>* temp = new Node<Type>; //initialize a new node with a known pointer
		temp->setValue(value); //set value to temp

		//if the first ever entry, initialize this node as head and tail
		if (head_ == NULL) {
			head_ = temp;
			tail_ = head_;
		}
		else {
			tail_->setNext(temp); //temp is now next to tail
			tail_ = temp; //temp has become tail, previous tail has become just another link in the chain
		}
	}

	void display() {
		Node<Type>* temp = head_; //create a Node, start with head.

		//keep doing this while temp is not NULL
		while (temp) {
			cout << temp->getValue() << " "; //display value
			temp = temp->getNext(); //move onto the next
		}
		cout << endl; //a little space after all is done
	}

	void update_value(int index, Type value) {
		Node<Type>* temp = head_; //create a Node, start with head.
		int counter = 0; //assume head is index 0 of an "array"

		//while it's not the desired index yet, keep scrolling right
		while (counter < index) {
			temp = temp->getNext();
			counter++;
		}

		//now that the desired index is obtained, 
		temp->setValue(value); //replace its value
	}

	void delete_value(int index) {
		Node<Type>* temp = head_; //create a Node, start with head.
		Node<Type>* dump;

		//if first index
		if (index == 0) {
			head_ = head_->getNext();
			dump = head_;
		}
		else {
			int counter = 0; //start with index 0
			//scroll right until desired index
			while (counter < index - 1) {
				temp = temp->getNext();
				counter++;
			}

			dump = temp->getNext();

			//at this point temp is the index RIGHT BEFORE the deleted index
			temp->setNext(temp->getNext()->getNext()); //temp's next becomes the deleted index's former next.
			tail_ = temp; //now temp is the last index
			//essentially, the deleted index is cut out of the link now.
		}
		delete dump;
	}

	void show_node(int index) {
		Node<Type>* temp = head_; //create a Node, start with head.

		int counter = 0; //start with index 0
			//scroll right until desired index
		while (counter < index) {
			temp = temp->getNext();
			counter++;
		}

		//now the index is obtained
		cout << "Current Node : " << temp->getValue() << endl;
		cout << "Next Node : " << temp->getNext()->getValue() << endl;

	}

	//operator
	Type operator[](int index) {
		Node<Type>* temp = head_;
		//keep scrolling until the right index
		for (int i = 0; i < index; i++) {
			temp = temp->getNext();
		}

		//return the value of this Node
		return temp->getValue();
	}
};