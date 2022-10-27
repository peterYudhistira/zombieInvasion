#pragma once
#include<iostream>
#include<fstream>
#include<SFML/Graphics.hpp>
#include<string>

using namespace std;
using namespace sf;

//untuk menyimpan skor dan menampilkan high score
	//karena waktu dan kekuatan mental yang menipis hanya satu top score yang ada
class Scoreboard {
private:
	//current score
	int curScore;
	//high score records
	int hsRecord; //?????

	//text display
	Font scoreFont;
	Text scoreText;
	Text hscoreText;
public:
	Scoreboard () {
		if (!scoreFont.loadFromFile("assets/fonts/kenvector_future.ttf")) {
			cout << "Could not load font file.";
		}
		scoreText.setFont(scoreFont);
		scoreText.setCharacterSize(30);
		scoreText.setPosition(0 + 400, 720 - 50);
		scoreText.setOutlineThickness(3);

		hscoreText.setFont(scoreFont);
		hscoreText.setCharacterSize(30);
		hscoreText.setPosition(0 + 700, 720 - 50);
		hscoreText.setOutlineThickness(3);

		hsRecord = 0;
		loadScore();

		curScore = 0;
	}

	void loadScore() {
		ifstream load("Highscore.dat", ios::binary);

		if (!load) {
			return;
		}

		load.read((char*)&hsRecord, sizeof(int));
		load.close();
	}

	void saveScore() {
		ofstream save("Highscore.dat", ios::binary);
		save.write((char*)&hsRecord, sizeof(int));
		save.close();
	}

	void addScore(bool human) {
		if (human == true) {
			curScore += 1000;
		}
		else {
			curScore += 500;
		}
	}

	void reset() {
		if (curScore >= hsRecord) {
			hsRecord = curScore;
			saveScore();
		}

		curScore = 0;
	}

	void displayScore(RenderWindow& window) {
		scoreText.setString(to_string(curScore));
		hscoreText.setString(to_string(hsRecord));
		window.draw(scoreText);
		window.draw(hscoreText);
	}
};