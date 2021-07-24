#ifndef BREAKABLE_PLATFORM_HPP
#define BREAKABLE_PLATFORM_HPP

#include "platform.hpp"

constexpr int BREAK_DELAY_COUNTER = 5;
constexpr int PIXEL_CHANGE = 4;

class BreakablePlatform : public Platform
{
public:
	enum class Status
	{
		NotBroken = 0,
		JustBroken = 1,
		AlmostBroken = 2,
		CompletelyBroken = 3
	};
	BreakablePlatform(const Point location, const sizeVector size, GameBoard* board, Doodler* doodler);
	Type getType() override;
	void breakPlatform(sizeVector size);
	Status getStatus() const { return status_; }
	void checkForBreakLevel(sizeVector size);
	bool shouldBeDeleted() const override;
private:
	int statusChangeCounter_;
	Status status_;
};

#endif
