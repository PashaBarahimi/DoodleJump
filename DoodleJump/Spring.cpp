#include "Spring.hpp"

Spring::Spring(Point topLeftLocation, std::vector<sizeVector> sizes, GameBoard* board, Doodler* doodler) :
	board_(board), sizes_(sizes), doodler_(doodler)
{
	wasUnderDoodler_ = false;
	status_ = SpringStatus::Compressed;
	statusChangeCounter_ = 0;
	bottomRightLocation_ = topLeftLocation + sizes_[static_cast<int>(status_)];
}

bool Spring::checkForCollision(sizeVector doodlerSize) const
{
	return wasUnderDoodler_ && isOverDoodler() && doesHorizontalCoordinateMatch(doodlerSize);
}

void Spring::setWasUnderDoodler()
{
	wasUnderDoodler_ = getTopLeftLocation().y >= doodler_->getBottomRightLocation().y;
}

bool Spring::isOverDoodler() const
{
	return getTopLeftLocation().y <= doodler_->getBottomRightLocation().y;
}

bool Spring::doesHorizontalCoordinateMatch(sizeVector doodlerSize) const
{
	const int rightCorner = doodler_->getBottomRightLocation().x - static_cast<int>(DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN * doodlerSize.x);
	const int leftCorner = doodler_->getBottomRightLocation().x - static_cast<int>((1 - DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN) * doodlerSize.x);
	return (rightCorner >= getTopLeftLocation().x && rightCorner <= getTopLeftLocation().x + sizes_[static_cast<int>(status_)].x) ||
		(leftCorner >= getTopLeftLocation().x && leftCorner <= getTopLeftLocation().x + sizes_[static_cast<int>(status_)].x) ||
		(rightCorner >= getTopLeftLocation().x + sizes_[static_cast<int>(status_)].x && leftCorner <= getTopLeftLocation().x);
}

void Spring::moveWithCamera(int pixel)
{
	bottomRightLocation_.y += pixel;
}

bool Spring::shouldBePrinted() const
{
	return (getTopLeftLocation().y + sizes_[static_cast<int>(status_)].y >= 0) && (getTopLeftLocation().y <= board_->getSize().y);
}

bool Spring::shouldBeDeleted() const
{
	return getTopLeftLocation().y > board_->getSize().y;
}

void Spring::expand()
{
	status_ = SpringStatus::Normal;
	statusChangeCounter_ = EXPAND_DELAY_COUNTER;
}

void Spring::checkForPossibleStatusChange()
{
	if (statusChangeCounter_ <= 0)
		status_ = SpringStatus::Compressed;
	statusChangeCounter_--;
}
