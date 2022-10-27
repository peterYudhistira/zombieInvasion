#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>

using namespace std;
using namespace sf;

class HP {
private:
	Sprite _Health;
	int hp;
public:
	//constructor
	HP() {
		hp = 1;
	}

	//PAM
	int getHP() {
		return hp;
	}
	Sprite getHealth() {
		return _Health;
	}
	void setTexture(const Texture& texture) {
		_Health.setTexture(texture);
	}
	void setPosition(int x, int y) {
		_Health.setPosition(x, y);
	}

	//functions
	void reduceHP() {
		hp = 0;
	}
	void resetHP() {
		hp = 1;
	}
};

class Bullet {
private:
	Sprite _Bullet;
	int ammo;
public:
	//constructor
	Bullet() {
		ammo = 1;
	}

	//PAM
	int getAmmo() {
		return ammo;
	}
	Sprite getBullet() {
		return _Bullet;
	}
	void setTexture(const Texture& texture) {
		_Bullet.setTexture(texture);
	}
	void setPosition(int x, int y) {
		_Bullet.setPosition(x, y);
	}

	//functions
	void reduceAmmo() {
		ammo = 0;
	}

	//dual-functioning as reset();
	void reload() {
		ammo = 1;
	}
};

class Stats {
private:
	//Stat backgrounds
	Sprite _Statbar;
	Texture _StatbarTexture;

	//HP Stat details
	HP Health[7]; 
	int maxHP = 5;
	int curHP;

	//HP Stat details
	Bullet Ammo[8];
	int maxAmmo = 6;
	int curAmmo;

	//For heart colors
	Sprite HPsprite[2];
	Texture _HPtexture[2];

	//For bullet colors
	Sprite Bulletsprite[2];
	Texture _Bullettexture[2];
public:
	//constructor
	Stats() {
		//Stat backgrounds
		if (!_StatbarTexture.loadFromFile("assets/images/StatBar.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_Statbar.setTexture(_StatbarTexture);
		_Statbar.setPosition(0, 0);
		
		//set sprite for HP 1 & HP 0
		for (int a = 0; a <= 1; a++) {
			if (!_HPtexture[a].loadFromFile("assets/images/HP" + to_string(a) + ".png")) {
				cout << "Cannot open board image file." << endl;
			}
			HPsprite[a].setTexture(_HPtexture[a]);
		}
		//set sprite for Bullet 1 & Bullet 0
		for (int a = 0; a <= 1; a++) {
			if (!_Bullettexture[a].loadFromFile("assets/images/Bullet" + to_string(a) + ".png")) {
				cout << "Cannot open board image file." << endl;
			}
			Bulletsprite[a].setTexture(_Bullettexture[a]);
		}
		
		//set sprite for stats(Health)
		for (int a = 1; a <= maxHP; a++) {
			int x = (a - 1) * 75;
			int y = 720 - 75;

			Health[a].setTexture(_HPtexture[Health[a].getHP()]);
			Health[a].setPosition(x, y);
		}

		//set sprite for stats(Bullet)
		for (int a = 1; a <= maxAmmo; a++) {
			int x = 1280 - (a * 37);
			int y = 720 - 75;

			Ammo[a].setTexture(_Bullettexture[Ammo[a].getAmmo()]);
			Ammo[a].setPosition(x, y);
		}

		curHP = maxHP;
		curAmmo = maxAmmo;
	}

	//PAM
	int getCurHP() {
		return curHP;
	}
	int getCurAmmo() {
		return curAmmo;
	}

	//Functions

	//Display HP (background)
	void drawStatbg(RenderWindow& window) {
		window.draw(_Statbar);
	}

	//Display HP (Hearts)
	void drawHP(RenderWindow& window) {
		for (int a = 1; a <= maxHP; a++) {
			window.draw(Health[a].getHealth());
		}
	}
	//Display Ammo (Bullets)
	void drawAmmo(RenderWindow& window) {
		for (int a = 1; a <= maxAmmo; a++) {
			window.draw(Ammo[a].getBullet());
		}
	}

	//Reduce player HP
	void playerDamaged() {
		int index = 0;
		for (int a = 1; a <= maxHP; a++) {
			if (Health[a].getHP() == 0) {
				break;
			}
			index = a;
		}

		Health[index].reduceHP();
		Health[index].setTexture(_HPtexture[Health[index].getHP()]);
		curHP--;
	}

	//Reduce player ammo
	void playerShoot() {
		int index = 0;
		for (int a = 1; a <= maxAmmo; a++) {
			if (Ammo[a].getAmmo() == 0) {
				break;
			}
			index = a;
		}

		Ammo[index].reduceAmmo();
		Ammo[index].setTexture(_Bullettexture[Ammo[index].getAmmo()]);
		curAmmo--;
	}

	bool CheckHP() {
		if (curHP <= 0) {
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckAmmo() {
		if (curAmmo <= 0) {
			return false;
		}
		else {
			return true;
		}
	}

	void reset() {
		curHP = maxHP;
		for (int a = 1; a <= maxHP; a++) {
			Health[a].resetHP();
			Health[a].setTexture(_HPtexture[Health[a].getHP()]);
		}
		
		reload();
	}

	void reload() {
		curAmmo = maxAmmo;
		for (int a = 1; a <= maxAmmo; a++) {
			Ammo[a].reload();
			Ammo[a].setTexture(_Bullettexture[Ammo[a].getAmmo()]);
		}
	}
};