#pragma once
#include"movable.h"

class Zombie : public Movable{
private:
	bool move = false;
public:
	//constructor
	Zombie() {
		_basespd = 2.5f;

		framemax_dd = 8;
		h_dd = 157;
		w_dd = 133;
		framemax_mv = 8;
		h_mv = 110;
		w_mv = 132.6666f;
		mvoffset = 0;
		mvanimspd = 0.075f;

		//set texture IntRect
		rect.left = 0;
		rect.top = 0;
		rect.width = 110;
		rect.height = 132.6666f;

		//file loader
		if (!texture.loadFromFile("assets/images/sprites/zmAttack.png")) {
			cout << "Cannot open board image file" << endl;
		}
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		if (!_deadTexture.loadFromFile("assets/images/sprites/zmDead.png")) {
			cout << "Cannot open board image file." << endl;
		}

		//location spawning y offset
		locationSpawn[1] = 130;
		locationSpawn[2] = 330;
		locationSpawn[3] = 530;
	}
	
	//PAM
	bool getMove() {
		return move;
	}

	//virtual functions

	//functions
	void Live() {
		alive = true;
		move = true;

		initialize();
	}

	//murder za zombie
	void Damage(Stats& stat) {
		hp_--;
		if (hp_ == 0) {
			Merder(true, stat);
		}
	}

	void Merder(bool HealthZero, Stats& stat) {
		alive = false;
		move = false;

		if (HealthZero == false) {
			//stats-1
			stat.playerDamaged();
		}
		else {
			//make dead animation code? is at Board.h and scroll down
			dead = true;

			rect.left = 0;
			rect.top = 0;
			rect.width = 159;
			rect.height = 133;

			sprite.setTexture(texture);
			sprite.setTextureRect(rect);

			frame = 1;
			sprite.setTexture(_deadTexture);
			sprite.setTextureRect(rect);
			sprite.setPosition(_xOffset - _xAnimOffset - 60, locationSpawn[spawn] + 10);

			return;
		}

		initialize();
	}

	void reset() {
		alive = false;
		move = false;

		initialize();
	}

	//stat initialization
	void initialize() {
		//random speed, random HP
		int randHP = rand() % 10 + 1;
		if (randHP <= 7) {
			hp_ = 3;
		}
		else {
			hp_ = 2;
		}

		int randSpd = rand() % 10 + 1;
		if (randSpd <= 1 && randSpd <= 5) {
			speed_ = 1.0f;
		}
		else if (randSpd < 5 && randSpd <= 8) {
			speed_ = 1.5f;
		}
		else {
			speed_ = 2.0f;
		}

		rect.left = 0;
		rect.top = 0;
		rect.width = 110;
		rect.height = 132.6666f;
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		frame = 1;
		_xAnimOffset = 0;
		spawn = rand() % 3 + 1;
		sprite.setPosition(_xOffset, locationSpawn[spawn]);
	}

	//spawn location changer to prevent zombie from spawning at same lane as hooman
	void spawnChange(int pos) {
		while (spawn == pos) {
			spawn = rand() % 3 + 1;
		}

		sprite.setPosition(_xOffset, locationSpawn[spawn]);
	}
};