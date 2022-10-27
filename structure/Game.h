#pragma once
#include "Background.h"
#include "Board.h"

using namespace std;
using namespace sf;

//class untuk membuat tekstur button.
	//meliputi tekstur, text, dan fungsi yang mendeteksi jika button tersebut di klik.
class Button {
private:
	Texture buttonTexture;
	Sprite button;
	Text buttonText;
	Font buttonFont;
public:
	Button() { }
	void loadImageButton(String filename) {
		if (!buttonTexture.loadFromFile(filename)) {
			cout << "Could not load button file";
		}
		button.setTexture(buttonTexture);
	}
	//Text related
	void setText(String text) {
		buttonText.setString(text);
	}
	void setFont(String fontPath) {
		if (!buttonFont.loadFromFile(fontPath)) {
			cout << "Could not load font file";
			return;
		}
		buttonText.setFont(buttonFont);
	}
	void setFontSize(int size) {
		buttonText.setCharacterSize(size);
	}
	Text getText() {
		return buttonText;
	}
	void setTextPosition(int x, int y) {
		buttonText.setPosition(x, y);
	}
	//End

	void setTexture(String filename) {
		if (!buttonTexture.loadFromFile(filename)) {
			cout << "Could not load button file";
		}
		button.setTexture(buttonTexture);
	}
	void setPosition(int x, int y) {
		button.setPosition(x, y);
	}
	Sprite getButton() {
		return button;
	}
	void draw(RenderWindow &window) {
		window.draw(button);
		window.draw(buttonText);
	}
	bool isClicked(int mouseX, int mouseY) {
		return button.getGlobalBounds().contains(mouseX, mouseY);
	}
};

//enum untuk menyatakan state dari game, apakah masih ada di main menu, sedang bermain, pause, atau game over.
enum GameState {
	MAINMENU,
	MAINGAME,
	EXIT,
	PAUSE,
	GAMEOVER
};

//object game yang menerima input user dan mengeluarkan respon.
class CGame {
private:
	//enum
	GameState _gameState;
	//tombol menu
	Button _playButton, _exitButton;
	//sound buffer dan sound untuk sfx klik button
	SoundBuffer _clickedSoundBuffer;
	Sound _clicked;
	//Machine in the Walls - The Binding of Isaac : Antibirth
	Music _backgroundMusic;

	//For cursor changing settings
	View fixed;
	Texture _cursorTexture;
	Sprite _cursor;

	//Custom Classes
	Background background;
	CBoard _board;
	
	//Measurements
		//time
	Clock _frameClock;
		//ukuran layar
	int width;
	int height;

	//tekstur dan text untuk main game, pause, dan game over
	Texture _GMture;
	Texture _Pauseture;
	Texture _GOture;
	Sprite _GMText;
	Sprite _PauseText;
	Sprite _GOText;
public:
	//constructor : loading semua yang dibutuhkan untuk menjalankan game
		//dimensi layar, state awal = main menu, aset-aset yang diperlukan
		//menggunakan error catching basic ("could not load x")
	CGame() {
		width = 1280;
		height = 720;
		_gameState = GameState::MAINMENU;
		_playButton.loadImageButton("assets/images/green_button04.png");
		_playButton.setFont("assets/fonts/kenvector_future.ttf");
		_playButton.setFontSize(48);
		_playButton.setPosition((width / 2) - 125, (height / 2) - 64 - 32 + 200);
		_playButton.setText("Play");
		_playButton.setTextPosition((width / 2) - 125 + 50, (height / 2) - 64 - 32 - 5 + 200);

		_exitButton.loadImageButton("assets/images/green_button04.png");
		_exitButton.setFont("assets/fonts/kenvector_future.ttf");
		_exitButton.setFontSize(48);
		_exitButton.setPosition((width / 2) - 125, (height / 2) - 64 + 64 + 200);
		_exitButton.setText("Exit");
		_exitButton.setTextPosition((width / 2) - 125 + 60, (height / 2) - 64 + 64 - 5 + 200);

		if (!_clickedSoundBuffer.loadFromFile("assets/sound/click1.ogg")) {
			cout << "Could not load clicked sound file" << endl;
		}
		_clicked.setBuffer(_clickedSoundBuffer);

		if (!_backgroundMusic.openFromFile("assets/music/Machine in the Walls.ogg")) {
			cout << "Could not load background music file" << endl;
		}

		//Menu texts
		if (!_GMture.loadFromFile("assets/images/text/title.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_GMText.setTexture(_GMture);
		_GMText.setPosition(140, 25);

		if (!_Pauseture.loadFromFile("assets/images/text/pause.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_PauseText.setTexture(_Pauseture);
		_PauseText.setPosition(302, 100);

		if (!_GOture.loadFromFile("assets/images/text/gameover.png")) {
			cout << "Cannot open board image file." << endl;
		}
		_GOText.setTexture(_GOture);
		_GOText.setPosition(140, 50);

		_cursorTexture.loadFromFile("assets/images/cursor.png");
		_cursor.setTexture(_cursorTexture);
		_cursor.setOrigin(32, 32);
	}

	//START Game function
		//setelah construct, memulai game.
		//game dimulai di main menu, bisa bermain(MAINGAME) atau exit(EXIT).
	void start() {
		RenderWindow window(VideoMode(width, height), "Zombie Invasion");
		
		_backgroundMusic.play();
		_backgroundMusic.setVolume(30);

		//ini untuk visibility cursor
		while (window.isOpen()) {
			if (_gameState == GameState::MAINMENU) {
				window.setMouseCursorVisible(true);
				mainMenu(window);
			}
			else if (_gameState == GameState::MAINGAME) {
				mainGame(window);
			}
			else if (_gameState == GameState::EXIT) {
				window.setMouseCursorVisible(true);
				window.close();
			}
			else if (_gameState == GameState::PAUSE) {
				window.setMouseCursorVisible(true);
				pause(window);
			}
			else if (_gameState == GameState::GAMEOVER) {
				window.setMouseCursorVisible(true);
				gameover(window);
			}
		}
	}

	//main menu
		//event yang ada : klik button play dan exit.
	void mainMenu(RenderWindow &window) {
		//Event Handling
		Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonReleased) {
				if (_playButton.isClicked(event.mouseButton.x, event.mouseButton.y)) {
					_playButton.loadImageButton("assets/images/green_button04.png");
					_clicked.play();
					_gameState = GameState::MAINGAME;
					break;
				}
				if (_exitButton.isClicked(event.mouseButton.x, event.mouseButton.y)) {
					_exitButton.loadImageButton("assets/images/green_button04.png");
					_clicked.play();
					_gameState = GameState::EXIT;
					break;
				}
			}
			if (event.type == Event::MouseButtonPressed) {
				if (_playButton.isClicked(event.mouseButton.x, event.mouseButton.y)) {
					_playButton.loadImageButton("assets/images/green_button03.png");
					break;
				}
				if (_exitButton.isClicked(event.mouseButton.x, event.mouseButton.y)) {
					_exitButton.loadImageButton("assets/images/green_button03.png");
					break;
				}
			}
		}
		//Draw Objects
		window.clear();
		background.drawMainBG(window);
		window.draw(_GMText);
		_playButton.draw(window);
		_exitButton.draw(window);
		window.display();
	}

	//main game
	void mainGame(RenderWindow& window) {
		//Setting cursor
			//cursor berupa crosshairs sekarang, untuk menembak zombie
		window.setMouseCursorVisible(false);
		fixed = window.getView();

		_cursor.setPosition(static_cast<Vector2f>(Mouse::getPosition(window)));
		
		//Stuffs
		Time frameTime = _frameClock.restart();
		
		//events :
			//keluar game dengan klik X
			//click anywhere in window, shoots a bullet
			//press R, reloads (has a cooldown)
			//press P, enters PAUSE state.
			//HP pemain habis, masuk state GAMEOVER.
		Event event;
		while (window.pollEvent(event)) {
			//klik dan pencet tombol
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseButtonReleased) {
				_board.Shoot(event);
			}
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::P) {
					_gameState = GameState::PAUSE;
					break;
				}
				if (event.key.code == Keyboard::R) {
					_board.Reload();
				}
			}
		}

		//kondisi untuk kalah
		if (!_board.checkHP()) {
			_gameState = GameState::GAMEOVER;
			return;
		}

		//Draw Objects (object harus digambar ulang tiap frame)
		window.clear();
		window.setView(fixed);
		background.drawBackground(window);

		_board.spawn(window, frameTime.asSeconds(), frameTime);
		_board.drawTowermv(window, frameTime);
		_board.towerDraw(window);
		_board.statDraw(window);
		_board.RCD(frameTime.asSeconds());

		window.draw(_cursor);
		
		window.display();
	}

	//gamestate pause
	//event :
		//pencet P, masuk ke MAINGAME lagi
	void pause(RenderWindow& window) {
		//pause menu
		Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::P) {
					_gameState = GameState::MAINGAME;
					break;
				}
			}
		}

		window.clear();
		background.drawMainBG(window);
		window.draw(_PauseText);
		window.display();
	}

	//gamestate game over
	//events :
		//klik X, keluar
		//click anywhere in window, back to MAINMENU state.
	void gameover(RenderWindow& window) {
		//gameover menu
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::MouseButtonReleased) {
				_gameState = GameState::MAINMENU;
				_board.reset();
			}
		}

		window.clear();
		background.drawMainBG(window);
		window.draw(_GOText);
		window.display();
	}
};