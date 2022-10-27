#pragma once
#include <iostream>
#include <string>
#include "spawn.h"
#include "Node.h"

class Keeper {
private:
	Spawn spawn[20]; //array isinya 20 karena nggak mungkin lebih dari segitu
	int fspawn = 100; //CONFUSION

	int defvalue = 20;
public:
	//constructor
	Keeper() {
		//Assurance purposes, nothing else
		for (int a = 0; a < defvalue; a++) {
			spawn[a].reset();
		}
	}

	//RESET ALL
		//reset isi array
	void reset() { //Note that the addresses haven't been changed for deletion purposes
		for (int a = 0; a < defvalue; a++) {
			spawn[a].reset();
		}
	}

	//Moving from Keeper to Spawner
	void addSpawn(Node* ab) {
		Blueprint newb = ab->getValue();
		
		for (int a = 0; a < defvalue; a++) {
			if (!spawn[a].Alive() && !spawn[a].Dying()) {
				spawn[a].setType(newb.getType());
				spawn[a].setSpawnPosition(newb.getPos());
				spawn[a].Load();
				spawn[a].Live();
				fspawn = a;
				break;
			}
		}
	}

	char newSpawn() {
		return spawn[fspawn].getType();
	}

	//Returning position X
	int getPosX(int index) {
		return spawn[index].getPosX();
	}

	//Shooting objects
		//return index penghuni Keeper yang tertembak untuk diproses sesuai tipe dan skenario
	int Shoot(Event event, Stats& stat) { //Note that the addresses haven't been changed for deletion purposes
		int counter = 0;

		for (int a = 0; a < defvalue; a++) {
			if (spawn[a].isClicked(event.mouseButton.x, event.mouseButton.y) && !spawn[a].Dying()) {
				if (spawn[a].getType() == 'h') {
					spawn[a].Merder(true, stat); //kalau human, langsung mati tertembak
					continue;
				}
				else if (spawn[a].getType() == 'z') {
					spawn[a].Damage(stat); //kalau zombie, damage

					//For each dead zombie, increases counter (Realistically speaking this is an impossible scenario, but better be safe)
					if (!spawn[a].Alive()) {
						counter++;
					}

					continue;
				}
			}
		}

		//Returns the counter
		return counter;
	}

	//Draw functions
		//drawing semua object yang ada di Keeper
	void drawSpawn(RenderWindow& window, Time frameTime) {
		for (int a = 0; a < defvalue; a++) {
			if (spawn[a].getType() == 'z') { //Zombies
				//If Alive
				if (spawn[a].Alive()) {
					if (spawn[a].getMove()) {
						spawn[a].animationMove(frameTime.asSeconds());
						spawn[a].moveSprite(frameTime.asSeconds());
					}
					spawn[a].drawSprite(window);
				}
				//Let them die. Don't let the real Zomu die tho :'3
				if (spawn[a].Dying()) {
					spawn[a].animationDead(frameTime.asSeconds());
					spawn[a].drawSprite(window);
				}
			}
			if (spawn[a].getType() == 'h') { //humans
				//if Alive
				if (spawn[a].Alive()) {
					spawn[a].animationMove(frameTime.asSeconds());
					spawn[a].moveSprite(frameTime.asSeconds());
					spawn[a].drawSprite(window);
				}

				//They're dying, why are you only watching them?!
				if (spawn[a].Dying()) {
					spawn[a].animationDead(frameTime.asSeconds());
					spawn[a].drawSprite(window);
				}
			}
		}
	}

	//Death functions
	void deathCheck(Stats& stat) {
		for (int a = 0; a < defvalue; a++) {
			if (spawn[a].getType() == 'h' && spawn[a].Alive() && !spawn[a].Dying()) {
				for (int b = 0; b < defvalue; b++) {
					if (a == b) {
						continue;
					}

					if (spawn[b].getType() == 'z' && spawn[b].Alive()) {
						spawn[a].checkCollision(spawn[b], stat);
					}
				}
			}
		}
	}

	int despawnCheck(Stats& stat) {
		int score = 0;

		for (int a = 0; a < defvalue; a++) {
			//If Health not zero, Zombie despawns, Player HP decreases
			if (spawn[a].getType() == 'z' && spawn[a].getPosX() <= 320 && spawn[a].Alive()) {
				spawn[a].Merder(false, stat);
			}
			//If Health not zero, Human despawns, Player Score increases
			if (spawn[a].getType() == 'h' && spawn[a].getPosX() <= 320 && spawn[a].Alive()) {
				spawn[a].Merder(false, stat);
				if (stat.CheckHP()) {
					score++;
				}
			}
		}

		return score;
	}

	//Tower Functions
	int towerCheck(int position) {
		int index = defvalue;

		for (int a = 0; a < defvalue; a++) {
			if (spawn[a].getType() == 'z' && spawn[a].Alive() && !spawn[a].Dying() && position == spawn[a].getSpawnPosition()) {
				index = a;
				break;
			}
		}

		return index;
	}

	void towerDeath(int index, Stats& stat) {
		if (!spawn[index].Dying()) {
			spawn[index].Merder(true, stat);
		}
	}

	//DEBUGGING FUNCTION
	void checkValue(int input = 100) {
		if (input == 100) {
			input = defvalue-1;
			cout << "-= New spawn =-" << endl;
		}
		else {
			cout << "-= Spawn No. " << input << "=-" << endl;
		}
		
		cout << "Type: " << spawn[input].getType() << endl;
		cout << "Speed: " << spawn[input].getSpeed() << endl;
		cout << "HP: " << spawn[input].getHP() << endl;
		cout << endl;
		cout << "Position X: " << spawn[input].getPosX() << endl;
		cout << "Position Y: " << spawn[input].getSpawnPosition() << endl;
		cout << endl;
		cout << "Alive: " << spawn[input].Alive() << endl;
		cout << "Dying: " << spawn[input].Dying() << endl;
		cout << endl;
	}
};