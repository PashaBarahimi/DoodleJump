#include "Menu.hpp"

Menu::Menu(const GamePlay::Addresses addresses, const GamePlay::Sizes sizes, const GamePlay::Locations locations) : addresses_(addresses), sizes_(sizes), locations_(locations)
{
	window_ = new Window(sizes.background.x, sizes.background.y, "Doodle Jump");
	menuBoard_ = new GameBoard(sizes_.background, static_cast<double>(DELAY) / 1000);
	Point menuDoodlerLocation = { locations_.menuPlatform.x + sizes_.normalAndMovablePlatforms.x / 2 - sizes_.doodler[0].x / 2,locations_.menuPlatform.y - sizes_.doodler[0].y };
	menuDoodler_ = new Doodler(menuDoodlerLocation, sizes_.doodler, menuBoard_);
	menuPlatform_ = new NormalPlatform(locations_.menuPlatform, sizes_.normalAndMovablePlatforms, menuBoard_, menuDoodler_);
	menuSpaceship_ = new Spaceship(menuBoard_, locations_.spaceship, sizes_.spaceship);
}

void Menu::mainMenu()
{
	int highScore = 0;
	string name = "doodler";
	readScoreFromFile(name, highScore);
	srand(static_cast<unsigned>(time(nullptr)));
	while (true)
	{
		GameBoard* menuBoard = new GameBoard(sizes_.background, DELAY);
		if (!checkForMenuEvents())
			return;
		while (true)
		{
			GamePlay* game = new GamePlay(addresses_, sizes_, locations_, window_, highScore, name);
			const GamePlay::Actions action = game->process();
			writeScoreToFile(name, highScore);
			if (action == GamePlay::Actions::PlayAgain)
				continue;
			if (action == GamePlay::Actions::Menu)
				break;
			if (action == GamePlay::Actions::Quit)
				return;
		}
	}
}

void Menu::readScoreFromFile(string& name, int& score) const
{
	ifstream input;
	input.open(addresses_.nameAndScore);
	if (input.is_open())
	{
		input >> name >> score;
		input.close();
	}
	else
		writeScoreToFile(name, score);
}

void Menu::writeScoreToFile(string& name, int& score) const
{
	ofstream output;
	output.open(addresses_.nameAndScore);
	if (!output.is_open())
	{
		cerr << "ERROR OPENING FILE!";
		return;
	}
	output << name << '\n' << score;
	output.close();
}

void Menu::printMainMenu()
{
	window_->clear();
	window_->draw_img(addresses_.background);
	window_->draw_img(addresses_.mainMenu);
	window_->draw_img(addresses_.playButton[0], Rectangle(locations_.playButton, sizes_.playButton.x, sizes_.playButton.y));
	window_->draw_img(addresses_.normalPlatform, Rectangle(menuPlatform_->getTopLeftLocation(), menuPlatform_->getBottomRightLocation()));
	window_->draw_img(addresses_.doodler[static_cast<int>(menuDoodler_->getPrintStatus())], Rectangle(menuDoodler_->getTopLeftLocation(), menuDoodler_->getBottomRightLocation()));
	window_->draw_img(addresses_.spaceship[rand() % 2], Rectangle(menuSpaceship_->getTopLeftLocation(), menuSpaceship_->getBottomRightLocation()));
	window_->update_screen();
}

bool Menu::checkForButtonPress(Point topLeftLocation, sizeVector size, const Event& event)
{
	return event.get_mouse_position().x >= topLeftLocation.x && event.get_mouse_position().x <= topLeftLocation.x + size.x &&
		event.get_mouse_position().y >= topLeftLocation.y && event.get_mouse_position().y <= topLeftLocation.y + size.y;
}

bool Menu::checkForMenuEvents()
{
	while (true)
	{
		menuDoodlerMove();
		menuSpaceship_->move();
		printMainMenu();
		while (window_->has_pending_event())
		{
			Event event = window_->poll_for_event();
			switch (event.get_type())
			{
			case Event::QUIT:
				return false;
			case Event::LCLICK:
				if (checkForButtonPress(locations_.playButton, sizes_.playButton, event))
				{
					playButton();
					return true;
				}
				break;
			default:
				break;
			}
		}
		delay(DELAY);
	}
}

void Menu::playButton()
{
	window_->draw_img(addresses_.playButton[1], Rectangle(locations_.playButton, sizes_.playButton.x, sizes_.playButton.y));
	window_->update_screen();
	delay(BUTTON_PRESS_DELAY);
}

void Menu::menuDoodlerMove()
{
	menuPlatform_->setWasUnderDoodler();
	menuDoodler_->move();
	if (menuPlatform_->checkForCollision(sizes_.doodler[0]))
	{
		menuDoodler_->handleCollision(Doodler::Collision::Platform, menuPlatform_->getTopLeftLocation());
		window_->play_sound_effect(addresses_.jumpEffect);
	}
}

