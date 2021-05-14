#include "BreakablePlatform.hpp"

BreakablePlatform::BreakablePlatform(const Point location, const sizeVector size, GameBoard* board, Doodler* doodler) : Platform(location, size, board, doodler)
{
	statusChangeCounter_ = 0;
	status_ = Status::NotBroken;
}

void BreakablePlatform::breakPlatform(sizeVector size)
{
	status_ = Status::JustBroken;
	size_ = size;
	statusChangeCounter_ = BREAK_DELAY_COUNTER;
}

void BreakablePlatform::checkForBreakLevel(sizeVector size)
{
	if (statusChangeCounter_ > 0)
	{
		topLeftLocation_.y += PIXEL_CHANGE;
		statusChangeCounter_--;
		return;
	}
	switch (status_)
	{
	case Status::NotBroken:
	case Status::CompletelyBroken:
		statusChangeCounter_ = 0;
		break;
	case Status::JustBroken:
		statusChangeCounter_ = BREAK_DELAY_COUNTER;
		status_ = Status::AlmostBroken;
		size_ = size;
		break;
	case Status::AlmostBroken:
		statusChangeCounter_ = BREAK_DELAY_COUNTER;
		status_ = Status::CompletelyBroken;
		size_ = size;
		break;
	}
}

bool BreakablePlatform::shouldBeDeleted() const
{
	return Platform::shouldBeDeleted() || (status_ == Status::CompletelyBroken && statusChangeCounter_ <= 0);
}

Platform::Type BreakablePlatform::getType()
{
	return Type::Breakable;
}

