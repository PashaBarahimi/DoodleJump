#include "include/doodler.hpp"

Doodler::Doodler(const Point location, const std::vector<sizeVector> &size, GameBoard *board) : topLeftLocation_(location), size_(size), board_(board)
{
	isGoingDownward_ = false;
	direction_ = Direction::Right;
	statusChangeCounter_ = 0;
	verticalVelocity_ = NORMAL_JUMP_SPEED;
	verticalAcceleration_ = DEFAULT_ACCELERATION;
	hasHorizontalVelocity_ = false;
	status_ = Status::Normal;
	scoreInScreen_ = 0;
	hasCrashedToEnemy_ = false;
}

void Doodler::startHorizontalMovement(Direction direction)
{
	direction_ = direction;
	hasHorizontalVelocity_ = true;
}

void Doodler::stopHorizontalMovement(Direction direction)
{
	if (direction != direction_)
		return;
	hasHorizontalVelocity_ = false;
}

void Doodler::move()
{
	topLeftLocation_.y -= static_cast<int>(verticalVelocity_ * board_->getDelay());
	checkForHigherScore();
	verticalVelocity_ += verticalAcceleration_ * board_->getDelay();
	isGoingDownward_ = verticalVelocity_ <= 0;
	if (hasHorizontalVelocity_)
		topLeftLocation_.x += static_cast<int>(static_cast<int>(direction_) * HORIZONTAL_SPEED * board_->getDelay());
	checkForFalling();
	checkForPossibleStatusChange();
}

void Doodler::checkForHigherScore()
{
	const int currentLocation = board_->getSize().y - getBottomRightLocation().y;
	if (scoreInScreen_ < currentLocation)
		scoreInScreen_ = currentLocation;
}

void Doodler::checkForFalling()
{
	if (topLeftLocation_.y < board_->getSize().y)
		return;
	status_ = Status::Falling;
	statusChangeCounter_ = 0;
}

void Doodler::checkForPossibleStatusChange()
{
	if (statusChangeCounter_ > 0)
	{
		statusChangeCounter_--;
		if (statusChangeCounter_ <= 0)
		{
			status_ = Status::Normal;
			statusChangeCounter_ = 0;
			verticalAcceleration_ = DEFAULT_ACCELERATION;
		}
	}
}

void Doodler::handleCollision(Collision collision, Point location = {0, 0})
{
	switch (collision)
	{
	case Collision::Platform:
		jumpOnPlatform(location);
		break;
	case Collision::Enemy:
		crashToEnemy();
		break;
	case Collision::Spring:
		jumpOnSpring(location);
		break;
	}
}

void Doodler::jumpOnPlatform(Point location)
{
	topLeftLocation_.y = location.y - size_[static_cast<int>(getPrintStatus())].y;
	verticalVelocity_ = NORMAL_JUMP_SPEED;
	status_ = Status::Bent;
	statusChangeCounter_ = NORMAL_BENT_KNEE_COUNTER;
	isGoingDownward_ = false;
}

void Doodler::jumpOnSpring(Point location)
{
	topLeftLocation_.y = location.y - size_[static_cast<int>(getPrintStatus())].y;
	verticalVelocity_ = SPRING_JUMP_SPEED;
	status_ = Status::Bent;
	statusChangeCounter_ = SPRING_BENT_KNEE_COUNTER;
	isGoingDownward_ = false;
}

void Doodler::crashToEnemy()
{
	verticalVelocity_ = 0;
	verticalAcceleration_ = DEFAULT_ACCELERATION;
	status_ = Status::EnemyCrash;
	hasCrashedToEnemy_ = true;
	statusChangeCounter_ = 0;
	isGoingDownward_ = true;
}

void Doodler::moveWithCamera(int pixel)
{
	topLeftLocation_.y += pixel;
	scoreInScreen_ -= pixel;
}

void Doodler::reverseLocation()
{
	if (topLeftLocation_.x < 0)
		topLeftLocation_.x += board_->getSize().x;
	else
		topLeftLocation_.x -= board_->getSize().x;
}

void Doodler::checkIfDoodlerIsMostlyOut()
{
	if (topLeftLocation_.x + size_[static_cast<int>(getPrintStatus())].x / 2 < 0 || topLeftLocation_.x + size_[static_cast<int>(getPrintStatus())].x / 2 > board_->getSize().x)
		reverseLocation();
}

Doodler::PrintStatus Doodler::getPrintStatus() const
{
	if (status_ == Status::ShootingPistol)
		return PrintStatus::ShootingPistol;
	switch (direction_)
	{
	case Direction::Right:
		if (status_ == Status::Bent)
			return PrintStatus::RightJumping;
		return PrintStatus::Right;
	case Direction::Left:
		if (status_ == Status::Bent)
			return PrintStatus::LeftJumping;
		return PrintStatus::Left;
	}
	return PrintStatus::Right;
}

void Doodler::shootPistol()
{
	status_ = Status::ShootingPistol;
	statusChangeCounter_ = PISTOL_SHOOT_COUNTER;
}
