#ifndef MENU_HPP
#define MENU_HPP

#include "game_play.hpp"
#include "spaceship.hpp"
#include <ctime>
#include <fstream>

class Menu
{
public:
	void mainMenu();
	Menu(const GamePlay::Addresses addresses, const GamePlay::Sizes sizes, const GamePlay::Locations locations);
	~Menu();
	Window *window_;

private:
	GamePlay::Addresses addresses_;
	GamePlay::Sizes sizes_;
	GamePlay::Locations locations_;
	GameBoard *menuBoard_;
	Doodler *menuDoodler_;
	NormalPlatform *menuPlatform_;
	Spaceship *menuSpaceship_;
	static bool checkForButtonPress(Point topLeftLocation, sizeVector size, const Event &event);
	void printMainMenu();
	bool checkForMenuEvents();
	void playButton();
	void menuDoodlerMove();
	void readScoreFromFile(string &name, int &score) const;
	void writeScoreToFile(string &name, int &score) const;
};

#endif
