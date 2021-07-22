#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "rsdl.hpp"
#include "GameBoard.hpp"
#include "Doodler.hpp"

typedef Point sizeVector;

class Platform
{
public:
	enum class Type
	{
		Normal,
		Movable,
		Breakable
	};
	Platform(const Point location, const sizeVector size, GameBoard* board, Doodler* doodler);
	virtual ~Platform() = default;
	virtual Type getType() = 0;
	bool checkForCollision(sizeVector doodlerSize) const;
	Point getTopLeftLocation() const { return topLeftLocation_; }
	Point getBottomRightLocation() const { return topLeftLocation_ + size_; }
	void setWasUnderDoodler();
	void moveWithCamera(int pixel);
	bool shouldBePrinted() const;
	virtual bool shouldBeDeleted() const;
protected:
	bool wasUnderDoodler_;
	Point topLeftLocation_;
	sizeVector size_;
	GameBoard* board_;
	Doodler* doodler_;
	bool isOverDoodler() const;
	bool doesHorizontalCoordinateMatch(sizeVector doodlerSize) const;
};

#endif
