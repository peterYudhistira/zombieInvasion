#pragma once
#include <iostream>
#include <string>
#include "spawn.h"
#include "Node.h"
//Spawner = Linked List
	//basically kelas ini adalah Linear Single Linked List
	//yang di-customize.
	//karena waktu pertama kali coba template, amburadul
	//thanks, C++. Real modern.
class Spawner {
private:
	Node* head_;
	Node* tail_;

	int defvalue = 20;
	int hpos = 0;
	int counter = 0;

	//human spawn special condition setter
	bool humanSpawn = false;
public:
	//constructor
	Spawner() {
		//is NULL at first because empty
		head_ = NULL;
		tail_ = NULL;
	}

	void add_value(Blueprint value) {
		Node* temp = new Node; //initialize a new node with a known pointer
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

	void update_value(int index, Blueprint value) {
		Node* temp = head_; //create a Node, start with head.
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
		Node* temp = head_; //create a Node, start with head.

		//if first index
		if (index == 0) {
			head_ = head_->getNext();
		}
		else {
			int counter = 0; //start with index 0
			//scroll right until desired index
			while (counter < index - 1) {
				temp = temp->getNext();
				counter++;
			}

			//at this point temp is the index RIGHT BEFORE the deleted index
			temp->setNext(temp->getNext()->getNext()); //temp's next becomes the deleted index's former next.
			tail_ = temp; //now temp is the last index
			//essentially, the deleted index is cut out of the link now.
		}
	}

	//operator
	Blueprint operator[](int index) {
		Node* temp = head_;
		//keep scrolling until the right index
		for (int i = 0; i < index; i++) {
			temp = temp->getNext();
		}

		//return the value of this Node
		return temp->getValue();
	}
	
	//So from this part onwards, it's the other stuffs
		//Linked List empty?
	bool Empty() {
		if (head_) {
			return false;
		}
		else {
			return true;
		}
	}

	//RESET ALL
	void reset() {
		humanSpawn = false;
		hpos = 0;
		RecycleBin();
	}

	//DELETE ALL FOR SPAWNER CLEAR
		//delete semua isi linked list
	void RecycleBin() {
		Node* tmp = head_;
		while (tmp) {
			delete_value(0); //pop pop pop pop pop
			tmp = tmp->getNext();
		}
	}

	//mengirimkan entity yang ada di head ke Keeper
	Node* deliverSpawn() {
		Node* temp = head_;
		return temp;
	}

	//pop
	void deliveryEnd() {
		delete_value(0);
	}

	//spawning a single entity
	void spawn() {
		//every few zombies, there's one human
		if (counter >= 3) {
			spawnHuman();
			int counter2 = rand() % 5; //randomizing that counter
			counter -= counter2;
			humanSpawn = true;
		}
		else {
			spawnZombie();
			counter++;
		}
	}

	void spawnZombie() {
		Blueprint spawn('z');
		
		if (humanSpawn == true) {
			if (spawn.getPos() == hpos) {
				spawn.spawnChange(hpos);
			}
			humanSpawn = false;
		}

		add_value(spawn);
	}

	void spawnHuman() {
		Blueprint spawn('h');
		hpos = spawn.getPos();

		add_value(spawn);
	}
};