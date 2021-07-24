#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "game_board.hpp"
#include <cmath>

class Spaceship
{
public:
	Point getTopLeftLocation() const { return topLeftLocation_; }
	Point getBottomRightLocation() const { return topLeftLocation_ + size_; }
	void move();
	Spaceship(GameBoard *board, Point topLeftLocation, sizeVector size);

private:
	GameBoard *board_;
	Point topLeftLocation_;
	Point mainTopLeftLocation_;
	sizeVector size_;
	long long int time_ = 0;
};

#endif
