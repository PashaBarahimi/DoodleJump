#include "include/enemy.hpp"

Enemy::Enemy(Point topLeftLocation, sizeVector size, GameBoard *board, Doodler *doodler) : topLeftLocation_(topLeftLocation), size_(size), board_(board), doodler_(doodler)
{
	wasUnderDoodler_ = false;
	destroyed_ = false;
	statusChangeCounter_ = 0;
	hoverTime_ = 0;
}

void Enemy::setWasUnderDoodler()
{
	wasUnderDoodler_ = topLeftLocation_.y >= doodler_->getBottomRightLocation().y;
}

bool Enemy::shouldBePrinted() const
{
	return (topLeftLocation_.y + size_.y >= 0) && (topLeftLocation_.y <= board_->getSize().y);
}

bool Enemy::shouldBeDeleted() const
{
	return topLeftLocation_.y > board_->getSize().y || destroyed_;
}

void Enemy::moveWithCamera(int pixel)
{
	topLeftLocation_.y += pixel;
}

bool Enemy::doesHorizontalCoordinateMatch(sizeVector doodlerSize) const
{
	const int rightCorner = doodler_->getBottomRightLocation().x - static_cast<int>(DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN * doodlerSize.x);
	const int leftCorner = doodler_->getBottomRightLocation().x - static_cast<int>((1 - DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN) * doodlerSize.x);
	return (rightCorner >= topLeftLocation_.x && rightCorner <= topLeftLocation_.x + size_.x) ||
		   (leftCorner >= topLeftLocation_.x && leftCorner <= topLeftLocation_.x + size_.x) ||
		   (rightCorner >= topLeftLocation_.x + size_.x && leftCorner <= topLeftLocation_.x);
}

bool Enemy::doesVerticalCoordinateMatch(sizeVector doodlerSize) const
{
	const int topCorner = doodler_->getTopLeftLocation().y + static_cast<int>(DOODLER_TOP_SIDE_MARGIN * doodlerSize.y);
	const int bottomCorner = doodler_->getBottomRightLocation().y;
	return (topCorner >= topLeftLocation_.y && topCorner <= topLeftLocation_.y + size_.y) ||
		   (bottomCorner >= topLeftLocation_.y && bottomCorner <= topLeftLocation_.y + size_.y) ||
		   (topCorner <= topLeftLocation_.y && bottomCorner >= topLeftLocation_.y + size_.y);
}

bool Enemy::checkForCollision(sizeVector doodlerSize) const
{
	return doesHorizontalCoordinateMatch(doodlerSize) && doesVerticalCoordinateMatch(doodlerSize);
}

void Enemy::destroy()
{
	statusChangeCounter_ = DESTROY_DELAY;
}

void Enemy::checkForDestroy()
{
	if (statusChangeCounter_ <= 0)
		return;
	topLeftLocation_.y += DESTROY_PIXEL_CHANGE;
	statusChangeCounter_--;
	if (statusChangeCounter_ <= 0)
		destroyed_ = true;
}

void Enemy::hover()
{
	topLeftLocation_.y += hoverTime_ < MAXIMUM_HOVER_TIME / 2 ? 1 : -1;
	hoverTime_ += static_cast<int>(board_->getDelay() * 1000);
	if (hoverTime_ > MAXIMUM_HOVER_TIME)
		hoverTime_ = 0;
}

bool Enemy::checkForPistolCollision(Point pistolTopLeftLocation, sizeVector pistolSize) const
{
	return ((pistolTopLeftLocation.x >= topLeftLocation_.x && pistolTopLeftLocation.x <= getBottomRightLocation().x) ||
			(pistolTopLeftLocation.x + pistolSize.x >= topLeftLocation_.x && pistolTopLeftLocation.x + size_.x <= getBottomRightLocation().x)) &&
		   ((pistolTopLeftLocation.y >= topLeftLocation_.y && pistolTopLeftLocation.y <= getBottomRightLocation().y) ||
			(pistolTopLeftLocation.y + size_.y >= topLeftLocation_.y && pistolTopLeftLocation.y + size_.y <= getBottomRightLocation().y));
}
