#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include "rsdl.hpp"
#include "GameBoard.hpp"
#include "Doodler.hpp"
#include "Platform.hpp"
#include "NormalPlatform.hpp"
#include "MovablePlatform.hpp"
#include "BreakablePlatform.hpp"
#include "Spring.hpp"
#include "Enemy.hpp"
#include "NormalEnemy.hpp"
#include "MovableEnemy.hpp"
#include "Pistol.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

typedef Point sizeVector;

constexpr int DELAY = 15; // in milliseconds

constexpr int BUTTON_PRESS_DELAY = 200; // in  milliseconds

constexpr int NAME_LENGTH = 10;

class GamePlay
{
public:
	struct Addresses
	{
		string background;
		string normalPlatform;
		string MovablePlatform;
		vector<string> breakablePlatform;
		vector<string> doodler;
		vector<string> enemies;
		string comicFont;
		string scoreBackground;
		string gameOver;
		vector<string> springs;
		vector<string> stars;
		string pauseButton;
		string pauseMenu;
		vector<string> resumeButton;
		vector<string> menuButton;
		vector<string> playAgainButton;
		string mainMenu;
		vector<string> playButton;
		vector<string> spaceship;
		string nameAndScore;
		string staticMap;
		string dynamicMap;
		string pistol;
		string jumpEffect;
		string springEffect;
		string monsterCrashEffect;
		string jumpOnMonsterEffect;
		string breakPlatformEffect;
		string fallEffect;
		string nearEnemyEffect;
		string pistolShootEffect;
		string pistolKilledMonsterEffect;
	};
	struct Sizes
	{
		sizeVector background;
		sizeVector normalAndMovablePlatforms;
		vector<sizeVector> breakablePlatform;
		vector<sizeVector> doodler;
		vector<sizeVector> enemies;
		sizeVector scoreBackground;
		vector<sizeVector> springs;
		sizeVector stars;
		sizeVector pauseButton;
		sizeVector resumeButton;
		sizeVector menuButton;
		sizeVector playAgainButton;
		sizeVector nameEdit;
		sizeVector playButton;
		sizeVector spaceship;
		sizeVector pistol;
	};
	struct Locations
	{
		Point scoreLocation;
		Point stars;
		Point pauseButton;
		Point resumeButton;
		Point menuButton;
		Point playAgainButton;
		Point gameOverScore;
		Point overallHighScore;
		Point name;
		Point playButton;
		Point menuPlatform;
		Point spaceship;
	};
	enum class Actions
	{
		PlayAgain,
		Menu,
		Quit
	};
	GamePlay(const Addresses addresses, const Sizes sizes, Locations location, Window* window, int& highScore, string& name);
	~GamePlay();
	Actions process();
private:
	struct Sequence
	{
		int start_;
		int end_;
		int totalHeight_;
		vector<string> types_;
		vector<int> x_;
		vector<int> y_;
		Sequence(int start, int end, int totalHeight, vector<string> types, vector<int> x, vector<int> y);
	};
	vector<Platform*> platforms_;
	vector<Spring*> springs_;
	vector<Enemy*> enemies_;
	vector<Pistol*> pistols_;
	Doodler* doodler_;
	GameBoard* board_;
	Addresses addresses_;
	Sizes sizes_;
	Locations locations_;
	Window* window_;
	bool gameOver_;
	bool fellAfterGameOver_;
	int& highScore_;
	string& name_;
	vector<Sequence> sequences_;
	int sequenceStart_;
	int sequenceHeight_;
	void printOnScreen() const;
	void processPlatformChanges();
	void processSpringChanges();
	void processEnemyChanges();
	void processPistolChanges();
	bool checkForInGameEvents();
	void moveCamera(int pixel);
	void checkForCollision();
	bool checkForPlatformCollision();
	bool checkForSpringCollision();
	bool checkForEnemyCollision();
	void printDoodler() const;
	void printPlatforms() const;
	void printSprings() const;
	void printEnemies() const;
	void printStars() const;
	void printPistols() const;
	int getHighScore() const { return doodler_->getScoreInScreen() + board_->getBase(); }
	bool fallingHandler();
	void moveCameraUp();
	Actions gameOverEvent() const;
	void printGameOver() const;
	static int getAppropriateDelay(unsigned int prevTime);
	void processPossibleChanges();
	static bool checkForButtonPress(Point topLeftLocation, sizeVector size, const Event& event);
	bool pause();
	void menuButton() const;
	void playAgainButton() const;
	int checkForOverallHighScore() const;
	void editName() const;
	void endEditingName(const ostringstream& nameStream, const string& prevName) const;
	bool update();
	void readStaticMap();
	void readValuesFromStaticMap(ifstream& input);
	Point getTopLeftFromMiddleDown(int x, int y, sizeVector size) const;
	bool readDynamicMap();
	void readValuesFromDynamicMap(ifstream& input);
	void addDataFromMap(int x, int y, string type, int base);
	void inGameSequenceAdd(int index, int base);
	void checkForNewSequence();
	void findSuitableSequence();
	bool isEnemyNear();
	void makePistol();
};

#endif