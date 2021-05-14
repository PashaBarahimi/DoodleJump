#include "GameBoard.hpp"

GameBoard::GameBoard(const sizeVector size, double delay) : size_(size), delay_(delay)
{
	base_ = 0;
	fallDistance_ = 0;
}

void GameBoard::changeBase(int pixel)
{
	base_ += pixel;
}

int GameBoard::getFallDistanceForThisRound()
{
	const int pixel = static_cast<int>(FALLING_SPEED * delay_);
	fallDistance_ += pixel;
	return pixel;
}
