#pragma once
#include"stats.h"
#include<SFML/Audio.hpp>

//inheritence stuff

class Movable {
private:
protected:
	int hp_; //HP, 1 click = -1 HP
	float speed_; //speed multiplier

	//Spritestuffs
	IntRect rect;
	Texture texture;
	Sprite sprite;
	Texture _deadTexture;

	//frame controls
	int frame;
	int framemax_mv; //max frames in move
	int framemax_dd; //max frames in dead
	//move height & width
	float h_mv;
	float w_mv;
	int mvoffset;
	//dead height & width
	float h_dd;
	float w_dd;

	//x spawn offset
	int _xOffset;
	float _xAnimOffset;

	double _currentTimeAnimation;
	double _currentTimeMove;
	float _basespd;

	float mvanimspd;

	//For the (y) of the spawning location, sprite height = 110 (this is false?), door[1] = 335(?)
	int locationSpawn[5];
	int spawn;

	bool alive;
	bool dead;
	bool move;
public:
	//constructor
	Movable() {
		_xOffset = 1280;
		_xAnimOffset = 0;

		_currentTimeAnimation = 0;
		_currentTimeMove = 0;

		alive = false;
		dead = false;
		move = false;

		//sprite frame start
		frame = 1;
	}


	//PAM
	void setHP(int hp) {
		hp_ = hp;
	}
	int getHP() {
		return hp_;
	}

	void setSpeed(float speed) {
		speed_ = speed;
	}
	float getSpeed() {
		return speed_;
	}

	int getSpawnPosition() {
		return spawn;
	}

	void setSpawnPosition(int spawn_) {
		spawn = spawn_;
	}

	void setAlive(bool value) {
		alive = value;
	}
	void setMove(bool value) {
		move = value;
	}
	void setDead(bool value) {
		dead = value;
	}

	bool Alive() {
		return alive;
	}
	//dyink
	bool Dying() {
		return dead;
	}

	int getPosX() {
		return _xOffset - _xAnimOffset;
	}

	//virtual functions
	virtual void initialize() {}

	//functions
	FloatRect getGlobalBoundss() {
		return sprite.getGlobalBounds();
	}

	bool isClicked(int mouseX, int mouseY) {
		return sprite.getGlobalBounds().contains(mouseX, mouseY);
	}

	//Sprite animation
	//move animation
	void animationMove(double deltaTime) {
		_currentTimeAnimation += deltaTime;
		if (_currentTimeAnimation >= mvanimspd) {
			_currentTimeAnimation = 0;

			frame++;
			int x = (frame - 1) % 3; //0 1 2 0 1 2 0 1
			int y = (frame - 1) / 3; //0 0 0 1 1 1 2 2

			int spriteX = (x * h_mv) + mvoffset;
			float spriteY = y * w_mv;

			rect.left = spriteX;
			rect.top = spriteY;
			sprite.setTextureRect(rect);

			if (frame >= framemax_mv) {
				frame = 0;
			}
		}
	}

	//dead animation
	void animationDead(double deltaTime) {
		_currentTimeAnimation += deltaTime;
		if (_currentTimeAnimation >= 0.05f && frame < framemax_dd) {
			_currentTimeAnimation = 0;

			frame++;
			int x = (frame - 1) % 3; //Frame 1 = 0, Frame 2 = 1, Frame 3 = 2, Frame 4 = 0
			int y = (frame - 1) / 3; //Frame 1 = 0, Frame 2 = 0, Frame 3 = 0, Frame 4 = 1

			int spriteX = x * h_dd;
			int spriteY = y * w_dd;

			rect.left = spriteX;
			rect.top = spriteY;
			sprite.setTextureRect(rect);
		}

		if (_currentTimeAnimation >= 3) {
			_currentTimeAnimation = 0;
			frame = 0;
			dead = false;

			initialize();
		}
	}

	//Sprite movement
	void moveSprite(double deltaTime) {
		_currentTimeMove += deltaTime;
		if (_currentTimeMove >= 0.05f) {
			_currentTimeMove = 0;
			_xAnimOffset += (_basespd * speed_);
		}

		sprite.setPosition(_xOffset - _xAnimOffset, locationSpawn[spawn]);
	}

	//draw sprite
	void drawSprite(RenderWindow& window) {
		window.draw(sprite);
	}
};