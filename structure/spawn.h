#pragma once
#include"movable.h"

//ini adalah "Movable", entitas yang bergerak
	//bisa berupa zombie, bisa berupa human.
	//dibedakan dengan satu char.
class Spawn : public Movable{
private:
	char type = ' ';
	//If type = 'h' -> Human
	//If type = 'z' -> Zombie
	SoundBuffer _zmatkSB;
	Sound _zmatk;
public:
	//constructor
	Spawn() {
		
	}
		//setting tipe
	Spawn(char inputtype) {
		type = inputtype;

		//jika tipe sesuai, inisialisasi stat nya
		if (type == 'z' || type == 'h' ) {
			initialize();
		}
	}

	//PAM 
	void setType(char input) {
		type = input;
	}
	
	char getType() {
		return type;
	}

	//virtual functions
		// gjd :3
	//functions
	//Zombie
	bool getMove() {
		return move;
	}

		//state hidup
	void Live() {
		setAlive(true);

		if (type == 'z') {
			setMove(true);
		}
	}
	void Live(char input) {
		type = input;
		setAlive(true);

		if (type == 'z') {
			setMove(true);
		}
	}

	//CONFUSION
	//murder za zombie
		//fungsi untuk mengerjakan damage pada zombie
	void Damage(Stats& stat) {
		//kalau human, salah masuk fungsi
		if (type == 'h') {
			return;
		}

		//decrement hp
		hp_--;

		//kalau mati,
		if (hp_ == 0) {
			Merder(true, stat); //mati dengan animasi
		}
	}

	//murder za hooman
		//fungsi untuk membunuh human
	void Merder(bool HealthZero, Stats& stat) {
		if (type == 'h') {
			setAlive(false); //no longer alive

			//kalau mati karena terbunuh zombie
			if (HealthZero == true) {
				//nyawa pemain berkurang 1
				stat.playerDamaged();

				//make dead animation code? is at Board.h & scroll down
					//mati
				setDead(true);

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
		else if (type == 'z') {
			setAlive(false);
			setMove(false);

			if (HealthZero == false) {
				//stats-1
				stat.playerDamaged();
			}
			else {
				//make dead animation code? is at Board.h and scroll down
				setDead(true);

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
	}

	//note : sesuatu yang tidak Dead dan tidak Alive adalah sesuatu yang ada,
		//tapi tidak di-spawn
		//karena kondisi draw membutuhkan dead atau alive.
	void reset() { //Reset object to make its state neither dead or alive (empty)
		setAlive(false);
		setDead(false);
		type = ' ';
	}

	//stat initialization... or just position and stuff actually.
		//menginisialisasi HP dan movespeed.
		//tekstur dan audio di load di tempat lain
		//karena memakan banyak memori. 
	void initialize() {
		if (type == 'h') {
			inithuman();

			rect.left = 50;
			rect.top = 0;
			rect.width = 81;
			rect.height = 120;
			sprite.setTexture(texture);
			sprite.setTextureRect(rect);

			frame = 1;
			_xAnimOffset = 0;
			//spawn = rand() % 3 + 1;
			sprite.setPosition(_xOffset, locationSpawn[spawn]);
		}
		else if (type == 'z') {
			initzombie();

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
			//spawn = rand() % 3 + 1;
			sprite.setPosition(_xOffset, locationSpawn[spawn]);
		}
	}

	void Load() {
		initialize();
	}

		//pembantu untuk initialize human dan zombie
	void inithuman() {
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
	void initzombie() {
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

	//Human type function
		//untuk cek tabrakan human,	
		//jika tabrakan dengan zombie, human mati
	void checkCollision(Spawn spawn, Stats& stat) {
		if (type == 'z') {
			return;
		}

		FloatRect bounds = sprite.getGlobalBounds();

		if (spawn.Dying()) {
			return;
		}

		if (bounds.intersects(spawn.getGlobalBoundss())) {
			_zmatk.play();
			Merder(true, stat);
		}
	}

	//spawn location changer to prevent zombie from spawning at same lane as hooman
		//jadi gini
		//ehem2
		//speed zombie jauh lebih cepat dari human sehingga kalau zombie spawn tepat di
		//belakang human, nggak fair.
		//jadi solusinya :
	void spawnChange(int pos) {
		if (type == 'h') {
			return;
		}

		//...posisi zombie di random lagi sampai dapat lane yang berbeda dari lane
		//milik human terakhir yang di spawn.
		while (spawn == pos) {
			spawn = rand() % 3 + 1;
		}
			//lalu spawn
		sprite.setPosition(_xOffset, locationSpawn[spawn]);
	}
};