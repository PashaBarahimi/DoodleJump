#ifndef PISTOL_HPP
#define PISTOL_HPP

#include "rsdl.hpp"
#include "game_board.hpp"

constexpr double PISTOL_SPEED = 1500;

class Pistol
{
public:
	Pistol(GameBoard *board, sizeVector size, Point topLeftLocation);
	void move();
	bool shouldBeDeleted() const { return topLeftLocation_.y + size_.y <= 0; }
	Point getTopLeftLocation() const { return topLeftLocation_; }
	Point getBottomRightLocation() const { return topLeftLocation_ + size_; }
	void moveWithCamera(int pixel);

private:
	GameBoard *board_;
	sizeVector size_;
	Point topLeftLocation_;
};

#endif
