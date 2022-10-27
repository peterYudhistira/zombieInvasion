#pragma once
#include <iostream>

using namespace std;

class node {
public:
	int value;
	node* next, * prev;
};

class CircularDoubleLinkedList {
public:
	int length = 0;
	node* head, * tail;
	CircularDoubleLinkedList() {
		head = NULL;
		tail = NULL;
	}
	void addRear(int value_) {
		node* temp = new node;
		length++;
		temp->value = value_;
		if (head == NULL) {
			head = temp;
			tail = head;
		}
		else {
			tail->next = temp;
			temp->prev = tail;
			tail = temp;
			// circular
			tail->next = head;
			head->prev = tail;
		}
	}
	void addFront(int value_) {
		node* temp = new node;
		length++;
		temp->value = value_;
		if (head == NULL) {
			head = temp;
			tail = head;
		}
		else {
			head->prev = temp;
			temp->next = head;
			head = temp;
			// circular
			tail->next = head;
			head->prev = tail;
		}
	}
	void addMiddle(int index_, int value_) {
		if (index_ == 0) {
			addFront(value_);
		}
		else if (index_ == length) {
			addRear(value_);
		}
		else {
			// looping ke index
			node* temp2 = head;
			for (int i = 0; i < index_; i++) {
				temp2 = temp2->next;
			}
			// add middle
			node* temp = new node;
			length++;
			temp->value = value_;
			temp->next = temp2;
			temp->prev = temp2->prev;
			temp2->prev->next = temp;	// kiri
			temp2->prev = temp;			// kanan
			// circular
			tail->next = head;
			head->prev = tail;
		}
	}
	void updateFront(int value_) {
		head->value = value_;
	}
	void updateRear(int value_) {
		tail->value = value_;
	}
	void updateMiddle(int index_, int value_) {
		node* temp = head;
		for (int i = 0; i < index_; i++) {
			temp = temp->next;
		}
		temp->value = value_;
	}
	void deleteRear() {
		node* temp = tail;
		temp = temp->prev;	// mundur 1 langkah
		delete[] temp->next;	// tendangan maut
		tail = temp;
		length--;
		// circular
		tail->next = head;
		head->prev = tail;
	}
	void deleteFront() {
		node* temp = head;
		temp = temp->next;	// maju 1 langkah
		delete[] temp->prev;	// tendangan maut
		head = temp;
		length--;
		// circular
		tail->next = head;
		head->prev = tail;
	}
	void deleteMidde(int index_) {
		node* temp = head;
		if (index_ == 0) {
			deleteFront();
		}
		else if (index_ == length) {
			deleteRear();
		}
		else {
			for (int i = 0; i < index_ - 1; i++) {
				temp = temp->next;
			}
			node* temp2 = temp->next;
			temp->next = temp->next->next;	// maju 2 langkah
			delete temp2;	// tendangan maut
			temp->next->prev = temp;
			// circular
			tail->next = head;
			head->prev = tail;
		}
	}
	void display() {
		node* temp = head;
		while (temp != tail) {
			cout << temp->value << " ";
			temp = temp->next;
		}
		cout << temp->value << endl;
	}
	void reversedisplay() {
		node* temp = tail;
		while (temp != head) {
			cout << temp->value << " ";
			temp = temp->prev;
		}
		cout << temp->value << endl;
	}
	int operator[](int index_) {
		node* temp = head;
		for (int i = 0; i < index_; i++) {
			temp = temp->next;
		}
		return temp->value;
	}
};