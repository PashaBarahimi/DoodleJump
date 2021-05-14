#include "Menu.hpp"
#undef main

GamePlay::Addresses getAddresses()
{
	GamePlay::Addresses addresses;
	addresses.normalPlatform = "Files/normal_platform.png";
	addresses.MovablePlatform = "Files/movable_platform.png";
	addresses.breakablePlatform = { "Files/breakable_platform.png" ,"Files/breaking1.png" ,"Files/breaking2.png" ,"Files/breaking3.png" };
	addresses.background = "Files/background.png";
	addresses.doodler = { "Files/right.png" ,"Files/left.png" ,"Files/right_jumping.png" ,"Files/left_jumping.png" ,"Files/shooting.png" };
	addresses.scoreBackground = "Files/score_background.png";
	addresses.comicFont = "Files/comic.ttf";
	addresses.gameOver = "Files/game_over.png";
	addresses.springs = { "Files/compressed_spring.png" ,"Files/spring.png" };
	addresses.enemies = { "Files/normal_enemy.png" ,"Files/movable_enemy.png" };
	addresses.stars = { "Files/stars1.png" ,"Files/stars2.png" ,"Files/stars3.png" };
	addresses.pauseButton = "Files/pause2.png";
	addresses.pauseMenu = "Files/pause_cover.png";
	addresses.resumeButton = { "Files/resume.png" ,"Files/resume_pressed.png" };
	addresses.menuButton = { "Files/menu.png" ,"Files/menu_pressed.png" };
	addresses.playAgainButton = { "Files/play_again.png" ,"Files/play_again_pressed.png" };
	addresses.mainMenu = "Files/main_menu.png";
	addresses.playButton = { "Files/play.png" ,"Files/play_pressed.png" };
	addresses.spaceship = { "Files/spaceship_on.png" ,"Files/spaceship_off.png" };
	addresses.pistol = "Files/pistol.png";
	addresses.nameAndScore = "Files/nas.esl";
	addresses.dynamicMap = "Files/sequence.txt";
	addresses.staticMap = "Files/map.txt";
	addresses.jumpEffect = "Files/jump.wav";
	addresses.breakPlatformEffect = "Files/break_platform.wav";
	addresses.jumpOnMonsterEffect = "Files/jump_on_monster.wav";
	addresses.monsterCrashEffect = "Files/monster_crash.wav";
	addresses.springEffect = "Files/spring.wav";
	addresses.fallEffect = "Files/falling.wav";
	addresses.nearEnemyEffect = "Files/near_enemy.wav";
	addresses.pistolShootEffect = "Files/pistol_shoot.wav";
	addresses.pistolKilledMonsterEffect = "Files/pistol_killed_enemy.wav";
	return addresses;
}

GamePlay::Sizes getSizes()
{
	GamePlay::Sizes sizes;
	sizes.normalAndMovablePlatforms = { 75, 22 };
	sizes.breakablePlatform = { {75,22},{75,25},{75,31},{75,37} };
	sizes.background = { 400, 640 };
	sizes.doodler = { {77,75},{77,75}, {77,75}, {77,75}, {77,75} };
	sizes.scoreBackground = { 400,60 };
	sizes.springs = { {35,29},{35,50} };
	sizes.enemies = { {85,115},{85,48} };
	sizes.stars = { 60,45 };
	sizes.pauseButton = { 20,30 };
	sizes.resumeButton = { 140,50 };
	sizes.menuButton = { 140,51 };
	sizes.playAgainButton = { 140,51 };
	sizes.nameEdit = { 170,50 };
	sizes.playButton = { 140,50 };
	sizes.spaceship = { 150,173 };
	sizes.pistol = { 14,14 };
	return sizes;
}

GamePlay::Locations getLocations()
{
	GamePlay::Locations locations;
	locations.scoreLocation = { 5,5 };
	locations.stars = { 8,0 };
	locations.pauseButton = { 359,6 };
	locations.resumeButton = { 200,450 };
	locations.menuButton = { 250,500 };
	locations.playAgainButton = { 150,430 };
	locations.gameOverScore = { 265,240 };
	locations.overallHighScore = { 265,285 };
	locations.name = { 230,325 };
	locations.playButton = { 70,190 };
	locations.menuPlatform = { 50,550 };
	locations.spaceship = { 250,30 };
	return locations;
}

int main()
{
	Menu* menu = new Menu(getAddresses(), getSizes(), getLocations());
	menu->mainMenu();
	return 0;
}