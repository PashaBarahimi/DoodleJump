#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "rsdl.hpp"

typedef Point sizeVector;

// Speed unit is pixels/seconds
constexpr int FALLING_SPEED = 3500;

// How many pixels should the camera change while falling down = This number * screenHeight
constexpr double FALL_DISTANCE_RATIO = 3;

class GameBoard
{
public:
	GameBoard(const sizeVector size, double delay);
	double getDelay() const { return delay_; }
	sizeVector getSize() const { return size_; }
	int getBase() const { return base_; }
	bool shouldFallMore() const { return FALL_DISTANCE_RATIO * size_.y > fallDistance_; }
	int getFallDistanceForThisRound();
	int getFallDistance() const { return fallDistance_; }
	void changeBase(int pixel);
private:
	sizeVector size_;
	double delay_; // in seconds
	int base_;
	int fallDistance_;
};

#endif
