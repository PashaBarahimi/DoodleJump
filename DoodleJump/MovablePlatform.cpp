#include "MovablePlatform.hpp"

MovablePlatform::MovablePlatform(Point location, const sizeVector size, GameBoard* board, Doodler* doodler) : Platform(location, size, board, doodler)
{
	speed_ = PLATFORM_SPEED;
}

Platform::Type MovablePlatform::getType()
{
	return Type::Movable;
}

void MovablePlatform::move()
{
	topLeftLocation_.x += static_cast<int>(speed_ * board_->getDelay());
	if (crashToWall())
		speed_ *= -1;
}

bool MovablePlatform::crashToWall() const
{
	return topLeftLocation_.x <= 0 || topLeftLocation_.x + size_.x >= board_->getSize().x;
}
