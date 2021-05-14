#include "MovableEnemy.hpp"

MovableEnemy::MovableEnemy(Point topLeftLocation, sizeVector size, GameBoard* board, Doodler* doodler) : Enemy(topLeftLocation, size, board, doodler)
{
	speed_ = ENEMY_SPEED;
}

Enemy::Type MovableEnemy::getType()
{
	return Type::Movable;
}

void MovableEnemy::move()
{
	topLeftLocation_.x += static_cast<int>(speed_ * board_->getDelay());
	if (crashToWall())
		speed_ *= -1;
}

bool MovableEnemy::crashToWall() const
{
	return topLeftLocation_.x <= 0 || topLeftLocation_.x + size_.x >= board_->getSize().x;
}
