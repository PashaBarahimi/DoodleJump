#include "include/menu.hpp"
#undef main

GamePlay::Addresses getAddresses()
{
	GamePlay::Addresses addresses;
	addresses.normalPlatform = "Files/Pictures/normal_platform.png";
	addresses.MovablePlatform = "Files/Pictures/movable_platform.png";
	addresses.breakablePlatform = {"Files/Pictures/breakable_platform.png", "Files/Pictures/breaking1.png", "Files/Pictures/breaking2.png", "Files/Pictures/breaking3.png"};
	addresses.background = "Files/Pictures/background.png";
	addresses.doodler = {"Files/Pictures/right.png", "Files/Pictures/left.png", "Files/Pictures/right_jumping.png", "Files/Pictures/left_jumping.png", "Files/Pictures/shooting.png"};
	addresses.scoreBackground = "Files/Pictures/score_background.png";
	addresses.comicFont = "Files/Fonts/comic.ttf";
	addresses.gameOver = "Files/Pictures/game_over.png";
	addresses.springs = {"Files/Pictures/compressed_spring.png", "Files/Pictures/spring.png"};
	addresses.enemies = {"Files/Pictures/normal_enemy.png", "Files/Pictures/movable_enemy.png"};
	addresses.stars = {"Files/Pictures/stars1.png", "Files/Pictures/stars2.png", "Files/Pictures/stars3.png"};
	addresses.pauseButton = "Files/Pictures/pause2.png";
	addresses.pauseMenu = "Files/Pictures/pause_cover.png";
	addresses.resumeButton = {"Files/Pictures/resume.png", "Files/Pictures/resume_pressed.png"};
	addresses.menuButton = {"Files/Pictures/menu.png", "Files/Pictures/menu_pressed.png"};
	addresses.playAgainButton = {"Files/Pictures/play_again.png", "Files/Pictures/play_again_pressed.png"};
	addresses.mainMenu = "Files/Pictures/main_menu.png";
	addresses.playButton = {"Files/Pictures/play.png", "Files/Pictures/play_pressed.png"};
	addresses.spaceship = {"Files/Pictures/spaceship_on.png", "Files/Pictures/spaceship_off.png"};
	addresses.pistol = "Files/Pictures/pistol.png";
	addresses.nameAndScore = "Files/nas.esl";
	addresses.dynamicMap = "Files/Maps/sequence.txt";
	addresses.staticMap = "Files/Maps/map.txt";
	addresses.jumpEffect = "Files/Sound Effects/jump.wav";
	addresses.breakPlatformEffect = "Files/Sound Effects/break_platform.wav";
	addresses.jumpOnMonsterEffect = "Files/Sound Effects/jump_on_monster.wav";
	addresses.monsterCrashEffect = "Files/Sound Effects/monster_crash.wav";
	addresses.springEffect = "Files/Sound Effects/spring.wav";
	addresses.fallEffect = "Files/Sound Effects/falling.wav";
	addresses.nearEnemyEffect = "Files/Sound Effects/near_enemy.wav";
	addresses.pistolShootEffect = "Files/Sound Effects/pistol_shoot.wav";
	addresses.pistolKilledMonsterEffect = "Files/Sound Effects/pistol_killed_enemy.wav";
	return addresses;
}

GamePlay::Sizes getSizes()
{
	GamePlay::Sizes sizes;
	sizes.normalAndMovablePlatforms = {75, 22};
	sizes.breakablePlatform = {{75, 22}, {75, 25}, {75, 31}, {75, 37}};
	sizes.background = {400, 640};
	sizes.doodler = {{77, 75}, {77, 75}, {77, 75}, {77, 75}, {77, 75}};
	sizes.scoreBackground = {400, 60};
	sizes.springs = {{35, 29}, {35, 50}};
	sizes.enemies = {{85, 115}, {85, 48}};
	sizes.stars = {60, 45};
	sizes.pauseButton = {20, 30};
	sizes.resumeButton = {140, 50};
	sizes.menuButton = {140, 51};
	sizes.playAgainButton = {140, 51};
	sizes.nameEdit = {170, 50};
	sizes.playButton = {140, 50};
	sizes.spaceship = {150, 173};
	sizes.pistol = {14, 14};
	return sizes;
}

GamePlay::Locations getLocations()
{
	GamePlay::Locations locations;
	locations.scoreLocation = {5, 5};
	locations.stars = {8, 0};
	locations.pauseButton = {359, 6};
	locations.resumeButton = {200, 450};
	locations.menuButton = {250, 500};
	locations.playAgainButton = {150, 430};
	locations.gameOverScore = {265, 240};
	locations.overallHighScore = {265, 285};
	locations.name = {230, 325};
	locations.playButton = {70, 190};
	locations.menuPlatform = {50, 550};
	locations.spaceship = {250, 30};
	return locations;
}

int main()
{
	Menu menu = Menu(getAddresses(), getSizes(), getLocations());
	menu.mainMenu();
	return 0;
}