#include "include/spaceship.hpp"

Spaceship::Spaceship(GameBoard *board, Point topLeftLocation, sizeVector size) : board_(board), topLeftLocation_(topLeftLocation), mainTopLeftLocation_(topLeftLocation), size_(size)
{
	time_ = 0;
}

void Spaceship::move()
{
	topLeftLocation_.x = static_cast<int>(30 * sin(board_->getDelay() * static_cast<long double>(time_)) + mainTopLeftLocation_.x);
	topLeftLocation_.y = static_cast<int>(20 * sin(board_->getDelay() * 2 * static_cast<long double>(time_)) + mainTopLeftLocation_.y);
	time_++;
}
