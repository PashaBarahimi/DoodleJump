#ifndef Movable_PLATFORM_HPP
#define Movable_PLATFORM_HPP

#include "Platform.hpp"

constexpr double PLATFORM_SPEED = 100;

class MovablePlatform : public Platform
{
public:
	MovablePlatform(const Point location, const sizeVector size, GameBoard* board, Doodler* doodler);
	Type getType() override;
	void move();
private:
	double speed_;
	bool crashToWall() const;
};

#endif
