#ifndef SPRING_HPP
#define SPRING_HPP

#include "rsdl.hpp"
#include "GameBoard.hpp"
#include "Doodler.hpp"

#include <vector>

typedef Point sizeVector;

constexpr int EXPAND_DELAY_COUNTER = 20;

class Spring
{
public:
	enum class SpringStatus
	{
		Compressed = 0,
		Normal = 1
	};
	Spring(Point topLeftLocation, std::vector<sizeVector> sizes, GameBoard* board, Doodler* doodler);
	SpringStatus getSpringStatus() const { return status_; }
	Point getTopLeftLocation() const { return bottomRightLocation_ - sizes_[static_cast<int>(status_)]; }
	Point getBottomRightLocation() const { return bottomRightLocation_; }
	bool checkForCollision(sizeVector doodlerSize) const;
	void setWasUnderDoodler();
	void moveWithCamera(int pixel);
	bool shouldBePrinted() const;
	bool shouldBeDeleted() const;
	void checkForPossibleStatusChange();
	void expand();
private:
	bool wasUnderDoodler_;
	GameBoard* board_;
	Doodler* doodler_;
	Point bottomRightLocation_;
	std::vector<sizeVector> sizes_;
	SpringStatus status_;
	int statusChangeCounter_;
	bool isOverDoodler() const;
	bool doesHorizontalCoordinateMatch(sizeVector doodlerSize) const;
};

#endif
