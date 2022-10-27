#pragma once
#include <SFML/Audio.hpp>
//#include"zombie.h"
//#include"human.h"
#include"tower.h"
#include"scoreboard.h"
#include"spawn.h"

using namespace std;
using namespace sf;

class CBoard {
private:
	Spawn spawn_[20];

	int counter = 0;
	int defvalue = 20; // <- Number of space in array

	double spawnertime = 0;

	//human spawn special condition setter
	bool humanSpawn = false;
	int humindexF = 0;

	//Player stats
	Stats stat;

	//Reload CD Timer (Should've been in "stats.h" but meh)
	Font CDFont;
	Text CDText;
	int CDT = 8;
	double ReloadCD = CDT-1;

	//Score
	Scoreboard score;

	//Tower
	Tower tower[5];

	//Shooting
	SoundBuffer _shootSB;
	Sound _shoot;

	//Tower-Sound
	SoundBuffer _ltstrikeSB;
	Sound _ltstrike;

	//Zombie sounds
	SoundBuffer _zmSB[3];
	Sound _zm[3];
public:
	//constructor
	CBoard() {
		if (!CDFont.loadFromFile("assets/fonts/kenvector_future.ttf")) {
			cout << "Could not load font file.";
		}
		CDText.setFont(CDFont);
		CDText.setCharacterSize(30);
		CDText.setPosition(1280 - 275, 720 - 50);
		CDText.setOutlineThickness(3);

		for (int a = 1; a <= 3; a++) {
			tower[a].setup(a);
		}

		//shooting sound
		if (!_shootSB.loadFromFile("assets/sound/shoot.wav")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_shoot.setBuffer(_shootSB);

		//lightning sound
		if (!_ltstrikeSB.loadFromFile("assets/sound/towerlightning.wav")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_ltstrike.setBuffer(_ltstrikeSB);

		//zombie sound
		if (!_zmSB[0].loadFromFile("assets/sound/Groan.ogg")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_zm[0].setBuffer(_zmSB[0]);
		if (!_zmSB[1].loadFromFile("assets/sound/Groan2.ogg")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_zm[1].setBuffer(_zmSB[1]);
		if (!_zmSB[2].loadFromFile("assets/sound/Groan4.ogg")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_zm[2].setBuffer(_zmSB[2]);
	}

	//Active functions
	//When clicking...
	void Shoot(Event event) {
		if (stat.CheckAmmo()) {
			stat.playerShoot();
			_shoot.play();

			for (int a = 0; a < defvalue; a++) {
				if (spawn_[a].isClicked(event.mouseButton.x, event.mouseButton.y) && !spawn_[a].Dying()) {
					if (spawn_[a].getType() == 'h') {
						spawn_[a].Merder(true, stat);
						return;
					}
					else if (spawn_[a].getType() == 'z') {
						spawn_[a].Damage(stat);

						if (!spawn_[a].Alive()) {
							score.addScore(false);
						}

						return;
					}
				}
			}
		}
	}

	void Reload() {
		if (ReloadCD >= CDT-1 && stat.getCurAmmo() == 0) {
			stat.reload();
			ReloadCD = 0;
		}
	}

	void reset() {
		stat.reset();
		score.reset();
		for (int a = 0; a < defvalue; a++) {
			spawn_[a].reset();
		}
		counter = 0;
		spawnertime = 0;
		humanSpawn = false;
		humindexF = 0;
		ReloadCD = CDT - 1;
	}

	//Background functions
	bool checkHP() {
		return stat.CheckHP();
	}

	void towerDraw(RenderWindow& window) {
		for (int a = 1; a <= 3; a++) {
			tower[a].drawTower(window);
			if (tower[a].Strike()) {
				tower[a].drawStrike(window);
			}
		}
	}

	void statDraw(RenderWindow& window) {
		stat.drawStatbg(window);
		stat.drawHP(window);
		stat.drawAmmo(window);
		CDTimer(window);
		score.displayScore(window);
	}

	void CDTimer(RenderWindow& window) {
		int timer = CDT - ReloadCD;
		CDText.setString(to_string(timer));

		window.draw(CDText);
	}

	void RCD(double deltaTime) {
		if (ReloadCD < CDT) {
			ReloadCD += deltaTime;
		}

		if (ReloadCD > CDT) {
			ReloadCD = CDT;
		}
	}

	void drawSpawn(RenderWindow& window, Time frameTime) {
		//Human death condition checker
		for (int a = 0; a < defvalue; a++) {
			if (spawn_[a].getType() == 'h' && spawn_[a].Alive() && !spawn_[a].Dying()) {
				for (int b = 0; b < defvalue; b++) {
					if (spawn_[b].getType() == 'z' && spawn_[b].Alive()) {
						spawn_[a].checkCollision(spawn_[b], stat);
					}
				}
			}
		}
		
		for (int a = 0; a < defvalue; a++) {
			//Wall despawn condition checker
			if (spawn_[a].getType() == 'z' && spawn_[a].getPosX() <= 320 && spawn_[a].Alive()) {
				spawn_[a].Merder(false, stat);
			}
			if (spawn_[a].getType() == 'h' && spawn_[a].getPosX() <= 320 && spawn_[a].Alive()) {
				spawn_[a].Merder(false, stat);
				if (stat.CheckHP()) {
					score.addScore(true);
				}
			}

			//Animating & drawing
			if (spawn_[a].getType() == 'z') { //Zombies
				//If Alive
				if (spawn_[a].Alive()) {
					if (spawn_[a].getMove()) {
						spawn_[a].animationMove(frameTime.asSeconds());
						spawn_[a].moveSprite(frameTime.asSeconds());
					}
					spawn_[a].drawSprite(window);
				}
				//Let them die. Don't let the real Zomu die tho :'3
				if (spawn_[a].Dying()) {
					spawn_[a].animationDead(frameTime.asSeconds());
					spawn_[a].drawSprite(window);
				}
			}
			if (spawn_[a].getType() == 'h') { //humans
				//if Alive
				if (spawn_[a].Alive()) {
					spawn_[a].animationMove(frameTime.asSeconds());
					spawn_[a].moveSprite(frameTime.asSeconds());
					spawn_[a].drawSprite(window);
				}

				//They're dying, why are you only watching them?!
				if (spawn_[a].Dying()) {
					spawn_[a].animationDead(frameTime.asSeconds());
					spawn_[a].drawSprite(window);
				}
			}
		}
	}

	void drawTowermv(RenderWindow& window, Time frameTime) {
		for (int a = 1; a <= 3; a++) {
			tower[a].moveStart(frameTime.asSeconds());
			if (tower[a].Move() && tower[a].getTarget() == defvalue) {
				int index = defvalue;
				cout << "shit ass long texts bacot" << endl;

				for (int b = 0; b < defvalue; b++) {
					if (spawn_[b].getType() == 'h') {
						continue;
					}
					cout << "shit ass long texts BBBBBBBB" << endl;
					if (spawn_[b].Alive() && !spawn_[b].Dying() && a == spawn_[b].getSpawnPosition()) {
						index = b;
						break;

						cout << "ZOMBIIIIIIIIIII " << spawn_[b].getSpawnPosition() << " " << spawn_[b].getPosX() << endl;
						cout << "Goooo tower " << a << endl;
					}
				}

				if (index == defvalue) {
					tower[a].cancelStrike();
					cout << "shit ass long texts bruh" << endl;
				}
				else {
					tower[a].setTarget(index, spawn_[index].getPosX());
					cout << "shit ass long long texts" << endl;
				}
			}
			
			if (tower[a].Move() && tower[a].getTarget() != defvalue) {
				tower[a].toweranimation(frameTime.asSeconds());
				cout << "yolo yolo (uwu)" << endl;
			}

			if (tower[a].Strike() && tower[a].getTarget() != defvalue) {
				_ltstrike.play();
				tower[a].strikeanimation(frameTime.asSeconds());
				if (tower[a].getFrame() == 6) {
					if (!spawn_[tower[a].getTarget()].Dying()) {
						spawn_[tower[a].getTarget()].Merder(true, stat);
					}
				}
				cout << "why did you uwu" << endl;
			}
		}
	}

	void spawn(RenderWindow& window, double deltaTime, Time frametime) {
		spawnertime += deltaTime;

		if (spawnertime >= 3.0f) {
			spawnertime = 0;
			
			if (counter >= 3) {
				spawnHuman();
				int counter2 = rand() % 5;
				counter -= counter2;
				humanSpawn = true;
			}
			else {
				spawnZombie();
				counter++;
			}
		}

		drawSpawn(window, frametime);
	}

	void spawnZombie() {
		int index = defvalue;

		for (int a = 0; a < defvalue; a++) {
			if (spawn_[a].getType() == 'h') {
				continue;
			}

			if (!spawn_[a].Alive() && !spawn_[a].Dying()) {
				index = a;
				break;
			}
		}

		if (index == defvalue) {
			return;
		}

		int sfx = rand() % 3;
		_zm[sfx].play();

		spawn_[index].Live('z'); //I think its this shitto, yes

		if (humanSpawn == true) {
			if (spawn_[index].getSpawnPosition() == spawn_[humindexF].getSpawnPosition()) {
				spawn_[index].spawnChange(spawn_[humindexF].getSpawnPosition());
			}
			humanSpawn = false;
		}

		cout << "Zombie has appeared!" << endl;
	}

	void spawnHuman() {
		int index = defvalue;

		for (int a = 0; a < defvalue; a++) {
			if (spawn_[a].getType() == 'z') {
				continue;
			}

			if (!spawn_[a].Alive() && !spawn_[a].Dying()) {
				index = a;
				break;
			}
		}

		if (index == defvalue) {
			return;
		}

		spawn_[index].Live('h');
		humindexF = index;

		cout << "Human has appeared" << endl;
	}
};