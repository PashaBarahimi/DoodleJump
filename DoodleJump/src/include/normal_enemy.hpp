#ifndef NORMAL_ENEMY_HPP
#define NORMAL_ENEMY_HPP

#include "enemy.hpp"

class NormalEnemy : public Enemy
{
public:
	NormalEnemy(Point topLeftLocation, sizeVector size, GameBoard *board, Doodler *doodler);
	Type getType() override;
};

#endif
