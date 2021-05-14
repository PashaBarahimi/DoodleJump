#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "rsdl.hpp"
#include "Doodler.hpp"
#include "GameBoard.hpp"

typedef Point sizeVector;

constexpr int DESTROY_DELAY = 10;
constexpr int DESTROY_PIXEL_CHANGE = 5;

constexpr int MAXIMUM_HOVER_TIME = 500;

class Enemy
{
public:
	enum class Type
	{
		Normal = 0,
		Movable = 1
	};
	Enemy(Point topLeftLocation, sizeVector size, GameBoard* board, Doodler* doodler);
	virtual Type getType() = 0;
	bool checkForCollision(sizeVector doodlerSize) const;
	bool checkForPistolCollision(Point pistolTopLeftLocation, sizeVector pistolSize) const;
	Point getTopLeftLocation() const { return topLeftLocation_; }
	Point getBottomRightLocation() const { return topLeftLocation_ + size_; }
	void setWasUnderDoodler();
	bool wasUnderDoodler() const { return wasUnderDoodler_; }
	void moveWithCamera(int pixel);
	bool shouldBePrinted() const;
	bool shouldBeDeleted() const;
	void checkForDestroy();
	void destroy();
	void hover();
protected:
	Point topLeftLocation_;
	sizeVector size_;
	GameBoard* board_;
	Doodler* doodler_;
	bool wasUnderDoodler_;
	bool destroyed_;
	int statusChangeCounter_;
	int hoverTime_;
	bool doesHorizontalCoordinateMatch(sizeVector doodlerSize) const;
	bool doesVerticalCoordinateMatch(sizeVector doodlerSize) const;
};

#endif
