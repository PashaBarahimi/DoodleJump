#include "include/platform.hpp"

Platform::Platform(const Point location, const sizeVector size, GameBoard *board, Doodler *doodler)
	: topLeftLocation_(location), size_(size), board_(board), doodler_(doodler)
{
	wasUnderDoodler_ = false;
}

bool Platform::checkForCollision(sizeVector doodlerSize) const
{
	return wasUnderDoodler_ && isOverDoodler() && doesHorizontalCoordinateMatch(doodlerSize);
}

bool Platform::isOverDoodler() const
{
	return topLeftLocation_.y <= doodler_->getBottomRightLocation().y;
}

bool Platform::doesHorizontalCoordinateMatch(sizeVector doodlerSize) const
{
	const int rightCorner = doodler_->getBottomRightLocation().x - static_cast<int>(DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN * doodlerSize.x);
	const int leftCorner = doodler_->getBottomRightLocation().x - static_cast<int>((1 - DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN) * doodlerSize.x);
	return (rightCorner >= topLeftLocation_.x && rightCorner <= topLeftLocation_.x + size_.x) ||
		   (leftCorner >= topLeftLocation_.x && leftCorner <= topLeftLocation_.x + size_.x) ||
		   (rightCorner >= topLeftLocation_.x + size_.x && leftCorner <= topLeftLocation_.x);
}

void Platform::setWasUnderDoodler()
{
	wasUnderDoodler_ = topLeftLocation_.y >= doodler_->getBottomRightLocation().y;
}

void Platform::moveWithCamera(int pixel)
{
	topLeftLocation_.y += pixel;
}

bool Platform::shouldBePrinted() const
{
	return (topLeftLocation_.y + size_.y >= 0) && (topLeftLocation_.y <= board_->getSize().y);
}

bool Platform::shouldBeDeleted() const
{
	return topLeftLocation_.y > board_->getSize().y;
}
