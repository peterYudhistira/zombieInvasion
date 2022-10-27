#pragma once
#include <SFML/Audio.hpp>
#include"zombie.h"
#include"human.h"
#include"tower.h"
#include"scoreboard.h"

using namespace std;
using namespace sf;

class CBoard {
private:
	Zombie zomu_[10];
	Human hooman_[10];

	int counter = 0;

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

			for (int a = 0; a < 10; a++) {
				if (zomu_[a].isClicked(event.mouseButton.x, event.mouseButton.y) && !zomu_[a].Dying()) {
					zomu_[a].Damage(stat);

					if (!zomu_[a].Alive()) {
						score.addScore(false);
					}

					return;
				}
			}
			for (int a = 0; a < 10; a++) {
				if (hooman_[a].isClicked(event.mouseButton.x, event.mouseButton.y) && !hooman_[a].Dying()) {
					hooman_[a].Merder(true, stat);
					return;
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
		for (int a = 0; a < 10; a++) {
			zomu_[a].reset();
			hooman_[a].reset();
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
		for (int a = 0; a < 10; a++) {
			if (hooman_[a].Alive() && !hooman_[a].Dying()) {
				for (int b = 0; b < 10; b++) {
					if (zomu_[b].Alive()) {
						hooman_[a].checkCollision(zomu_[b], stat);
					}
				}
			}
		}
		
		for (int a = 0; a < 10; a++) {
			//Wall despawn condition checker
			if (zomu_[a].getPosX() <= 320 && zomu_[a].Alive()) {
				zomu_[a].Merder(false, stat);
			}
			if (hooman_[a].getPosX() <= 320 && hooman_[a].Alive()) {
				hooman_[a].Merder(false, stat);
				if (stat.CheckHP()) {
					score.addScore(true);
				}
			}

			//Animating & drawing
			if (zomu_[a].Alive()) {
				if(zomu_[a].getMove()) {
					zomu_[a].animationMove(frameTime.asSeconds());
					zomu_[a].moveSprite(frameTime.asSeconds());
				}
				zomu_[a].drawSprite(window);
			}
			if (hooman_[a].Alive()) {
				hooman_[a].animationMove(frameTime.asSeconds());
				hooman_[a].moveSprite(frameTime.asSeconds());
				hooman_[a].drawSprite(window);
			}

			//They're dying, why are you only watching them?!
			if (hooman_[a].Dying()) {
				hooman_[a].animationDead(frameTime.asSeconds());
				hooman_[a].drawSprite(window);
			}

			//Let them die. Don't let the real Zomu die tho :'3
			if (zomu_[a].Dying()) {
				zomu_[a].animationDead(frameTime.asSeconds());
				zomu_[a].drawSprite(window);
			}
		}
	}

	void drawTowermv(RenderWindow& window, Time frameTime) {
		for (int a = 1; a <= 3; a++) {
			tower[a].moveStart(frameTime.asSeconds());

			if (tower[a].Move() && tower[a].getTarget() == 10) {
				int index = 10;
				for (int b = 0; b < 10; b++) {
					if (zomu_[b].Alive() && !zomu_[b].Dying() && a == zomu_[b].getSpawnPosition()) {
						index = b;
						break;
					}
				}

				if (index == 10) {
					tower[a].cancelStrike();
				}
				else {
					tower[a].setTarget(index, zomu_[index].getPosX());
				}
			}
			
			if (tower[a].Move() && tower[a].getTarget() != 10) {
				tower[a].toweranimation(frameTime.asSeconds());
			}

			if (tower[a].Strike() && tower[a].getTarget() != 10) {
				_ltstrike.play();
				tower[a].strikeanimation(frameTime.asSeconds());
				if (tower[a].getFrame() == 6) {
					if (!zomu_[tower[a].getTarget()].Dying()) {
						zomu_[tower[a].getTarget()].Merder(true, stat);
					}
				}
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
		int index = 10;

		for (int a = 0; a < 10; a++) {
			if (!zomu_[a].Alive() && !zomu_[a].Dying()) {
				index = a;
				break;
			}
		}

		if (index == 10) {
			return;
		}

		int sfx = rand() % 3;
		_zm[sfx].play();

		zomu_[index].Live();

		if (humanSpawn == true) {
			if (zomu_[index].getSpawnPosition() == hooman_[humindexF].getSpawnPosition()) {
				zomu_[index].spawnChange(hooman_[humindexF].getSpawnPosition());
			}
			humanSpawn = false;
		}
	}

	void spawnHuman() {
		int index = 10;

		for (int a = 0; a < 10; a++) {
			if (!hooman_[a].Alive() && !hooman_[a].Dying()) {
				index = a;
				break;
			}
		}

		if (index == 10) {
			return;
		}

		hooman_[index].Live();
		humindexF = index;
	}
};