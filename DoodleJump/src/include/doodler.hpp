#ifndef DOODLER_HPP
#define DOODLER_HPP

#include "rsdl.hpp"
#include "game_board.hpp"
#include <vector>

// Speed unit is pixels/seconds
constexpr double HORIZONTAL_SPEED = 350;
constexpr double NORMAL_JUMP_SPEED = 750;
constexpr double SPRING_JUMP_SPEED = 1500;

// Acceleration unit is pixels/seconds^2
constexpr double DEFAULT_ACCELERATION = -1500;

// How many delays take to change some special status
constexpr int NORMAL_BENT_KNEE_COUNTER = 25;
constexpr int SPRING_BENT_KNEE_COUNTER = 50;
constexpr int PISTOL_SHOOT_COUNTER = 20;

// 30% of each side should not be included to calculate collisions because of picture's margin
constexpr double DOODLER_LEFT_SIDE_AND_RIGHT_SIDE_MARGIN = 0.3;

// 25% of top side should not be included to calculate collisions because of picture's margin
constexpr double DOODLER_TOP_SIDE_MARGIN = 0.25;

typedef Point sizeVector;

class Doodler
{
public:
	enum class Collision
	{
		Platform,
		Spring,
		Enemy
	};
	enum class Status
	{
		Normal,
		Bent,
		EnemyCrash,
		Falling,
		ShootingPistol
	};
	enum class PrintStatus
	{
		Right = 0,
		Left = 1,
		RightJumping = 2,
		LeftJumping = 3,
		ShootingPistol = 4
	};
	enum class Direction
	{
		Right = 1,
		Left = -1
	};
	Doodler(const Point location, const std::vector<sizeVector> &size, GameBoard *board);
	Point getTopLeftLocation() const { return topLeftLocation_; }
	Point getBottomRightLocation() const { return topLeftLocation_ + size_[static_cast<int>(getPrintStatus())]; }
	int getScoreInScreen() const { return scoreInScreen_; }
	bool isGoingDownward() const { return isGoingDownward_; }
	bool isCrashedToEnemy() const { return hasCrashedToEnemy_; }
	Status getStatus() const { return status_; }
	PrintStatus getPrintStatus() const;
	void move();
	void startHorizontalMovement(Direction direction);
	void stopHorizontalMovement(Direction direction);
	void handleCollision(Collision collision, Point location);
	void moveWithCamera(int pixel);
	void checkIfDoodlerIsMostlyOut();
	void shootPistol();

private:
	bool isGoingDownward_;
	Direction direction_;
	int statusChangeCounter_;
	int scoreInScreen_;
	double verticalVelocity_;
	double verticalAcceleration_;
	bool hasHorizontalVelocity_;
	bool hasCrashedToEnemy_;
	Point topLeftLocation_;
	std::vector<sizeVector> size_;
	GameBoard *board_;
	Status status_;
	void checkForHigherScore();
	void checkForFalling();
	void checkForPossibleStatusChange();
	void jumpOnPlatform(Point location);
	void jumpOnSpring(Point location);
	void crashToEnemy();
	void reverseLocation();
};

#endif
