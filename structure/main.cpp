#include <iostream>
#include "Game.h"

using namespace std;

int main() {
	srand(time(NULL));
	CGame game;
	game.start();
	return 0;
}