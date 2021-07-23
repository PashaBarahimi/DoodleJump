#include "GamePlay.hpp"

GamePlay::GamePlay(const Addresses addresses, const Sizes sizes, Locations locations, Window *window, int &highScore, string &name) : addresses_(addresses), sizes_(sizes), locations_(locations), window_(window), highScore_(highScore), name_(name)
{
	board_ = new GameBoard(sizes_.background, static_cast<double>(DELAY) / 1000);
	doodler_ = new Doodler({sizes_.background.x / 2 - sizes_.doodler[0].x / 2, sizes_.background.y - sizes_.doodler[0].y},
						   sizes_.doodler, board_);
	gameOver_ = false;
	fellAfterGameOver_ = false;
	sequenceStart_ = 0;
	sequenceHeight_ = 0;
}

GamePlay::~GamePlay()
{
	delete board_;
	delete doodler_;
	for (Platform *pl : platforms_)
		delete pl;
	for (Spring *sp : springs_)
		delete sp;
	for (Enemy *en : enemies_)
		delete en;
	for (Pistol *pi : pistols_)
		delete pi;
}

GamePlay::Actions GamePlay::process()
{
	const bool isDynamic = readDynamicMap();
	if (!isDynamic)
		readStaticMap();
	window_->play_music(addresses_.nearEnemyEffect);
	window_->pause_music();
	while (true)
	{
		if (!checkForInGameEvents())
			return Actions::Quit;
		int lag = DELAY;
		while (lag >= DELAY)
		{
			const unsigned int prevTime = SDL_GetTicks();
			if (!update())
				return Actions::Quit;
			if (fellAfterGameOver_)
			{
				printOnScreen();
				return gameOverEvent();
			}
			if (isDynamic)
				checkForNewSequence();
			lag -= DELAY;
			lag += getAppropriateDelay(prevTime);
		}
		printOnScreen();
		delay(DELAY - lag);
	}
}

bool GamePlay::update()
{
	processPossibleChanges();
	doodler_->move();
	moveCameraUp();
	if (isEnemyNear())
		window_->resume_music();
	else
		window_->pause_music();
	if (doodler_->getStatus() == Doodler::Status::Falling)
	{
		if (!gameOver_)
			window_->play_sound_effect(addresses_.fallEffect);
		gameOver_ = true;
		if (!fallingHandler())
			fellAfterGameOver_ = true;
	}
	else if (doodler_->getStatus() != Doodler::Status::EnemyCrash)
		checkForCollision();
	return true;
}

void GamePlay::checkForNewSequence()
{
	if (sequenceStart_ + sequenceHeight_ - board_->getBase() > board_->getSize().y)
		return;
	findSuitableSequence();
}

void GamePlay::findSuitableSequence()
{
	const int height = sequenceStart_ + sequenceHeight_;
	vector<int> indexes;
	for (unsigned int i = 0; i < sequences_.size(); i++)
		if (height >= sequences_[i].start_ && (height <= sequences_[i].end_ || sequences_[i].end_ == -1))
			indexes.push_back(i);
	inGameSequenceAdd(indexes[rand() % indexes.size()], sequenceStart_ + sequenceHeight_);
}

bool GamePlay::readDynamicMap()
{
	ifstream dynamicInput;
	dynamicInput.open(addresses_.dynamicMap);
	if (!dynamicInput.is_open())
		return false;
	readValuesFromDynamicMap(dynamicInput);
	dynamicInput.close();
	return true;
}

void GamePlay::readStaticMap()
{
	ifstream staticInput;
	staticInput.open(addresses_.staticMap);
	if (!staticInput.is_open())
		throw runtime_error("FILE NOT FOUND!");
	readValuesFromStaticMap(staticInput);
	staticInput.close();
}

void GamePlay::readValuesFromDynamicMap(ifstream &input)
{
	int sequencesCount;
	input >> sequencesCount;
	for (int i = 0; i < sequencesCount; i++)
	{
		int start, end, totalHeight, entitiesCount;
		vector<string> types;
		vector<int> x, y;
		input >> start >> end >> totalHeight >> entitiesCount;
		for (int j = 0; j < entitiesCount; j++)
		{
			int singleX, singleY;
			string type;
			input >> singleX >> singleY >> type;
			x.push_back(singleX);
			y.push_back(singleY);
			types.push_back(type);
		}
		sequences_.emplace_back(5 * start / 8, 5 * end / 8, 5 * totalHeight / 8, types, x, y);
	}
}

GamePlay::Sequence::Sequence(int start, int end, int totalHeight, vector<string> types, vector<int> x, vector<int> y)
{
	start_ = start;
	end_ = end;
	totalHeight_ = totalHeight;
	types_ = types;
	x_ = x;
	y_ = y;
}

void GamePlay::readValuesFromStaticMap(ifstream &input)
{
	int entitiesCount;
	input >> entitiesCount;
	for (int i = 0; i < entitiesCount; i++)
	{
		int x, y;
		string type;
		input >> x >> y >> type;
		addDataFromMap(x, y, type, 0);
	}
}

void GamePlay::inGameSequenceAdd(int index, int base)
{
	const Sequence seq = sequences_[index];
	for (unsigned int i = 0; i < seq.types_.size(); i++)
		addDataFromMap(seq.x_[i], seq.y_[i], seq.types_[i], base - board_->getBase());
	sequenceStart_ = base;
	sequenceHeight_ = seq.totalHeight_;
}

void GamePlay::addDataFromMap(int x, int y, string type, int base = 0)
{
	if (type == "platform")
		platforms_.push_back(new NormalPlatform(getTopLeftFromMiddleDown(x, y, sizes_.normalAndMovablePlatforms) - Point(0, base), sizes_.normalAndMovablePlatforms, board_, doodler_));
	else if (type == "mplatform")
		platforms_.push_back(new MovablePlatform(getTopLeftFromMiddleDown(x, y, sizes_.normalAndMovablePlatforms) - Point(0, base), sizes_.normalAndMovablePlatforms, board_, doodler_));
	else if (type == "bplatform")
		platforms_.push_back(new BreakablePlatform(getTopLeftFromMiddleDown(x, y, sizes_.breakablePlatform[0]) - Point(0, base), sizes_.breakablePlatform[0], board_, doodler_));
	else if (type == "spring")
		springs_.push_back(new Spring(getTopLeftFromMiddleDown(x, y, sizes_.springs[0]) - Point(0, base), sizes_.springs, board_, doodler_));
	else if (type == "enemy")
		enemies_.push_back(new NormalEnemy(getTopLeftFromMiddleDown(x, y, sizes_.enemies[0]) - Point(0, base), sizes_.enemies[0], board_, doodler_));
	else if (type == "menemy")
		enemies_.push_back(new MovableEnemy(getTopLeftFromMiddleDown(x, y, sizes_.enemies[1]) - Point(0, base), sizes_.enemies[1], board_, doodler_));
}

Point GamePlay::getTopLeftFromMiddleDown(int x, int y, sizeVector size) const
{
	const int newX = (5 * x / 8) - size.x / 2;
	const int newY = sizes_.background.y - ((5 * y / 8) - size.y);
	return {newX, newY};
}

void GamePlay::processPossibleChanges()
{
	processPlatformChanges();
	processSpringChanges();
	processEnemyChanges();
	processPistolChanges();
}

int GamePlay::getAppropriateDelay(unsigned int prevTime)
{
	const unsigned int currentTime = SDL_GetTicks();
	return static_cast<int>(currentTime - prevTime);
}

GamePlay::Actions GamePlay::gameOverEvent() const
{
	while (true)
		while (window_->has_pending_event())
		{
			Event event = window_->poll_for_event();
			switch (event.get_type())
			{
			case Event::LCLICK:
				if (checkForButtonPress({locations_.menuButton.x, locations_.menuButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
										sizes_.menuButton, event))
				{
					menuButton();
					return Actions::Menu;
				}
				if (checkForButtonPress({locations_.playAgainButton.x, locations_.playAgainButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
										sizes_.playAgainButton, event))
				{
					playAgainButton();
					return Actions::PlayAgain;
				}
				if (checkForButtonPress(locations_.name, sizes_.nameEdit, event))
					editName();
				break;
			case Event::QUIT:
				return Actions::Quit;
			default:
				break;
			}
		}
}

void GamePlay::editName() const
{
	string prevName = name_;
	name_ = "";
	ostringstream nameStream;
	while (true)
	{
		while (window_->has_pending_event())
		{
			Event event = window_->poll_for_event();
			char pressedChar;
			switch (event.get_type())
			{
			case Event::KEY_PRESS:
				pressedChar = event.get_pressed_key();
				if (pressedChar >= 0 && pressedChar != 13 && nameStream.str().size() < NAME_LENGTH)
				{
					nameStream << pressedChar;
					name_ = nameStream.str();
					printOnScreen();
				}
				else if (pressedChar == 13)
				{
					endEditingName(nameStream, prevName);
					return;
				}
				break;
			case Event::LCLICK:
				endEditingName(nameStream, prevName);
				return;
			default:
				break;
			}
		}
		delay(DELAY);
	}
}

void GamePlay::endEditingName(const ostringstream &nameStream, const string &prevName) const
{
	if (nameStream.str().empty())
		name_ = prevName;
	else
		name_ = nameStream.str();
	printOnScreen();
}

void GamePlay::menuButton() const
{
	window_->draw_img(addresses_.menuButton[1],
					  Rectangle({locations_.menuButton.x, locations_.menuButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
								sizes_.menuButton.x, sizes_.menuButton.y));
	window_->update_screen();
	delay(BUTTON_PRESS_DELAY);
}

void GamePlay::playAgainButton() const
{
	window_->draw_img(addresses_.playAgainButton[1],
					  Rectangle({locations_.playAgainButton.x, locations_.playAgainButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
								sizes_.playAgainButton.x, sizes_.playAgainButton.y));
	window_->update_screen();
	delay(BUTTON_PRESS_DELAY);
}

bool GamePlay::checkForInGameEvents()
{
	while (window_->has_pending_event())
	{
		char pressedKey, releasedKey;
		const Event event = window_->poll_for_event();
		switch (event.get_type())
		{
		case Event::KEY_PRESS:
			pressedKey = event.get_pressed_key();
			if (pressedKey == 'd' || pressedKey == 'O')
				doodler_->startHorizontalMovement(Doodler::Direction::Right);
			else if (pressedKey == 'a' || pressedKey == 'P')
				doodler_->startHorizontalMovement(Doodler::Direction::Left);
			else if (pressedKey == 'R' || pressedKey == 'w' || pressedKey == ' ')
				if (doodler_->getStatus() != Doodler::Status::EnemyCrash)
					makePistol();
			break;
		case Event::KEY_RELEASE:
			releasedKey = event.get_pressed_key();
			if (releasedKey == 'd' || releasedKey == 'O')
				doodler_->stopHorizontalMovement(Doodler::Direction::Right);
			else if (releasedKey == 'a' || releasedKey == 'P')
				doodler_->stopHorizontalMovement(Doodler::Direction::Left);
			break;
		case Event::LCLICK:
			if (checkForButtonPress(locations_.pauseButton, sizes_.pauseButton, event))
				if (!pause())
					return false;
			break;
		case Event::QUIT:
			return false;
		default:
			break;
		}
	}
	return true;
}

bool GamePlay::pause()
{
	window_->draw_img(addresses_.pauseMenu);
	window_->draw_img(addresses_.resumeButton[0], Rectangle(locations_.resumeButton, sizes_.resumeButton.x, sizes_.resumeButton.y));
	window_->update_screen();
	while (true)
	{
		while (window_->has_pending_event())
		{
			Event event = window_->poll_for_event();
			switch (event.get_type())
			{
			case Event::QUIT:
				return false;
			case Event::LCLICK:
				if (checkForButtonPress(locations_.resumeButton, sizes_.resumeButton, event))
				{
					window_->draw_img(addresses_.resumeButton[1], Rectangle(locations_.resumeButton, sizes_.resumeButton.x, sizes_.resumeButton.y));
					window_->update_screen();
					delay(BUTTON_PRESS_DELAY);
					return true;
				}
				break;
			default:
				break;
			}
		}
	}
}

bool GamePlay::checkForButtonPress(Point topLeftLocation, sizeVector size, const Event &event)
{
	return event.get_mouse_position().x >= topLeftLocation.x && event.get_mouse_position().x <= topLeftLocation.x + size.x &&
		   event.get_mouse_position().y >= topLeftLocation.y && event.get_mouse_position().y <= topLeftLocation.y + size.y;
}

void GamePlay::moveCameraUp()
{
	if (doodler_->getBottomRightLocation().y >= sizes_.background.y / 2)
		return;
	const int pixel = sizes_.background.y / 2 - doodler_->getBottomRightLocation().y;
	moveCamera(pixel);
}

void GamePlay::moveCamera(int pixel)
{
	doodler_->moveWithCamera(pixel);
	for (Platform *platform : platforms_)
		platform->moveWithCamera(pixel);
	for (Spring *spring : springs_)
		spring->moveWithCamera(pixel);
	for (Enemy *enemy : enemies_)
		enemy->moveWithCamera(pixel);
	for (Pistol *pistol : pistols_)
		pistol->moveWithCamera(pixel);
	board_->changeBase(pixel);
}

void GamePlay::printOnScreen() const
{
	window_->clear();
	window_->draw_img(addresses_.background);
	printPlatforms();
	printSprings();
	printEnemies();
	printPistols();
	window_->draw_img(addresses_.scoreBackground, Rectangle({0, 0}, sizes_.scoreBackground));
	window_->draw_img(addresses_.pauseButton, Rectangle(locations_.pauseButton, sizes_.pauseButton.x, sizes_.pauseButton.y));
	window_->show_text(to_string(getHighScore()), locations_.scoreLocation, {100, 100, 100}, addresses_.comicFont, 30);
	printDoodler();
	if (doodler_->isCrashedToEnemy())
		printStars();
	if (gameOver_ && board_->getFallDistance() >= (FALL_DISTANCE_RATIO - 1) * board_->getSize().y)
		printGameOver();
	window_->update_screen();
}

void GamePlay::printPlatforms() const
{
	for (Platform *platform : platforms_)
		if (platform->shouldBePrinted())
			switch (platform->getType())
			{
			case Platform::Type::Normal:
				window_->draw_img(addresses_.normalPlatform, Rectangle(platform->getTopLeftLocation(), platform->getBottomRightLocation()));
				break;
			case Platform::Type::Movable:
				window_->draw_img(addresses_.MovablePlatform, Rectangle(platform->getTopLeftLocation(), platform->getBottomRightLocation()));
				break;
			case Platform::Type::Breakable:
				const int status = static_cast<int>(dynamic_cast<BreakablePlatform *>(platform)->getStatus());
				window_->draw_img(addresses_.breakablePlatform[status], Rectangle(platform->getTopLeftLocation(), platform->getBottomRightLocation()));
				break;
			}
}

void GamePlay::printSprings() const
{
	for (Spring *spring : springs_)
		if (spring->shouldBePrinted())
			window_->draw_img(addresses_.springs[static_cast<int>(spring->getSpringStatus())],
							  Rectangle(spring->getTopLeftLocation(), spring->getBottomRightLocation()));
}

void GamePlay::printEnemies() const
{
	for (Enemy *enemy : enemies_)
		if (enemy->shouldBePrinted())
			window_->draw_img(addresses_.enemies[static_cast<int>(enemy->getType())], Rectangle(enemy->getTopLeftLocation(), enemy->getBottomRightLocation()));
}

void GamePlay::printDoodler() const
{
	doodler_->checkIfDoodlerIsMostlyOut();
	const int status = static_cast<int>(doodler_->getPrintStatus());
	window_->draw_img(addresses_.doodler[status], Rectangle(doodler_->getTopLeftLocation(), doodler_->getBottomRightLocation()));
}

void GamePlay::printStars() const
{
	window_->draw_img(addresses_.stars[rand() % addresses_.stars.size()],
					  Rectangle(doodler_->getTopLeftLocation() + locations_.stars, sizes_.stars.x, sizes_.stars.y));
}

void GamePlay::printPistols() const
{
	for (Pistol *pistol : pistols_)
		window_->draw_img(addresses_.pistol, Rectangle(pistol->getTopLeftLocation(), pistol->getBottomRightLocation()));
}

void GamePlay::printGameOver() const
{
	window_->draw_img(addresses_.gameOver, Rectangle({0, static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
													 board_->getSize().x, board_->getSize().y));
	window_->draw_img(addresses_.menuButton[0],
					  Rectangle({locations_.menuButton.x, locations_.menuButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
								sizes_.menuButton.x, sizes_.menuButton.y));
	window_->draw_img(addresses_.playAgainButton[0],
					  Rectangle({locations_.playAgainButton.x, locations_.playAgainButton.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
								sizes_.playAgainButton.x, sizes_.playAgainButton.y));
	window_->show_text(to_string(getHighScore()),
					   {locations_.gameOverScore.x, locations_.gameOverScore.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
					   BLACK, addresses_.comicFont, 30);
	window_->show_text(to_string(checkForOverallHighScore()),
					   {locations_.overallHighScore.x, locations_.overallHighScore.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
					   BLACK, addresses_.comicFont, 30);
	window_->show_text(name_,
					   {locations_.name.x, locations_.name.y + static_cast<int>(FALL_DISTANCE_RATIO * board_->getSize().y - board_->getFallDistance())},
					   BLACK, addresses_.comicFont, 30);
}

int GamePlay::checkForOverallHighScore() const
{
	if (getHighScore() > highScore_)
		highScore_ = getHighScore();
	return highScore_;
}

void GamePlay::processPlatformChanges()
{
	for (int i = static_cast<int>(platforms_.size()) - 1; i >= 0; i--)
	{
		if (platforms_[i]->shouldBeDeleted())
		{
			platforms_.erase(platforms_.begin() + i);
			continue;
		}
		platforms_[i]->setWasUnderDoodler();
		switch (platforms_[i]->getType())
		{
		case Platform::Type::Normal:
			break;
		case Platform::Type::Movable:
			dynamic_cast<MovablePlatform *>(platforms_[i])->move();
			break;
		case Platform::Type::Breakable:
			BreakablePlatform *platform = dynamic_cast<BreakablePlatform *>(platforms_[i]);
			const BreakablePlatform::Status status = platform->getStatus();
			if (status != BreakablePlatform::Status::NotBroken)
				platform->checkForBreakLevel(sizes_.breakablePlatform[static_cast<int>(status)]);
		}
	}
}

void GamePlay::processSpringChanges()
{
	for (int i = static_cast<int>(springs_.size()) - 1; i >= 0; i--)
	{
		if (springs_[i]->shouldBeDeleted())
		{
			springs_.erase(springs_.begin() + i);
			continue;
		}
		springs_[i]->setWasUnderDoodler();
		springs_[i]->checkForPossibleStatusChange();
	}
}

void GamePlay::processEnemyChanges()
{
	for (int i = static_cast<int>(enemies_.size()) - 1; i >= 0; i--)
	{
		if (enemies_[i]->shouldBeDeleted())
		{
			enemies_.erase(enemies_.begin() + i);
			continue;
		}
		bool isDeleted = false;
		for (unsigned int j = 0; j < pistols_.size(); j++)
			if (enemies_[i]->checkForPistolCollision(pistols_[j]->getTopLeftLocation(), sizes_.pistol))
			{
				enemies_.erase(enemies_.begin() + i);
				pistols_.erase(pistols_.begin() + j);
				isDeleted = true;
				window_->play_sound_effect(addresses_.pistolKilledMonsterEffect);
			}
		if (isDeleted)
			continue;
		enemies_[i]->hover();
		enemies_[i]->setWasUnderDoodler();
		enemies_[i]->checkForDestroy();
		if (enemies_[i]->getType() == Enemy::Type::Movable)
			dynamic_cast<MovableEnemy *>(enemies_[i])->move();
	}
}

void GamePlay::processPistolChanges()
{
	for (int i = static_cast<int>(pistols_.size()) - 1; i >= 0; i--)
	{
		if (pistols_[i]->shouldBeDeleted())
		{
			pistols_.erase(pistols_.begin() + i);
			continue;
		}
		pistols_[i]->move();
	}
}

void GamePlay::checkForCollision()
{
	if (checkForEnemyCollision())
		return;
	if (checkForSpringCollision())
		return;
	if (checkForPlatformCollision())
		return;
}

bool GamePlay::checkForPlatformCollision()
{
	if (!doodler_->isGoingDownward())
		return false;
	for (Platform *platform : platforms_)
		if (platform->checkForCollision(sizes_.doodler[static_cast<int>(doodler_->getPrintStatus())]))
		{
			switch (platform->getType())
			{
			case Platform::Type::Normal:
			case Platform::Type::Movable:
				doodler_->handleCollision(Doodler::Collision::Platform, platform->getTopLeftLocation());
				window_->play_sound_effect(addresses_.jumpEffect);
				break;
			case Platform::Type::Breakable:
				dynamic_cast<BreakablePlatform *>(platform)->breakPlatform(sizes_.breakablePlatform[1]);
				window_->play_sound_effect(addresses_.breakPlatformEffect);
				break;
			}
			return true;
		}
	return false;
}

bool GamePlay::checkForSpringCollision()
{
	if (!doodler_->isGoingDownward())
		return false;
	for (Spring *spring : springs_)
		if (spring->checkForCollision(sizes_.doodler[static_cast<int>(doodler_->getPrintStatus())]))
		{
			spring->expand();
			doodler_->handleCollision(Doodler::Collision::Spring, spring->getTopLeftLocation());
			window_->play_sound_effect(addresses_.springEffect);
			return true;
		}
	return false;
}

bool GamePlay::checkForEnemyCollision()
{
	for (Enemy *enemy : enemies_)
		if (enemy->checkForCollision(sizes_.doodler[static_cast<int>(doodler_->getPrintStatus())]))
		{
			if (enemy->wasUnderDoodler())
			{
				enemy->destroy();
				doodler_->handleCollision(Doodler::Collision::Platform, enemy->getTopLeftLocation());
				window_->play_sound_effect(addresses_.jumpOnMonsterEffect);
			}
			else
			{
				doodler_->handleCollision(Doodler::Collision::Enemy, enemy->getTopLeftLocation());
				window_->play_sound_effect(addresses_.monsterCrashEffect);
			}
			return true;
		}
	return false;
}

bool GamePlay::fallingHandler()
{
	if (!board_->shouldFallMore() && doodler_->getTopLeftLocation().y >= sizes_.background.y)
		return false;
	if (board_->shouldFallMore())
		moveCamera(-board_->getFallDistanceForThisRound());
	return true;
}

bool GamePlay::isEnemyNear()
{
	for (Enemy *enemy : enemies_)
		if (enemy->getTopLeftLocation().y >= -board_->getSize().y / 2)
			return true;
	return false;
}

void GamePlay::makePistol()
{
	doodler_->shootPistol();
	const Point pistolTopLeftLocation = {doodler_->getTopLeftLocation().x + sizes_.doodler[static_cast<int>(doodler_->getPrintStatus())].x / 2 - sizes_.pistol.x / 2,
										 doodler_->getTopLeftLocation().y - sizes_.pistol.y};
	pistols_.push_back(new Pistol(board_, sizes_.pistol, pistolTopLeftLocation));
	window_->play_sound_effect(addresses_.pistolShootEffect);
}
