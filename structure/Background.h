#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>

using namespace std;
using namespace sf;

//loading textures and sprites

class Background {
private:
	Texture _bgTexture;
	Sprite _bg;
	Texture _wallTexture;
	Sprite _wall;
	Texture _mbgTexture;
	Sprite _mbg;
public:
	//constructor
	Background() {
		if (!_bgTexture.loadFromFile("assets/images/roadn.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_bg.setTexture(_bgTexture);
		_bg.setPosition(0, 0);

		if (!_wallTexture.loadFromFile("assets/images/wallsh.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_wall.setTexture(_wallTexture);
		_wall.setPosition(0, 0);

		if (!_mbgTexture.loadFromFile("assets/images/roadn.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_mbg.setTexture(_mbgTexture);
		_mbg.setPosition(0, 0);
	}

	//functions
	void drawBackground(RenderWindow& window) {
		window.draw(_bg);
		window.draw(_wall);
	}

	void drawMainBG(RenderWindow& window) {
		window.draw(_mbg);
	}
};