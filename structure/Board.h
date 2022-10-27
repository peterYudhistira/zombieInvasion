#pragma once
#include <SFML/Audio.hpp>
//#include"zombie.h"
//#include"human.h"
#include"tower.h"
#include"scoreboard.h"
#include"spawn.h"
#include"Keeper.h"
#include"Spawner.h"

using namespace std;
using namespace sf;

//ini untuk fungsi-fungsi di belakang layar

class CBoard {
private:
	Spawner spawner; //implementasi LL
	Keeper keeper; //pertamanya LL tapi gabisa, sekarang jadi array lagi

	int counter = 0;
	int defvalue = 20; // <- Number of space in array

	double spawnertime = 0; //timer untuk spawn

	//Player stats
	Stats stat;

	//Reload CD Timer (Should've been in "stats.h" but meh)
		//jika 8 detik belum habis, belum bisa reload.
	Font CDFont;
	Text CDText;
	int CDT = 8;
	double ReloadCD = CDT-1; //tapi yang keliatan adalah angka 7, karena angka 0 dihitung.

	//Score
	Scoreboard score;

	//Tower
	Tower tower[5]; //sebenarnya ada 3, tapi Cia kasih padding index 0 dan 4

	//Shooting
		//sound dan buffer untuk suara tembakan
	SoundBuffer _shootSB;
	Sound _shoot;

	//Tower-Sound
		//sound dan buffer untuk suara petir
	SoundBuffer _ltstrikeSB;
	Sound _ltstrike;

	//Zombie sounds
		//sound dan buffer untuk suara zombie
		//ada 3, nanti dirandom tiap spawn zombie
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
		//cek apakah peluru masih ada
			//kalo ada baru tembak
		if (stat.CheckAmmo()) {
			stat.playerShoot();
			_shoot.play();

			//ini untuk mendeteksi ada atau tidak, dan berapa objek yang sedang tertembak
			int ct = keeper.Shoot(event, stat);
			//jika ada zombie yang mati, score bertambah
			for (int a = 1; a <= ct; a++) {
				score.addScore(false);
			}
		}
	}
		//when pressing R...
	void Reload() {
		//jika cooldown masih ada, nggak bisa reload
		if (ReloadCD >= CDT-1 && stat.getCurAmmo() == 0) {
			stat.reload();
			ReloadCD = 0;
		}
	}

	//ini untuk reset semuanya ketika new game dan/atau game over
	void reset() {
		stat.reset();
		score.reset();
		keeper.reset();
		counter = 0;
		spawnertime = 0;
		ReloadCD = CDT - 1;
	}

	//Background functions
		//cek apakah nyawa pemain masih ada
	bool checkHP() {
		return stat.CheckHP();
	}
		//drawing tower
	void towerDraw(RenderWindow& window) {
		for (int a = 1; a <= 3; a++) {
			tower[a].drawTower(window);
			//ini jika tower sedang menembak
			if (tower[a].Strike()) {
				tower[a].drawStrike(window);
			}
		}
	}

	//menggambar HUD (skor, nyawa, counter peluru)
	void statDraw(RenderWindow& window) {
		stat.drawStatbg(window);
		stat.drawHP(window);
		stat.drawAmmo(window);
		CDTimer(window);
		score.displayScore(window);
	}

	//timer untuk reload cooldown
	void CDTimer(RenderWindow& window) {
		int timer = CDT - ReloadCD;
		CDText.setString(to_string(timer));

		window.draw(CDText);
	}

	void RCD(double deltaTime) {
		//jika reload masih belum penuh, as is.
		if (ReloadCD < CDT) {
			ReloadCD += deltaTime;
		}

		//jika sudah 8, stay 8 aja.
		if (ReloadCD > CDT) {
			ReloadCD = CDT;
		}
	}

	void drawSpawn(RenderWindow& window, Time frameTime) {
		//Human death condition checker
		keeper.deathCheck(stat);
		//Human gets to finish line checker
		int scorect = keeper.despawnCheck(stat);

		//jika lebih dari satu zombie mati, increment score nya sesuai.
		for (int a = 0; a < scorect; a++) {
			score.addScore(true);
		}

		//Animating & drawing
			//buat semua yang ada di layar
		keeper.drawSpawn(window, frameTime);
	}

	void drawTowermv(RenderWindow& window, Time frameTime) {
		//untuk draw aksi tower
		for (int a = 1; a <= 3; a++) {
			tower[a].moveStart(frameTime.asSeconds());
			//kalo sudah dapat target, initiate :
			if (tower[a].Move() && tower[a].getTarget() == defvalue) {
				int index = keeper.towerCheck(a);
				
				//kalau nggak ada zombie di lane ini, nggak jadi strike.
				if (index == defvalue) {
					tower[a].cancelStrike();
				}
				//else, get posisi zombienya
				else {
					int position = keeper.getPosX(index);
					tower[a].setTarget(index, position);
				}
			}

			//spawn sprite petir
			if (tower[a].Move() && tower[a].getTarget() != defvalue) {
				tower[a].toweranimation(frameTime.asSeconds());
			}

			if (tower[a].Strike() && tower[a].getTarget() != defvalue) {
				_ltstrike.play();
				tower[a].strikeanimation(frameTime.asSeconds());
				if (tower[a].getFrame() == 6) {
					keeper.towerDeath(tower[a].getTarget(), stat);
				}
			}
		}
	}
	
	//berkaitan dengan spawn
	void spawn(RenderWindow& window, double deltaTime, Time frametime) {
		spawnertime += deltaTime;

		//jika cooldown sudah selesai,
		if (spawnertime >= 3.0f) {
			spawnertime = 0;

			//jika Spawner kosong, Spawner akan random ulang 20 karakter.
			if (spawner.Empty()) {
				for (int a = 0; a < defvalue; a++) {
					spawner.spawn(); //Spawner.h --> Spawner --> spawn() --> ngisi linked list sebanyak 20
				}
			}
			
			//memindahkan isi Linked List (QUEUE) ke array movable yang aktif.
			keeper.addSpawn(spawner.deliverSpawn());

			//kalau zombie, maka akan play random 1 dari 3 sound effect zombie.
			char spawnType = keeper.newSpawn();
			if (spawnType == 'z') {
				int sfx = rand() % 3;
				_zm[sfx].play();
			}
			//fungsi Spawner yang mengatur setelah pop head ke Keeper.
			spawner.deliveryEnd();
		}

		//draw semua yang ada di Keeper.
		drawSpawn(window, frametime);
	}
};