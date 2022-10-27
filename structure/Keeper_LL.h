#pragma once
#include <iostream>
#include <string>
#include "spawn.h"
#include "Node.h"

class Keeper {
private:
	Node* head_;
	Node* tail_;

	int defvalue = 20;
public:
	//constructor
	Keeper() {
		//is NULL at first because empty
		head_ = NULL;
		tail_ = NULL;
	}

	void add_value(Spawn value) {
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

	void add_value(Node* value) {
		value->setNext(NULL);

		Node* temp = value; //set value to temp
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

	//void display() {
	//	Node* temp = head_; //create a Node, start with head.

	//	//keep doing this while temp is not NULL
	//	while (temp) {
	//		cout << temp->getValue()- << " "; //display value
	//		temp = temp->getNext(); //move onto the next
	//	}
	//	cout << endl; //a little space after all is done
	//}

	void update_value(int index, Spawn value) {
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
		Node* dump;

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

	//void show_node(int index) {
	//	Node* temp = head_; //create a Node, start with head.

	//	int counter = 0; //start with index 0
	//		//scroll right until desired index
	//	while (counter < index) {
	//		temp = temp->getNext();
	//		counter++;
	//	}

	//	//now the index is obtained
	//	cout << "Current Node : " << temp->getValue() << endl;
	//	cout << "Next Node : " << temp->getNext()->getValue() << endl;

	//}

	//operator
	Spawn operator[](int index) {
		Node* temp = head_;
		//keep scrolling until the right index
		for (int i = 0; i < index; i++) {
			temp = temp->getNext();
		}

		//return the value of this Node
		return temp->getValue();
	}

	//So from this part onwards, it's the other stuffs

	//For Template purposes only because copaste base
	void PlsDontUseThisFunction() {
		Node* tmp = head_;
		while (tmp) {
			//AAAAA
			tmp = tmp->getNext();
		}
	}

	void PlsDontUseThisFunction2() {
		Node* tmp = head_;
		while (tmp) {
			Node* tmp2 = head_;
			while (tmp2) {
				//AAAAA
				tmp2 = tmp2->getNext();
			}
			tmp = tmp->getNext();
		}
	}
	//Will be deleted/moved into another file later for "Cleaning" purposes.

	//RESET ALL
	void reset() { //Note that the addresses haven't been changed for deletion purposes
		Node* tmp = head_;
		while (tmp) {
			tmp->getValue().reset();
			tmp = tmp->getNext();
		}

		RecycleBin();
	}

	//DELETE ALL... Or more like removing the dead bodies.
	void RecycleBin() {
		Node* tmp = head_;
		while (tmp) {
			delete_value(0);
			tmp = tmp->getNext();
		}
	}

	//Moving from Keeper to Spawner
	void addSpawn(Node* spawn) {
		spawn->getValue().Live();
		add_value(spawn);
	}

	char newSpawn() {
		return tail_->getValue().getType();
	}

	//Returning position X
	int getPosX(int index) {
		Node* tmp = head_;
		int counter = 0;
		while (counter < index) {
			tmp = tmp->getNext();
		}

		return tmp->getValue().getPosX();
	}

	//Shooting objects
	int Shoot(Event event, Stats& stat) { //Note that the addresses haven't been changed for deletion purposes
		Node* tmp = head_;
		int counter = 0;

		while (tmp) {
			if (tmp->getValue().isClicked(event.mouseButton.x, event.mouseButton.y) && !tmp->getValue().Dying()) {
				if (tmp->getValue().getType() == 'h') {
					tmp->getValue().Merder(true, stat);
					continue;
				}
				else if (tmp->getValue().getType() == 'z') {
					tmp->getValue().Damage(stat);

					//For each dead zombie, increases counter
					if (!tmp->getValue().Alive()) {
						counter++;
					}

					continue;
				}
			}
			tmp = tmp->getNext();
		}

		//Returns the counter
		return counter;
	}

	//Draw functions
	void drawSpawn(RenderWindow& window, Time frameTime) {
		Node* tmp = head_;
		while (tmp) {
			if (tmp->getValue().getType() == 'z') { //Zombies
				//If Alive
				if (tmp->getValue().Alive()) {
					if (tmp->getValue().getMove()) {
						tmp->getValue().animationMove(frameTime.asSeconds());
						tmp->getValue().moveSprite(frameTime.asSeconds());
					}
					tmp->getValue().drawSprite(window);
				}
				//Let them die. Don't let the real Zomu die tho :'3
				if (tmp->getValue().Dying()) {
					tmp->getValue().animationDead(frameTime.asSeconds());
					tmp->getValue().drawSprite(window);
				}
			}
			if (tmp->getValue().getType() == 'h') { //humans
				//if Alive
				if (tmp->getValue().Alive()) {
					tmp->getValue().animationMove(frameTime.asSeconds());
					tmp->getValue().moveSprite(frameTime.asSeconds());
					tmp->getValue().drawSprite(window);
				}

				//They're dying, why are you only watching them?!
				if (tmp->getValue().Dying()) {
					tmp->getValue().animationDead(frameTime.asSeconds());
					tmp->getValue().drawSprite(window);
				}
			}
			tmp = tmp->getNext();
		}
	}

	//Death functions
	void deathCheck(Stats& stat) {
		Node* tmp = head_;
		while (tmp) {
			if (tmp->getValue().getType() == 'h' && tmp->getValue().Alive() && !tmp->getValue().Dying()) {
				Node* tmp2 = head_;
				while (tmp2) {
					if (tmp2->getValue().getType() == 'z' && tmp2->getValue().Alive()) {
						tmp->getValue().checkCollision(tmp2->getValue(), stat);
					}
					tmp2 = tmp2->getNext();
				}
			}
			tmp = tmp->getNext();
		}
	}

	int despawnCheck(Stats& stat) {
		int score = 0;
		Node* tmp = head_;
		while (tmp) {
			//If Health not zero, Zombie despawns, Player HP decreases
			if (tmp->getValue().getType() == 'z' && tmp->getValue().getPosX() <= 320 && tmp->getValue().Alive()) {
				tmp->getValue().Merder(false, stat);
			}
			//If Health not zero, Human despawns, Player Score increases
			if (tmp->getValue().getType() == 'h' && tmp->getValue().getPosX() <= 320 && tmp->getValue().Alive()) {
				tmp->getValue().Merder(false, stat);
				if (stat.CheckHP()) {
					score++;
				}
			}
			tmp = tmp->getNext();
		}
		return score;
	}

	//Tower Functions
	int towerCheck(int position) {
		int index = defvalue;
		Node* tmp = head_;
		int counter = 0;

		while (tmp) {
			if (tmp->getValue().getType() == 'z' && tmp->getValue().Alive() && !tmp->getValue().Dying() && position == tmp->getValue().getSpawnPosition()) {
				index = counter;
				break;
			}
			counter++;
			tmp = tmp->getNext();
		}
		return index;
	}

	void towerDeath(int index, Stats& stat) {
		Node* tmp = head_;
		int counter = 0;

		while (counter < index && tmp) {
			counter++;
			tmp = tmp->getNext();
		}

		if (!tmp->getValue().Dying()) {
			tmp->getValue().Merder(true, stat);
		}
	}

	//DEBUGGING FUNCTION
	void checkValue(int input = 100) {
		Node* tmp;
		if (input == 100) {
			tmp = tail_;
			cout << "-= New spawn =-" << endl;
		}
		else {
			tmp = head_;
			int counter = 0;
			while (counter < input) {
				tmp = tmp->getNext();
				counter++;
			}
			cout << "-= Spawn No. " << input << "=-" << endl;
		}

		cout << "Type: " << tail_->getValue().getType() << endl;
		cout << "Speed: " << tail_->getValue().getSpeed() << endl;
		cout << "HP: " << tail_->getValue().getHP() << endl;
		cout << endl;
		cout << "Position X: " << tail_->getValue().getPosX() << endl;
		cout << "Position Y: " << tail_->getValue().getSpawnPosition() << endl;
		cout << endl;
		cout << "Alive: " << tail_->getValue().Alive() << endl;
		cout << "Dying: " << tail_->getValue().Dying() << endl;
		cout << endl;
	}
};