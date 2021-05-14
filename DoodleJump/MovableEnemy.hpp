#ifndef MOVABLE_ENEMY_HPP
#define MOVABLE_ENEMY_HPP

#include "Enemy.hpp"

constexpr double ENEMY_SPEED = 200;

class MovableEnemy : public Enemy
{
public:
	MovableEnemy(Point topLeftLocation, sizeVector size, GameBoard* board, Doodler* doodler);
	Type getType() override;
	void move();
private:
	double speed_;
	bool crashToWall() const;
};

#endif
