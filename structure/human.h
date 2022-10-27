#pragma once
#include"movable.h"
#include"zombie.h"

class Human : public Movable{
private:
	SoundBuffer _zmatkSB;
	Sound _zmatk;
public:
	//constructor
	Human() {
		hp_ = 1;
		speed_ = 1;

		_basespd = 2.0f;

		framemax_dd = 8;
		h_dd = 131;
		w_dd = 120;
		framemax_mv = 8;
		h_mv = 131;
		w_mv = 120;
		mvoffset = 50;
		mvanimspd = 0.05f;

		//set texture IntRect
		rect.left = 50;
		rect.top = 0;
		rect.width = 81;
		rect.height = 120;

		//file loader
		if (!texture.loadFromFile("assets/images/sprites/hmRun.png")) {
			cout << "Cannot open board image file." << endl;
		}
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		if (!_deadTexture.loadFromFile("assets/images/sprites/hmDead.png")) {
			cout << "Cannot open board image file." << endl;
		}

		//location spawning y offset (assumption)
		locationSpawn[1] = 150;
		locationSpawn[2] = 350;
		locationSpawn[3] = 550;

		//zombie attack sound
		if (!_zmatkSB.loadFromFile("assets/sound/ZombieBite.ogg")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_zmatk.setBuffer(_zmatkSB);
	}
	
	//PAM

	//virtual functions

	//functions
	void Live() {
		alive = true;
		initialize();
	}

	//murder za hooman
	void Merder(bool HealthZero, Stats& stat) {
		alive = false;

		if (HealthZero == true) {
			//stats-1
			stat.playerDamaged();

			//make dead animation code? is at Board.h & scroll down
			dead = true;

			rect.left = 0;
			rect.top = 0;
			rect.width = 131;
			rect.height = 120;

			frame = 1;
			sprite.setTexture(_deadTexture);
			sprite.setTextureRect(rect);
			sprite.setPosition(_xOffset - _xAnimOffset - 50, locationSpawn[spawn]);

			return;
		}

		initialize();
	}

	void reset() {
		alive = false;
		dead = false;
		
		initialize();
	}

	//stat initialization... or just position and stuff actually.
	void initialize() {
		rect.left = 50;
		rect.top = 0;
		rect.width = 81;
		rect.height = 120;
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		frame = 1;
		_xAnimOffset = 0;
		spawn = rand() % 3 + 1;
		sprite.setPosition(_xOffset, locationSpawn[spawn]);
	}

	void checkCollision(Zombie zombie, Stats& stat) {
		FloatRect bounds = sprite.getGlobalBounds();

		if (zombie.Dying()) {
			return;
		}

		if (bounds.intersects(zombie.getGlobalBoundss())) {
			_zmatk.play();
			Merder(true, stat);
		}
	}
};