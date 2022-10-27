#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>

class Tower {
private:

	//texture dan sprite untuk tower
	IntRect rectTower;
	Texture _towerTexture;
	Sprite _tower;

	//ditto, untuk petir
	IntRect rectStrike;
	Texture _strikeTexture;
	Sprite _strike;

	int defaulttimer = 18; //cooldown sebelum sebuah tower aktif adalah 18 detik
	int defvalue = 20; //untuk jumlah maksimum entitas yang ada di Keeper

	//animator
	double _currentTimeAnimation = 0;
	int frame;
	bool tower = false;
	bool strike = false;

	double _shootTimer = 0;
	int _nextShoot;

	//simpler data-keeping purposes...
	int locationSpawn[5]; //tracking zombie yang ada di lane 1, 2, atau 3
	int location;

	//targeted zombie
	int target; //index
	int target_x; //posisi x nya
public:
	//constructor
	Tower() {
		frame = 0;
		target = defvalue;

		//for tower
		rectTower.left = 0;
		rectTower.top = 0;
		rectTower.width = 105;
		rectTower.height = 277;

		if (!_towerTexture.loadFromFile("assets/images/sprites/tower.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_tower.setTexture(_towerTexture);
		_tower.setTextureRect(rectTower);

		//for strike
		rectStrike.left = 0;
		rectStrike.top = 0;
		rectStrike.width = 276;
		rectStrike.height = 720;

		if (!_strikeTexture.loadFromFile("assets/images/sprites/strike.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_strike.setTexture(_strikeTexture);
		_strike.setTextureRect(rectStrike);
		
		//timer cooldown
		_nextShoot = rand() % 2 + defaulttimer;

		//location spawning y offset (For the thunder)
		locationSpawn[1] = 130;
		locationSpawn[2] = 330;
		locationSpawn[3] = 530;
	}

	//untuk draw
	bool Move() {
		return tower;
	}

	//untuk draw petir
	bool Strike() {
		return strike;
	}

	int getFrame() {
		return frame;
	}

	//menyimpan index, posisi x, serta lane target
	void setTarget(int _target, int _target_x) {
		target = _target;
		target_x = _target_x;

		int ly = 0 - 720 + locationSpawn[location] + 133;
		
		//data akan digunakan untuk spawn petir.
		_strike.setPosition(_target_x - 110, ly);
	}

	//getter untuk target yang akan disambar petir
	int getTarget() {
		return target;
	}

	//kalo nggak jadi nembak
	void cancelStrike() {
		tower = false;
		target = defvalue;
	}

	//functions
	void setup(int loc) {
		location = loc;
		if (loc == 1) {
			_tower.setPosition(60 + 15, -130);
		}
		else if (loc == 2) {
			_tower.setPosition(60 + 10, 60);
		}
		else if (loc == 3) {
			_tower.setPosition(60 + 5, 250);
		}
	}

	//resetting tower setelah game
	void reset() {
		tower = false;
		strike = false;
		frame = 0;
	}

	//menentukan apakah tower sedang aktif atau cooldown
	void moveStart(double deltaTime) {
		_shootTimer += deltaTime;
		if (_shootTimer >= _nextShoot) {
			_shootTimer = 0;
			//cooldown modifier
				//nggak pas 18 detik
			_nextShoot = rand() % 2 + defaulttimer;

			tower = true;
		}
	}

	//draw functions
	void drawTower(RenderWindow& window) {
		window.draw(_tower);
	}

	void drawStrike(RenderWindow& window) {
		window.draw(_strike);
	}

	//animations
		//Tower movement
	void toweranimation(double deltaTime) {
		_currentTimeAnimation += deltaTime;
		if (_currentTimeAnimation >= 0.05f) {
			_currentTimeAnimation = 0;

			frame++;
			int x = (frame - 1) % 3;
			int y = (frame - 1) / 3;
			int spriteX = x * 105;
			int spriteY = y * 277;

			rectTower.left = spriteX;
			rectTower.top = spriteY;
			_tower.setTextureRect(rectTower);

			if (frame >= 12) {
				frame = 0;
				tower = false;
				strike = true;
			}
		}
	}
		//Thunda movement and sound
	void strikeanimation(double deltaTime) {
		_currentTimeAnimation += deltaTime;
		if (_currentTimeAnimation >= 0.015f) {
			_currentTimeAnimation = 0;

			frame++;
			int x = (frame - 1) % 7;
			int y = (frame - 1) / 7;
			int spriteX = x * 276;
			int spriteY = y * 720;

			rectStrike.left = spriteX;
			rectStrike.top = spriteY;
			_strike.setTextureRect(rectStrike);

			if (frame >= 7) {
				frame = 0;
				target = defvalue;
				strike = false;
			}
		}
	}
};